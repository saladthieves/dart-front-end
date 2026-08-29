#pragma once

#include "begin_token.hpp"
#include "characters.hpp"
#include "token/recovery.hpp"
#include "token_types.hpp"

#include "messages/diagnostic_codes.hpp"

#include <memory>
#include <string_view>

namespace dart {
namespace front_end {
namespace scanner {
namespace token {

using namespace messages::codes;

/*
Base class for all error tokens produced by the scanner during scanning.

Error tokens wrap around error messages, so their length is `1` instead of the
length of their error message.
*/
class ErrorToken : public Token {
public:
    explicit ErrorToken(std::size_t beginOffset)
        : Token{&type::BAD_INPUT, beginOffset, 1} { }

    /*
    Returns the error character.

    For error types (such as `AsciiControlCharacterToken`), this would return
    the erroneous character.
    */
    virtual dart::u8 character() const { return chars::$EOF; };

    /*
    Returns the begin token.

    For types (such as grouped tokens), this returns the corresponding beginning
    token.
    */
    virtual const BeginToken* getBeginToken() const { return nullptr; }

    /*
    The assertion message for this error token.

    The `Message` objects used for initialization typically come from the
    `front_end/messages/diagnostic_codes.hpp` header file.
    */
    std::unique_ptr<Message> message{nullptr};
};

/*
Represents an ASCII control character that's being used outside a comment or
token.

These types of control characters have a value less than 0x1F (31).
*/
class AsciiControlCharacterToken : public ErrorToken {
public:
    explicit AsciiControlCharacterToken(
        dart::u8 character, std::size_t beginOffset
    )
        : ErrorToken{beginOffset},
          _character{character} {
        message = diag::asciiControlCharacter(_character);
        lexeme = message->problemMessage;

        init();
    }

    virtual dart::u8 character() const override { return _character; }

private:
    /*
    The ASCII control character.
    */
    dart::u8 _character;
};

/*
Represents an encoding error.

This could take place if a character (such as the unicode replacement character
U+FFFD �) is encountered.
*/
class EncodingErrorToken : public ErrorToken {
public:
    explicit EncodingErrorToken(std::size_t beginOffset)
        : ErrorToken{beginOffset} {
        message = diag::encoding();
        lexeme = message->problemMessage;

        init();
    }
};

/*
Represents a non-ASCII whitespace character appearing in the code but not inside
a comment or a string.
*/
class NonAsciiWhitespaceToken : public ErrorToken {
public:
    explicit NonAsciiWhitespaceToken(
        dart::u16 character, std::size_t beginOffset
    )
        : ErrorToken{beginOffset},
          _character{character} {
        message = diag::nonAsciiWhitespace(character);
        lexeme = message->problemMessage;

        init();
    }

    virtual dart::u8 character() const override { return _character; }

private:
    /*
    The non-ASCII whitespace character.
    */
    dart::u16 _character;
};

/*
Represents a non-ASCII character appearing in the code but not inside a comment
or a string. This excludes non-ASCII whitespace characters, as those are
represented by the `NonAsciiWhitespace` class.
*/
class NonAsciiIdentifierToken : public ErrorToken {
public:
    explicit NonAsciiIdentifierToken(
        dart::u16 character, std::size_t beginOffset
    )
        : ErrorToken{beginOffset},
          _character{character} {
        message = diag::nonAsciiIdentifier(character);
        lexeme = message->problemMessage;

        init();
    }

    virtual dart::u8 character() const override { return _character; }

private:
    /*
    The non-ASCII character.
    */
    dart::u16 _character;
};

// TODO: Add docs
class UnterminatedString : public ErrorToken {
public:
    explicit UnterminatedString(std::size_t beginOffset, std::string_view start)
        : ErrorToken{beginOffset} {
        message = diag::unterminatedString(start, closeQuoteFor(start));
        lexeme = message->problemMessage;

        init();
    }
};

// TODO: Add docs
class UnmatchedToken : public ErrorToken {
public:
    explicit UnmatchedToken(const BeginToken* begin)
        : ErrorToken(begin->beginOffset),
          begin{begin} {
        message = diag::unmatchedToken(
            closeBraceFor(begin->lexeme), //
            begin->lexeme
        );
        lexeme = message->problemMessage;

        init();
    }

    const BeginToken* begin;
};

// TODO: Add docs
class UnterminatedToken : public ErrorToken {
public:
    explicit UnterminatedToken(
        std::unique_ptr<Message> assertionMessage,
        std::size_t beginOffset,
        std::size_t endOffset
    )
        : ErrorToken(beginOffset) {
        length = endOffset - beginOffset;
        message = std::move(assertionMessage);
        lexeme = message->problemMessage;
        init();
    }
};

// TODO: Add docs
ErrorToken*
buildUnexpectedCharacterToken(dart::u16 character, std::size_t offset);
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart