#pragma once

#include "simple_token.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
/*
A specific `SimpleToken` implementation representing a keyword token.
*/
class KeywordToken : public SimpleToken {
public:
    explicit KeywordToken(
        const type::TokenType* keyword,
        std::size_t offset,
        CommentToken* precedingComment = nullptr
    )
        : SimpleToken(keyword, offset, precedingComment) {}

    virtual const keyword::Keyword* getKeyword() const override {
        return static_cast<const keyword::Keyword*>(getType());
    }

    virtual bool isIdentifier() const override {
        return getKeyword()->isPseudo() || getKeyword()->isBuiltIn();
    }

    virtual bool isKeyword() const override { return true; }

    virtual bool isKeywordOrIdentifier() const override { return true; }

    // TODO: Implement Object value();
};
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart