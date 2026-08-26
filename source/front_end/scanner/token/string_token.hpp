#pragma once

#include "token.hpp"

#include "common/types.hpp"
#include "token/token_type.hpp"

#include <span>

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
/*
A token whose value is independent of its type.

An example of such a token is a comment token.
*/
// TODO: Add support for lazy strings
class StringToken : public Token {
public:
    explicit StringToken(
        const type::TokenType* type,
        std::size_t beginOffset,
        std::string_view stringValue,
        std::size_t length,
        CommentToken* precedingComment = nullptr
    )
        : Token{type, beginOffset, length, precedingComment} {
        lexeme = stringValue;
        
        init();
    }

    explicit StringToken(
        const type::TokenType* type,
        std::span<dart::u8> bytes,
        std::size_t start,
        std::size_t stop,
        std::size_t beginOffset,
        bool asciiOnly,
        CommentToken* precedingComment = nullptr
    )
        : StringToken(
              type,
              beginOffset,
              std::string{bytes.begin() + start, bytes.begin() + stop},
              stop - start,
              precedingComment
          ) { }
};
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart