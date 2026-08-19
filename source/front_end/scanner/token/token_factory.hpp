#pragma once

#include "comment_token.hpp"

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
};

} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart