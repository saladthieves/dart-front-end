#include "utf8_bytes_scanner.hpp"

#include "internal_utils.hpp"
#include "token/characters.hpp"
#include "token/comment_token.hpp"
#include "token/synthetic_token.hpp"
#include "token/token_type.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace {
using namespace token::chars;

using token::CommentToken;
using token::DartDocToken;
using token::LanguageVersionToken;
using token::StringToken;
using token::type::TokenType;

using Int = Scanner<>::Int;

/*
Format hack table for checking if a character is a line feed or carriage return,
as used by `isEol()`.

I liked Dart's implementation since it allows looking up the table in constant
time while saving extra instructions in the loop, so I'm doing the same :)
*/
inline constexpr std::array<bool, 256> ASCII_TABLE{
    // TODO: Move to utils
    // clang-format off
    false, false, false, false, false, false, false, false, //   0 - 7
    false, false, true,  false, false, true,  false, false, //   8 - 15 | 10 = $LF, 13 = $CR
    false, false, false, false, false, false, false, false, //  16 - 23
    false, false, false, false, false, false, false, false, //  24 - 31
    false, false, false, false, false, false, false, false, //  32 - 39
    false, false, false, false, false, false, false, false, //  40 - 47
    false, false, false, false, false, false, false, false, //  48 - 55
    false, false, false, false, false, false, false, false, //  56 - 63
    false, false, false, false, false, false, false, false, //  64 - 71
    false, false, false, false, false, false, false, false, //  72 - 79
    false, false, false, false, false, false, false, false, //  80 - 87
    false, false, false, false, false, false, false, false, //  88 - 95
    false, false, false, false, false, false, false, false, //  96 - 103
    false, false, false, false, false, false, false, false, // 104 - 111
    false, false, false, false, false, false, false, false, // 112 - 119
    false, false, false, false, false, false, false, false, // 120 - 127
    false, false, false, false, false, false, false, false, // 128 - 135
    false, false, false, false, false, false, false, false, // 136 - 143
    false, false, false, false, false, false, false, false, // 144 - 151
    false, false, false, false, false, false, false, false, // 152 - 159
    false, false, false, false, false, false, false, false, // 160 - 167
    false, false, false, false, false, false, false, false, // 168 - 175
    false, false, false, false, false, false, false, false, // 176 - 183
    false, false, false, false, false, false, false, false, // 184 - 191
    false, false, false, false, false, false, false, false, // 192 - 199
    false, false, false, false, false, false, false, false, // 200 - 207
    false, false, false, false, false, false, false, false, // 208 - 215
    false, false, false, false, false, false, false, false, // 216 - 223
    false, false, false, false, false, false, false, false, // 224 - 231
    false, false, false, false, false, false, false, false, // 232 - 239
    false, false, false, false, false, false, false, false, // 240 - 247
    false, false, false, false, false, false, false, false, // 248 - 255
    // clang-format on
};
} // namespace

// Utf8BytesScanner
std::size_t Utf8BytesScanner::getScanOffset() const {
    if (byteOffset == scanSlackOffset) {
        return byteOffset - scanSlack;
    } else {
        return byteOffset;
    }
}

Int Utf8BytesScanner::advance() {
    // Always increment to ensure it goes past the end.
    ++byteOffset;
    if (byteOffset > bytesLengthMinusOne) return $EOF;
    return bytes[byteOffset];
}

Int Utf8BytesScanner::advanceNoBoundsCheck() {
    ++byteOffset;
    return bytes[byteOffset];
}

Int Utf8BytesScanner::current() const {
    if (byteOffset > bytesLengthMinusOne) return $EOF;
    return bytes[byteOffset];
}

bool Utf8BytesScanner::scanUntilLineEnd() {
    /*
    Keeps track of the bits read from `next` on each call from the function
    `advanceNoBoundsCheck()`. If every character read is an ASCII one (less than
    128), then only the lower 7 bits will be set. Anything higher than 127 (non-
    ASCII) will automatically set the 8th bit.
    */
    dart::u8 nonAsciiCount = 0;

    /*
    Bit mask for only the 8th (most significant) bit. If the result of masking
    with `nonAsciiCount` has the 8th bit set, a non-ASCII character was found,
    otherwise only ASCII characters were encountered.
    */
    constexpr dart::u8 MASK{128};

    while ((byteOffset + 10) < bytesLengthMinusOne) {
        // 1
        /*
        Read the next byte without performing an EOF bounds check. This saves an
        extra instruction.
        */
        Int next = advanceNoBoundsCheck();

        /*
        Set all the bits in `next` into `nonAsciiCount`. If `next` consists of
        only ASCII characters (less than 128), then only the lower 7 bits will
        be set and the upper 8th bit will not be set.
        */
        nonAsciiCount |= next;

        /*
        If `next` is an end-of-line character ($LF or $CR), mask off all the
        lower 7 bits in `nonAsciiCount` except for the upper 8th bit. If the
        result is zero, then only ASCII characters were encountered (`true`)
        otherwise `false`.
        */
        if (ASCII_TABLE[next]) return (nonAsciiCount & MASK) == 0;

        // Do the same thing for the remaining 9 bytes

        // 2
        next = advanceNoBoundsCheck();
        nonAsciiCount |= next;
        if (ASCII_TABLE[next]) return (nonAsciiCount & MASK) == 0;

        // 3
        next = advanceNoBoundsCheck();
        nonAsciiCount |= next;
        if (ASCII_TABLE[next]) return (nonAsciiCount & MASK) == 0;

        // 4
        next = advanceNoBoundsCheck();
        nonAsciiCount |= next;
        if (ASCII_TABLE[next]) return (nonAsciiCount & MASK) == 0;

        // 5
        next = advanceNoBoundsCheck();
        nonAsciiCount |= next;
        if (ASCII_TABLE[next]) return (nonAsciiCount & MASK) == 0;

        // 6
        next = advanceNoBoundsCheck();
        nonAsciiCount |= next;
        if (ASCII_TABLE[next]) return (nonAsciiCount & MASK) == 0;

        // 7
        next = advanceNoBoundsCheck();
        nonAsciiCount |= next;
        if (ASCII_TABLE[next]) return (nonAsciiCount & MASK) == 0;

        // 8
        next = advanceNoBoundsCheck();
        nonAsciiCount |= next;
        if (ASCII_TABLE[next]) return (nonAsciiCount & MASK) == 0;

        // 9
        next = advanceNoBoundsCheck();
        nonAsciiCount |= next;
        if (ASCII_TABLE[next]) return (nonAsciiCount & MASK) == 0;

        // 10
        next = advanceNoBoundsCheck();
        nonAsciiCount |= next;
        if (ASCII_TABLE[next]) return (nonAsciiCount & MASK) == 0;
    }
    Int next = advance();

    // There's less than 10 bytes left, so run a normal loop
    while (true) {
        nonAsciiCount |= next;
        if (ASCII_TABLE[next] || next == $EOF) {
            return (nonAsciiCount & MASK) == 0;
        }
        next = advance();
    }
}

Int Utf8BytesScanner::skipSpaces() {
    /*
    Dart's implementation loop unrolls for the next 10 bytes (if available)
    before doing the same in a loop (if more bytes remain) to save up on extra
    instructions.
    */
    if (byteOffset + 10 < bytesLengthMinusOne) {
        Int next = advanceNoBoundsCheck();
        if (next == $SPACE && (next = advanceNoBoundsCheck()) == $SPACE &&
            (next = advanceNoBoundsCheck()) == $SPACE &&
            (next = advanceNoBoundsCheck()) == $SPACE &&
            (next = advanceNoBoundsCheck()) == $SPACE &&
            (next = advanceNoBoundsCheck()) == $SPACE &&
            (next = advanceNoBoundsCheck()) == $SPACE &&
            (next = advanceNoBoundsCheck()) == $SPACE &&
            (next = advanceNoBoundsCheck()) == $SPACE &&
            (next = advanceNoBoundsCheck()) == $SPACE) {
            // All 10 bytes were spaces, so use the loop to process more.
        } else {
            // One of the `advanceNoBoundsCheck()` isn't a space, so return.
            return next;
        }
    }

    // We still have more bytes, so process 10 at a time
    while (byteOffset + 10 < bytesLengthMinusOne) {
        Int next = advanceNoBoundsCheck();
        if (next == $SPACE && (next = advanceNoBoundsCheck()) == $SPACE &&
            (next = advanceNoBoundsCheck()) == $SPACE &&
            (next = advanceNoBoundsCheck()) == $SPACE &&
            (next = advanceNoBoundsCheck()) == $SPACE &&
            (next = advanceNoBoundsCheck()) == $SPACE &&
            (next = advanceNoBoundsCheck()) == $SPACE &&
            (next = advanceNoBoundsCheck()) == $SPACE &&
            (next = advanceNoBoundsCheck()) == $SPACE &&
            (next = advanceNoBoundsCheck()) == $SPACE) {
            continue; // Process the next 10 bytes (if any)
        } else {
            // One of the `advanceNoBoundsCheck()` isn't a space, so return.
            return next;
        }
    }

    // There's less than 10 bytes left, so run a normal loop.
    Int next = advance();
    while (next == $SPACE) {
        next = advance();
    }

    return next;
}

Int Utf8BytesScanner::peek() const {
    const Int next = byteOffset + 1;
    if (next > bytesLengthMinusOne) return $EOF;
    return bytes[next];
}

Int Utf8BytesScanner::currentAsUnicode(Int next) {
    // TODO: Add full implementation
    return next;
}

void Utf8BytesScanner::handleUnicode(std::size_t startScanOffset) {
    // TODO: Add full implementation
}

std::size_t Utf8BytesScanner::getStringOffset() const {
    // TODO: Add full implementation
    return getScanOffset();
}

StringToken* Utf8BytesScanner::createSubstringToken(
    const TokenType* type,
    std::size_t start,
    bool asciiOnly,
    std::size_t extraOffset,
    bool allowLazy
) {
    return new token::StringToken{
        type,
        bytes,
        start,
        byteOffset + extraOffset,
        tokenStart,
        asciiOnly,
        comments,
    };
}

StringToken* Utf8BytesScanner::createSyntheticSubstringToken(
    const TokenType* type,
    std::size_t start,
    bool asciiOnly,
    std::string_view syntheticChars
) {
    return new token::SyntheticStringToken{
        type,
        start,
        syntheticChars,
    };
}

CommentToken* Utf8BytesScanner::createCommentToken(
    const TokenType* type,
    std::size_t start,
    bool asciiOnly,
    std::size_t extraOffset
) {
    return new token::CommentToken{
        type,
        bytes,
        start,
        byteOffset + extraOffset,
        tokenStart,
        asciiOnly,
    };
}

DartDocToken* Utf8BytesScanner::createDartDocToken(
    const TokenType* type,
    std::size_t start,
    bool asciiOnly,
    std::size_t extraOffset
) {
    const auto end = byteOffset + extraOffset;
    return new token::DartDocToken{
        type,
        bytes,
        start,
        byteOffset + extraOffset,
        tokenStart,
        asciiOnly,
    };
}

LanguageVersionToken* Utf8BytesScanner::createLanguageVersionToken(
    std::size_t start, dart::u8 major, dart::u8 minor
) {
    return new token::LanguageVersionToken{
        bytes,
        start,
        byteOffset,
        tokenStart,
        major,
        minor,
    };
}

namespace {
bool (&isIdent)(Int) = internal_utils::isIdentifierCharAllowDollarTableLookup;
}

// TODO: Add docs
Int Utf8BytesScanner::passIdentifierCharAllowDollar()  {
    auto localByteOffset = byteOffset;
    while (localByteOffset + 10 < bytesLengthMinusOne) {
        Int next = bytes[++localByteOffset];
        if (isIdent(next) && //
            isIdent(next = bytes[++localByteOffset]) &&
            isIdent(next = bytes[++localByteOffset]) &&
            isIdent(next = bytes[++localByteOffset]) &&
            isIdent(next = bytes[++localByteOffset]) &&
            isIdent(next = bytes[++localByteOffset]) &&
            isIdent(next = bytes[++localByteOffset]) &&
            isIdent(next = bytes[++localByteOffset]) &&
            isIdent(next = bytes[++localByteOffset]) &&
            isIdent(next = bytes[++localByteOffset])) {
            continue;
        }

        byteOffset = localByteOffset;
        return next;
    }

    while (true) {
        Int next = advance();
        if (next == $EOF || !isIdent(next)) {
            return next;
        }
    }
}

} // namespace scanner
} // namespace front_end
} // namespace dart