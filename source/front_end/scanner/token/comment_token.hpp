#pragma once

#include "token.hpp"
#include "token/token_type.hpp"
#include "token_types.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {

// TODO: Add support for lazy strings

/*
A token representing a comment.

If the comment is in a stream of comment tokens, previous and next comments can
be reached via the `next()` and `prev()` functions, respectively.

A comment token also has a `parent`, which is the next non-comment token that
directly follows the token stream.
*/
class CommentToken : public Token {
public:
    explicit CommentToken(
        const type::TokenType* type, //
        std::size_t beginOffset,
        std::string_view stringValue
    )
        : Token{type, beginOffset, stringValue.length()} {
        lexeme = stringValue;

        init();
    }

    explicit CommentToken(
        const type::TokenType* type,
        std::span<dart::u8> bytes,
        std::size_t start,
        std::size_t stop,
        std::size_t beginOffset,
        bool asciiOnly
    )
        : CommentToken{
              type, beginOffset,
              std::string{bytes.begin() + start, bytes.begin() + stop}
    } { }

    /*
    The parent token that contains this comment.
    */
    const Token* parent{nullptr};
};

/*
A token representing a Dart documentation comment.

A Dart doc comment begins with `/✶✶` or `///`.
*/
class DocumentationCommentToken : public CommentToken {
public:
    explicit DocumentationCommentToken(
        const type::TokenType* type, //
        std::size_t beginOffset,
        std::string_view stringValue
    )
        : CommentToken{type, beginOffset, stringValue} { }

    explicit DocumentationCommentToken(
        const type::TokenType* type,
        std::span<dart::u8> bytes,
        std::size_t start,
        std::size_t stop,
        std::size_t beginOffset,
        bool asciiOnly
    )
        : CommentToken{type, bytes, start, stop, beginOffset, asciiOnly} { }
};

// TODO: Add docs
class DartDocToken : public CommentToken {
public:
    explicit DartDocToken(
        const type::TokenType* type,
        std::span<dart::u8> bytes,
        std::size_t start,
        std::size_t stop,
        std::size_t beginOffset,
        bool asciiOnly
    )
        : CommentToken{type, bytes, start, stop, beginOffset, asciiOnly} { }
};

/*
A specialized comment representing the Dart language version of the file.

The Dart language version comment takes the format:

    `// @dart = MAJOR.MINOR`

Where `MAJOR` and `MINOR` are the Dart language major and minor version values,
respectively. Example:

    `// @dart = 3.1`
*/
class LanguageVersionToken : public CommentToken {
public:
    explicit LanguageVersionToken(
        std::size_t beginOffset,
        std::string_view stringValue,
        dart::u16 major,
        dart::u16 minor
    )
        : CommentToken{&type::SINGLE_LINE_COMMENT, beginOffset, stringValue},
          major{major},
          minor{minor} { }

    explicit LanguageVersionToken(
        std::span<dart::u8> bytes,
        std::size_t start,
        std::size_t stop,
        std::size_t beginOffset,
        dart::u16 major,
        dart::u16 minor
    )
        : LanguageVersionToken{
              beginOffset,
              std::string{bytes.begin() + start, bytes.begin() + stop},
              major,
              minor
    } { }

    /*
    The major version.

    In a version such as `3.1`, major would be `3`.
    */
    dart::u16 major;
    
    /*
    The minor version.

    In a version such as `3.1`, minor would be `1`.
    */
    dart::u16 minor;
};
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart