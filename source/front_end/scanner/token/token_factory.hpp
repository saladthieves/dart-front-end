#pragma once

#include "comment_token.hpp"
#include "token/simple_token.hpp"
#include "token/token_type.hpp"

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
    eof(std::size_t offset, CommentToken* precedingComments = nullptr) {
        Token* eof =
            new SimpleToken{&type::END_OF_FILE, offset, precedingComments};

        // And EOF points to itself, so there's always infinite prev/next.
        eof->setPrevious(eof);
        eof->setNext(eof);

        return eof;
    }

    static SimpleToken* simple(
        const type::TokenType* type,
        std::size_t offset,
        CommentToken* precedingComments = nullptr
    ) {
        return new SimpleToken(type, offset, precedingComments);
    }
};

} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart