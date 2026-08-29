#pragma once

#include "comment_token.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
/*
A token implementation representing a keyword.
*/
class KeywordToken : public Token {
public:
    explicit KeywordToken(
        const type::TokenType* keyword,
        std::size_t beginOffset,
        CommentToken* precedingComment = nullptr
    )
        : Token{
              keyword,
              beginOffset,
              keyword->lexeme.length(),
              precedingComment,
          } {

        init();
    }
};
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart