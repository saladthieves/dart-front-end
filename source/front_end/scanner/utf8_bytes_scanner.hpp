#pragma once

#include "abstract_scanner.hpp"

#include "common/types.hpp"
#include "scanner.hpp"
#include "token/characters.hpp"

#include <span>

namespace dart {
namespace front_end {
namespace scanner {
// TODO: Inline some of the functions

/*
A scanner that reads a UTF-8 encoded list of bytes and creates a list of `Token`
types pointing to the substrings.

The scanner also handles ASCII and Unicode characters when they're encountered.
*/
class Utf8BytesScanner : public AbstractScanner {
public:
    explicit Utf8BytesScanner(
        std::span<dart::u8> bytes,
        const ScannerConfiguration* config = nullptr,
        bool includeComments = false,
        const LanguageVersionChanged* languageVersionChanged = nullptr,
        bool allowLazyStrings = true
    )
        : AbstractScanner(
              config,
              includeComments,
              languageVersionChanged,
              bytes.size_bytes(),
              allowLazyStrings
          ),
          bytes{bytes},
          bytesLengthMinusOne{bytes.size_bytes() - 1} {
        if (token::chars::containsBOMAt(0, this->bytes)) {
            byteOffset += 3;
            utf8Slack += 3;
        }
    }

    /*
    Returns the byte offset of the first byte that belongs to the current
    character.
    */
    virtual std::size_t getScanOffset() const override;

    virtual Int advance() override;

    /*
    Increments the `byteOffset` value by 1 and returns the character it then
    points to.

    Unlike `advance()`, this does not perform a check to ensure `byteOffset` is
    within bounds.
    */
    Int advanceNoBoundsCheck();

    virtual Int current() const override;

    // TODO: Implement AbstractScanner::passIdentifierCharAllowDollar()

    virtual bool scanUntilLineEnd() override;

    virtual Int skipSpaces() override;

    virtual Int peek() const override;

    // TODO: Implement nextCodePoint() for UTF-16

    virtual Int currentAsUnicode(Int next) override;

    virtual void handleUnicode(std::size_t startScanOffset) override;

    virtual std::size_t getStringOffset() const override;

    virtual token::StringToken* createSubstringToken(
        const token::type::TokenType* type,
        std::size_t start,
        bool asciiOnly,
        std::size_t extraOffset,
        bool allowLazy
    ) override;

    virtual token::StringToken* createSyntheticSubstringToken(
        const token::type::TokenType* type,
        std::size_t start,
        bool asciiOnly,
        std::string_view syntheticChars
    ) override;

    virtual token::CommentToken* createCommentToken(
        const token::type::TokenType* type,
        std::size_t start,
        bool asciiOnly,
        std::size_t extraOffset = 0
    ) override;

    virtual token::DartDocToken* createDartDocToken(
        const token::type::TokenType* type,
        std::size_t start,
        bool asciiOnly,
        std::size_t extraOffset = 0
    ) override;

    virtual token::LanguageVersionToken* createLanguageVersionToken(
        std::size_t start, dart::u8 major, dart::u8 minor
    ) override;

    virtual bool atEndOfFile() const override {
        return byteOffset > bytesLengthMinusOne;
    }

    /*
    The current offset of the last byte returned by `advance()`.

    After invoking `currentAsUnicode()`, the `byteOffset` value points to the
    last byte that's part of the unicode / ASCII character. This is to ensure
    that calling `advance()` always progresses forward the byte offset by 1 to
    the next character.
    */
    std::size_t byteOffset{0};

    /*
    The `getScanOffset()` getter is expected to return the offset of where the
    current character begins, denoted by `byteOffset`. After invoking
    `currentAsUnicode()`, the `byteOffset` value points to the last byte in a
    multi-byte character (such as Unicode).

    To remedy this, `scanSlack` keeps track of the number of bytes in the
    current character. In the example below, byte 3 represents one character,
    byte 4 represents another character. Bytes 5, 6 and 7 represent a single
    Unicode character. `byteOffset` is currently positioned on byte 4:

                 ... [ 3 ][ 4 ][ 5 ; 6 ; 7 ] ...
               byteOffset --^

    Calling `advance()` moves `byteOffset` forward by 1, to byte 5, the starting
    byte in the multi-byte Unicode character:

                 ... [ 3 ][ 4 ][ 5 ; 6 ; 7 ] ...
                    byteOffset --^

    Calling `currentAsUnicode()` moves `byteOffset` to byte 7, the last byte in
    the Unicode character and sets `scanSlack` to 2:

                 ... [ 3 ][ 4 ][ 5 ; 6 ; 7 ] ...
                            byteOffset --^

    Calling `getScanOffset()` then returns 5, instead of 7.
    */
    std::size_t scanSlack{0};

    /*
    Holds the `byteOffset` value for which the current `scanSlack` is valid.
    */
    // TODO: Experiment with this a bit more
    std::size_t scanSlackOffset{0};

    /*
    The difference between the number of bytes and the number of corresponding
    string characters, up to the current `byteOffset`.
    */
    // TODO: Add clearer docs once we know what this does
    std::size_t utf8Slack{0};

    // TODO: Add docs once we know what this is
    std::size_t stringOffsetSlackOffset{0};

private:
    /*
    The raw bytes of the scanned file.
    */
    std::span<dart::u8> bytes;

    /*
    The total length of the scanned file (in bytes) minus one.
    */
    std::size_t bytesLengthMinusOne;
};

} // namespace scanner
} // namespace front_end
} // namespace dart