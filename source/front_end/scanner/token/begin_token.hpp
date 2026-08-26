#pragma once

#include "token.hpp"

#include "common/errors.hpp"
#include "token_types.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {

/*
A token that represents the opening half of a grouping pair of tokens.

This is used for the opening parentheses '(', curly bracket '{', and square
bracket '[' tokens. The corresponding closing token is found in the `endToken`
field.
*/
class BeginToken : public Token {
public:
    explicit BeginToken(
        const type::TokenType* type,
        std::size_t beginOffset,
        std::size_t length,
        CommentToken* precedingComment = nullptr
    )
        : Token{type, beginOffset, length, precedingComment} {
        // TODO: Add assertion error after support for std::formatter<TokenType>
        assert::assert(
            type == &type::LT ||                 //
            type == &type::OPEN_CURLY_BRACKET || //
            type == &type::OPEN_PAREN ||         //
            type == &type::OPEN_SQUARE_BRACKET ||
            type == &type::STRING_INTERPOLATION_EXPRESSION
        );

        init();
    }
};
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart