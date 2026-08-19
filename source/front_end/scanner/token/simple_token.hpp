#pragma once

#include "keywords.hpp"
#include "token.hpp"
#include "token_type.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
/*
An implementation of the `Token` interface to represent a single simple token
that was scanned from the input. Each token is in a stream, aware of the next
and previous tokens in that same stream.

This acts as the root class for both synthetic and non-synthetic (normal)
tokens.

TODO: Check if class can be optimized:
    - If some member accesses (those using getType()) can be constexpr.
    - If constructor can be constexpr.
*/
class SimpleToken : public Token {
    /*
    The value used to mask the lower 8 bits for the token type index.
    */
    static constexpr dart::u8 TYPE_MASK{0xFF};

    /*
    The shift value for the offset in the `typeAndOffset` member.
    */
    static constexpr dart::u8 OFFSET_MASK{8};

public:
    explicit SimpleToken(
        const type::TokenType* type,
        std::size_t offset,
        CommentToken* precedingComment = nullptr
    )
        : typeAndOffset{(offset << OFFSET_MASK) | type->index},
          precedingComment{precedingComment} {
        setCommentParent(precedingComment);
    }

    /*
    Returns the `TokenType` (from the `types::ALL_TYPES` array) that corresponds
    to this token.

    The index used to retrieve it is obtained by masking off the lower 8-bits of
    the `typeAndOffset` value.
    */
    virtual const type::TokenType* getType() const override {
        return type::ALL_TOKENTYPES[typeAndOffset & TYPE_MASK];
    }

    /*
    Returns the index value found in the lower 8 bits of the `typeAndOffset`
    value.
    */
    virtual dart::u8 getTypeIndex() const override {
        return typeAndOffset & TYPE_MASK;
    }

    /*
    Returns the offset value, found in the upper 56 bits of the `typeAndOffset`
    value, by shifting them all to the right first.
    */
    virtual std::size_t getOffset() const override {
        return typeAndOffset >> OFFSET_MASK;
    }

    /*
    Sets the provided offset value by storing it in the upper 56 bits of the
    `typeAndOffset` value, by shifting them to the left first.
    */
    virtual void setOffset(std::size_t value) override {
        typeAndOffset = (value << OFFSET_MASK) | (typeAndOffset & TYPE_MASK);
    }

    virtual Token* getPrevious() const override { return previous; }

    virtual Token* getNext() const override { return next; }

    virtual void setNext(Token* next) override { this->next = next; }

    virtual void setPrevious(Token* previous) override {
        this->previous = previous;
    }

    virtual std::size_t getCharCount() const override { return getLength(); }

    virtual std::size_t getCharOffset() const override { return getOffset(); }

    virtual std::size_t getCharEnd() const override { return getEnd(); }

    virtual const Token* getBeforeSynthetic() const override { return nullptr; }

    virtual void setBeforeSynthetic(const Token* token) override {}

    virtual std::size_t getEnd() const override {
        return getOffset() + getLength();
    }

    virtual const Token* getEndGroup() const override { return nullptr; }

    virtual bool isEof() const override {
        return getType() == &type::END_OF_FILE;
    }

    virtual bool isIdentifier() const override { return false; }

    virtual bool isKeyword() const override { return false; }

    virtual bool isKeywordOrIdentifier() const override {
        return isIdentifier();
    }

    virtual bool isModifier() const override { return getType()->isModifier; }

    virtual bool isOperator() const override { return getType()->isOperator; }

    virtual bool isSynthetic() const override { return getLength() == 0; }

    virtual bool isTopLevelKeyword() const override {
        return getType()->isTopLevelKeyword;
    }

    virtual bool isUserDefinableOperator() const override {
        return getType()->isUserDefinableOperator;
    }

    virtual const keyword::Keyword* getKeyword() const override {
        return nullptr;
    }

    virtual dart::u8 getKind() const override { return getType()->kind; }

    virtual std::size_t getLength() const override {
        return getLexeme().size();
    }

    virtual std::string_view getLexeme() const override {
        return getType()->lexeme;
    }

    virtual CommentToken* getPrecedingComments() const override {
        return precedingComment;
    }

    /*
    Sets the `precedingComment` to the provided parameter. Once set, the
    `precedingComment` (and any comments after) all have their `parent` field
    set to `this`.
    */
    void setPrecedingComments(CommentToken* comment) {
        precedingComment = comment;
        setCommentParent(precedingComment);
    }

    virtual const char* getStringValue() const override {
        return getType()->stringValue;
    }

    virtual bool matchesAny(
        std::initializer_list<const type::TokenType*> types
    ) const override {
        const auto* t = getType();
        for (const auto* type : types) {
            if (type == t) return true;
        }
        return false;
    }

    virtual Token* setNextToken(Token* token) override {
        setNext(token);
        token->setPrevious(this);
        token->setBeforeSynthetic(this);
        return token;
    }

    virtual Token* setNextWithoutSettingPrevious(Token* token) override {
        setNext(token);
        return token;
    }

    // TODO: Implement std::formatter<SimpleToken>

    // TODO: Implement Object value()

private:
    /*
    Sets the provided comment's `parent` field to `this` and also any other
    subsequent comment tokens after.
    */
    void setCommentParent(CommentToken* comment); // TODO: Implement from source

    /*
    The previous token in the stream.
    */
    Token* previous;

    /*
    The next token in the stream.
    */
    Token* next;

    /*
    The first comment token in a list of comment tokens preceding this token, or
    `nullptr` if none exists.
    */
    CommentToken* precedingComment;

    /*
    The combined encoding of the token type index (from `getTypeIndex()`) and
    token offset into a single [typically] 64-bit value.

    The type index occupies the lower (least significant) 8 bits since it's
    always less than 256. The offset occupies the upper (most significant) 56
    bits since the offset from the code start can be much larger.
    */
    std::size_t typeAndOffset{0};
};
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart