#pragma once

#include "comment_token.hpp"
#include "token/token.hpp"

#include "token/token_type.hpp"
#include "token/token_types.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
class TokenFactory {
public:
    /*
    Creates and returns a new instance of an end-of-file token with the given
    offset.
    */
    static Token*
    eof(std::size_t beginOffset, CommentToken* precedingComment = nullptr) {
        /*
        The Dart implementation of this factory constructor has the previous and
        next point to the same instance. In this implementation, they point to
        `nullptr`.
        */
        auto* eof = new Token{
            &type::END_OF_FILE, beginOffset, type::END_OF_FILE.lexeme.length(),
            precedingComment
        };
        eof->init();
        
        return eof;
    }

    /*
    Creates and returns a simple token with the provided `type`, `beginOffset`,
    `length` and optional `precedingComment` values.
    */
    static Token* create(
        const type::TokenType* type,
        std::size_t beginOffset,
        std::size_t length,
        CommentToken* precedingComment = nullptr
    ) {
        return new Token{type, beginOffset, length, precedingComment};
    }
};

} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart