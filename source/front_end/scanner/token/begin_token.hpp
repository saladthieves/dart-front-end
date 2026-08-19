#pragma once

#include "simple_token.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
/*
A `SimpleToken` that represents the opening half of a grouping pair of tokens.
This is used for the opening parentheses '(', curly bracket '{', and square
bracket '[' tokens.
*/
class BeginToken : public SimpleToken {
public:
    explicit BeginToken(
        const type::TokenType* type,
        std::size_t offset,
        CommentToken* precedingComment = nullptr
    )
        : SimpleToken(type, offset, precedingComment) {
        assert::assert(
            type == &type::LT ||                 //
            type == &type::OPEN_CURLY_BRACKET || //
            type == &type::OPEN_PAREN ||         //
            type == &type::OPEN_SQUARE_BRACKET ||
            type == &type::STRING_INTERPOLATION_EXPRESSION
        );
    }

    virtual const Token* getEndGroup() const override { return endToken; }

    /*
    Sets the token that corresponds to this token's other half.
    */
    void setEndGroup(const Token* token) { endToken = token; }

    /*
    The other half token that corresponds to this token.
    */
    const Token* endToken{nullptr};
};
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart