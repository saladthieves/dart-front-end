#pragma once

#include "simple_token.hpp"
#include "begin_token.hpp"
#include "keyword_token.hpp"
#include "string_token.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
/*
The base class for synthetic tokens.

A synthetic token is one that's generated / inserted into the stream (such as
by a parser to recover from errors), and not one that originates from the source
code.
*/
class SyntheticToken : public SimpleToken {
public:
    explicit SyntheticToken(const type::TokenType* type, std::size_t offset)
        : SimpleToken(type, offset) {}

    virtual const Token* getBeforeSynthetic() const override = 0;

    virtual bool isSynthetic() const override { return true; }

    virtual std::size_t getLength() const override { return 0; }
};

/*
A synthetic token used to replace another token in the stream.

The token being replaced is kept inside the `replacedToken` field. The offset
and precedingComment values of the replacement token are set from the replaced
one.
*/
class ReplacementToken : public SyntheticToken {
public:
    explicit ReplacementToken(const type::TokenType* type, const Token* replacedToken)
        : SyntheticToken(type, replacedToken->getOffset()),
          replacedToken{replacedToken} {
        setPrecedingComments(replacedToken->getPrecedingComments());
    }

    virtual const Token* getBeforeSynthetic() const override {
        return beforeSynthetic;
    }

    virtual void setBeforeSynthetic(const Token* previous) override {
        beforeSynthetic = previous;
    }

    /*
    The token that `this` token replaces.
    This will normally correspond to what the user originally typed in.
    */
    const Token* replacedToken;

private:
    const Token* beforeSynthetic{nullptr};
};

/*
A synthetic version of a begin token.
*/
class SyntheticBeginToken : public BeginToken {
public:
    explicit SyntheticBeginToken(
        const type::TokenType* type,
        std::size_t offset,
        CommentToken* precedingComment = nullptr
    )
        : BeginToken(type, offset, precedingComment) {}

    virtual const Token* getBeforeSynthetic() const override {
        return beforeSynthetic;
    }

    virtual void setBeforeSynthetic(const Token* previous) override {
        beforeSynthetic = previous;
    }

    virtual bool isSynthetic() const override { return true; }

    virtual std::size_t getLength() const override { return 0; }

private:
    const Token* beforeSynthetic{nullptr};
};

/*
A synthetic version of a keyword token.
*/
class SyntheticKeywordToken : public KeywordToken {
public:
    explicit SyntheticKeywordToken(const type::TokenType* keyword, std::size_t offset)
        : KeywordToken(keyword, offset) {}

    virtual const Token* getBeforeSynthetic() const override {
        return beforeSynthetic;
    }

    virtual void setBeforeSynthetic(const Token* previous) override {
        beforeSynthetic = previous;
    }

    virtual std::size_t getLength() const override { return 0; }

private:
    const Token* beforeSynthetic{nullptr};
};

/*
A synthetic version of a string token.
*/
class SyntheticStringToken : public StringToken {
public:
    explicit SyntheticStringToken(
        const type::TokenType* type,
        std::string_view value,
        std::size_t offset,
        bool useLength = false,
        std::size_t length = 0
    )
        : StringToken(type, value, offset),
          useLength{useLength},
          length{length} {}

    virtual const Token* getBeforeSynthetic() const override {
        return beforeSynthetic;
    }

    virtual void setBeforeSynthetic(const Token* previous) override {
        beforeSynthetic = previous;
    }

    virtual bool isSynthetic() const override { return true; }

    virtual std::size_t getLength() const override {
        return useLength ? length : StringToken::getLength();
    }

private:
    const Token* beforeSynthetic{nullptr};

    bool useLength{false};
    std::size_t length{0};
};
}
} // namespace scanner
} // namespace front_end
} // namespace dart