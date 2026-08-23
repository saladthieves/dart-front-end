#pragma once

#include "string_token.hpp"

#include "token/comment_token.hpp"
#include "token/token_type.hpp"
#include "token/token_types.hpp"

#include <span>

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
// TODO: Add docs
class StringTokenImpl : public StringToken {
public:
    // TODO: Add support for canonicalization
    explicit StringTokenImpl(
        const type::TokenType* type,
        std::string_view value,
        std::size_t offset,
        CommentToken* precedingComment = nullptr
    )
        : StringToken(type, value, offset, precedingComment) { }

    // TODO: Add support for canonicalization
    explicit StringTokenImpl(
        const type::TokenType* type,
        std::span<dart::u8> data,
        bool asciiOnly,
        std::size_t offset,
        CommentToken* precedingComment = nullptr,
        bool allowLazy = true
    )
        : StringToken(
              type,
              std::string{data.begin(), data.end()},
              offset,
              precedingComment
          ) { }

    // TODO: Implement String value();
};

// TODO: Add docs
class CommentTokenImpl : public StringTokenImpl, public CommentToken {
public:
    explicit CommentTokenImpl(
        const type::TokenType* type,
        std::span<dart::u8> data,
        bool asciiOnly,
        std::size_t offset,
        CommentToken* precedingComment = nullptr,
        bool allowLazy = true
    )
        : StringTokenImpl(
              type, data, asciiOnly, offset, precedingComment, allowLazy
          ),
          CommentToken(type, std::string{data.begin(), data.end()}, offset) { }
};

// TODO: Add docs
class DartDocToken : public CommentTokenImpl {
public:
    explicit DartDocToken(
        const type::TokenType* type,
        std::span<dart::u8> data,
        bool asciiOnly,
        std::size_t offset,
        CommentToken* precedingComment = nullptr,
        bool allowLazy = true
    )
        : CommentTokenImpl(
              type, data, asciiOnly, offset, precedingComment, allowLazy
          ) { }
};

// TODO: Add docs
class LanguageVersionTokenImpl :
    public CommentTokenImpl,
    public LanguageVersionToken {
public:
    explicit LanguageVersionTokenImpl(
        std::span<dart::u8> data,
        bool asciiOnly,
        std::size_t offset,
        dart::u16 major,
        dart::u16 minor
    )
        : CommentTokenImpl(&type::SINGLE_LINE_COMMENT, data, asciiOnly, offset),
          LanguageVersionToken(
              std::string{data.begin(), data.end()}, offset, major, minor
          ) { }
};
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart