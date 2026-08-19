#pragma once

#include "string_token.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
/*
A token representing a comment.

A token has a `parent`, which is can be the next non-comment token in the field.
*/
class CommentToken : public StringToken {
public:
    explicit CommentToken(
        const type::TokenType* type, //
        std::string_view value,
        std::size_t offset
    )
        : StringToken(type, value, offset) {}

    /*
    The parent token that contains this comment.
    */
    const SimpleToken* parent{nullptr};
};

/*
A token representing a documentation comment.
*/
class DocumentationCommentToken : public CommentToken {
public:
    explicit DocumentationCommentToken(
        const type::TokenType* type, //
        std::string_view value,
        std::size_t offset
    )
        : CommentToken(type, value, offset) {}
};

/*
A specialized comment representing the Dart language version which includes the
major and minor parts.
Example: `// '@dart = 2.1'
*/
class LanguageVersionToken : public CommentToken {
public:
    explicit LanguageVersionToken(
        std::string_view text,
        std::size_t offset,
        dart::u16 major,
        dart::u16 minor
    )
        : CommentToken(&type::SINGLE_LINE_COMMENT, text, offset),
          major{major},
          minor{minor} {}

    dart::u16 major;
    dart::u16 minor;
};
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart