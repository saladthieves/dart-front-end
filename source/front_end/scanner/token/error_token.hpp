#pragma once

#include "begin_token.hpp"
#include "characters.hpp"
#include "simple_token.hpp"
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

Accessing `ErrorToken::getLexeme()` instead of using the derived classes is
considered to be an error.
*/
class ErrorToken : public SimpleToken {
public:
    explicit ErrorToken(std::size_t offset)
        : SimpleToken(&type::BAD_INPUT, offset, nullptr) { }

    /*
    Creates the `Message` object that represents the error information regarding
    this error token.

    This should be overridden by subclasses and called from their constructor
    body.
    */
    virtual void buildAssertionMessage() = 0;

    std::string_view getLexeme() const override {
        return getAssertionMessage()->problemMessage;
    }

    std::size_t getLength() const override { return 1; }

    const Message* getAssertionMessage() const { return message.get(); };

    const Code* getErrorCode() const { return &(getAssertionMessage()->code); }

    virtual dart::u8 getCharacter() const { return chars::$EOF; };

    virtual std::string_view getStart() const { return ""; };

    virtual std::size_t getEndOffset() const { return 0; };

    virtual const BeginToken* getBeginToken() const { return nullptr; }

protected:
    /*
    The assertion message for this error token.

    The `Message` objects used for initialization typically come from the
    `diagnostic_codes.hpp` header file. An overridden call to the
    `buildAssertionMessage` function in derived classes initializes this object.
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
    explicit AsciiControlCharacterToken(dart::u8 character, std::size_t offset)
        : ErrorToken(offset),
          character{character} {
        buildAssertionMessage();
    }

    virtual void buildAssertionMessage() override {
        message = diag::asciiControlCharacter(character);
    }

    virtual dart::u8 getCharacter() const override { return character; }

    // TODO: Add std::formatter<AsciiControlCharacterToken> specialization

private:
    /*
    The ASCII control character.
    */
    dart::u8 character;
};

/*
Represents an encoding error.

This could take place if a character (such as the unicode replacement character
U+FFFD �) is encountered.
*/
class EncodingErrorToken : public ErrorToken {
public:
    explicit EncodingErrorToken(std::size_t offset): ErrorToken(offset) {
        buildAssertionMessage();
    }

    virtual void buildAssertionMessage() override {
        message = diag::encoding();
    }

    // TODO: add std::formatter<EncodingErrorToken> specialization
};

/*
Represents a non-ASCII whitespace character appearing in the code but not inside
a comment or a string.
*/
class NonAsciiWhitespaceToken : public ErrorToken {
public:
    explicit NonAsciiWhitespaceToken(dart::u16 character, std::size_t offset)
        : ErrorToken(offset),
          character{character} {
        buildAssertionMessage();
    }

    virtual void buildAssertionMessage() override {
        message = diag::nonAsciiWhitespace(character);
    }

    // TODO: Add std::formatter<NonAsciiWhitespaceToken> specialization

private:
    /*
    The non-ASCII whitespace character.
    */
    dart::u16 character;
};

/*
Represents a non-ASCII character appearing in the code but not inside a comment
or a string. This excludes non-ASCII whitespace characters, as those are
represented by the `NonAsciiWhitespace` class.
*/
class NonAsciiIdentifierToken : public ErrorToken {
public:
    explicit NonAsciiIdentifierToken(dart::u16 character, std::size_t offset)
        : ErrorToken(offset),
          character{character} {
        buildAssertionMessage();
    }

    virtual void buildAssertionMessage() override {
        message = diag::nonAsciiIdentifier(character);
    }

    // TODO: Add std::formatter<NonAsciiIdentifierToken> specialization

private:
    /*
    The non-ASCII character.
    */
    dart::u16 character;
};

// TODO: Add docs
class UnterminatedString : public ErrorToken {
public:
    explicit UnterminatedString(
        std::string_view start, std::size_t charOffset, std::size_t endOffset
    )
        : ErrorToken(charOffset),
          start{start},
          endOffset{endOffset} {
        buildAssertionMessage();
    }

    virtual void buildAssertionMessage() override {
        message = diag::unterminatedString(start, closeQuoteFor(start));
    }

    virtual std::string_view getStart() const override { return start; }

    virtual std::size_t getEndOffset() const override { return endOffset; }

    virtual std::size_t getCharCount() const override {
        return endOffset - getCharOffset();
    }

    virtual std::size_t getLength() const override { return getCharCount(); }

    // TODO: Add std::formatter<UnterminatedString> specialization

private:
    std::string start;
    std::size_t endOffset;
};

// TODO: Add docs
class UnmatchedToken : public ErrorToken {
public:
    explicit UnmatchedToken(const BeginToken* begin)
        : ErrorToken(begin->getCharOffset()),
          begin{begin} {
        buildAssertionMessage();
    }

    virtual void buildAssertionMessage() override {
        message = diag::unmatchedToken(
            closeBraceFor(begin->getLexeme()), begin->getLexeme()
        );
    }

    const BeginToken* getBeginToken() const override { return begin; }

    // TODO: Add std::formatter<UnmatchedToken> specialization

private:
    const BeginToken* begin;
};

// TODO: Add docs
class UnterminatedToken : public ErrorToken {
public:
    explicit UnterminatedToken(
        std::unique_ptr<Message> assertionMessage,
        std::size_t charOffset,
        std::size_t endOffset
    )
        : ErrorToken(charOffset),
          endOffset{endOffset} {
        message = std::move(assertionMessage);
    }

    virtual void buildAssertionMessage() override {
        // Do nothing
    }

    virtual std::size_t getEndOffset() const override { return endOffset; }

    virtual std::size_t getCharCount() const override {
        return endOffset - getCharOffset();
    }

    // TODO: Add std::formatter<UnterminatedToken> specialization

private:
    std::size_t endOffset;
};

// TODO: Add docs
ErrorToken*
buildUnexpectedCharacterToken(dart::u16 character, std::size_t offset);
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart