#pragma once

#include "codes.hpp"
#include "conversions.hpp"
#include "diagnostic.hpp"
#include "severity.hpp"

#include <format>
#include <memory>
#include <string>
#include <string_view>

namespace dart {
namespace front_end {
namespace messages {
namespace codes {
namespace diag {
// clang-format off
/*
Returns a `Message` indicating that the provided ASCII `character` was used in
the source code but not inside a comment or string.
*/
inline std::unique_ptr<Message> asciiControlCharacter(dart::u8 character) {
    constexpr auto code = Code{
        "AsciiControlCharacter", 
        PseudoSharedCode::ILLEGAL_CHARACTER,
        severity::CfeSeverity::ERROR
    };
    const auto problemMessage = std::format(
        "The control character `{}` can only be used in strings and comments.",
        character
    );
    const auto arguments = Message::Args{
        {"character", format::charToUnicode(character)}
    };

    return std::make_unique<Message>(code, problemMessage, arguments);
}

/*
Returns a `Message` indicating a problem in the encoding of the text.

This could happen if a character (such as the unicode replacement character
U+FFFD �) is encountered.
*/
inline std::unique_ptr<Message> encoding() {
    constexpr auto code = Code {
        "Encoding",
        PseudoSharedCode::ENCODING
    };
    constexpr auto problemMessage = "Unable to decode bytes as UTF-8.";
    
    return std::make_unique<Message>(code, problemMessage);
}

/*
Returns a `Message` indicating that a non-ASCII character (other than a space)
appears in an identifier instead of in a string or comment.
*/
inline std::unique_ptr<Message> nonAsciiIdentifier(dart::u16 character) {
    constexpr auto code = Code {
        "NonAsciiIdentifier",
        PseudoSharedCode::ILLEGAL_CHARACTER
    };
    const auto unicode = format::charToUnicode(character);
    const auto problemMessage = std::format(
        "The non-ASCII character `{}` ({}) can't be used in identifiers, only in strings and comments",
        character, unicode
    );
    const auto arguments = Message::Args {
        {"character", std::to_string(character)},
        {"unicode", unicode}
    };
    constexpr auto correctionMessage = "Try using an US-ASCII letter, digit, '_' (underscore) or '$' (dollar sign).";

    return std::make_unique<Message>(
        code, 
        problemMessage, 
        arguments, 
        correctionMessage
    );
}

/*
Returns a `Message` indicating that a non-ASCII whitespace character appears in
the code but not in a string or comment.
*/
inline std::unique_ptr<Message> nonAsciiWhitespace(dart::u16 character) {
    constexpr auto code = Code {
        "NonAsciiWhitespace",
        PseudoSharedCode::ILLEGAL_CHARACTER
    };
    const auto problemMessage = std::format(
        "The non-ASCII space character `{}` can only be used in strings and comments.", 
        character
    );
    const auto arguments = Message::Args {
        {"character", format::charToUnicode(character)}
    };
    
    return std::make_unique<Message>(code, problemMessage, arguments);
}

/*
Returns a `Message` indicating an unterminated string with the given `openQuote`
value and missing `expectedCloseQuote`.
*/
inline std::unique_ptr<Message> unterminatedString(
    std::string_view openQuote,
    std::string_view expectedCloseQuote
) {
    constexpr auto code = Code {
        "UnterminatedString",
        PseudoSharedCode::UNTERMINATED_STRING_LITERAL
    };
    const auto problemMessage = std::format(
        "String starting with `{}` must end with `{}`.", 
        openQuote, expectedCloseQuote
    );
    const auto arguments = Message::Args {
        {"openQuote", std::string{openQuote}},
        {"expectedCloseQuote", std::string{expectedCloseQuote}}
    };

    return std::make_unique<Message>(code, problemMessage, arguments);
}

// TODO: Add docs
inline std::unique_ptr<Message> unmatchedToken(
    std::string_view expected,
    std::string_view lexeme
) {
    constexpr auto code = Code {
        "UnmatchedToken",
        PseudoSharedCode::EXPECTED_TOKEN
    };
    const auto problemMessage = std::format(
        "Can't find `{}` to match `{}`.", expected, lexeme
    );
    const auto arguments = Message::Args {
        {"expected", std::string{expected}},
        {"lexeme", std::string{lexeme}}
    };

    return std::make_unique<Message>(code, problemMessage, arguments);
}

// TODO: Add docs
inline std::unique_ptr<Message> unexpectedDollarInString() { 
    constexpr auto code = Code {
        "UnexpectedDollarInString",
        PseudoSharedCode::UNEXPECTED_DOLLAR_IN_STRING
    };

    constexpr auto problemMessage = "A '$' has a special meaning inside a "
        "string, and must be followed by an identifier or an expression in "
        "curly braces ({}).";

    constexpr auto correctionMessage = 
        "Try adding a backslash (\\) to escape the '$'.";

    const auto arguments = Message::Args {};
        return std::make_unique<Message>(code, problemMessage, arguments, 
            correctionMessage
        );
}

// TODO: Add docs
inline std::unique_ptr<Message> unterminatedComment() {
    constexpr auto code = Code {
        "UnterminatedComment",
        PseudoSharedCode::UNTERMINATED_MULTI_LINE_COMMENT,
    };
    
    constexpr auto problemMessage = "Comment starting with '/*' must end wit '*/'.";
    return std::make_unique<Message>(code, problemMessage);
}

// clang-format on
} // namespace diag
} // namespace codes
} // namespace messages
} // namespace front_end
} // namespace dart