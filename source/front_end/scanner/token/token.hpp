#pragma once

#include "token/token_type.hpp"

#include <cstdint>
#include <format>
#include <initializer_list>
#include <string>

// TODO: Implement std::formatter<> for all Token types

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
// Forward declarations
namespace keyword {
class Keyword;
}

namespace type {
class TokenType;
}
class CommentToken;

/*
An interface representing a single token scanned from the input source text.
Each token keeps track of which token comes before it and which comes after, in
a doubly-linked list like structure.
*/
/*
Base class for all tokens processed by the scanner from the input source code.

Each token keeps track of the token before and after it in a doubly-linked list,
forming a stream of tokens.

Design note: The Dart version of this class is a pure interface - it only lists
field accessor methods (getters / setters) which are then implemented in derived
classes by providing the appropriate concrete field. Most of these accessor
methods do nothing other than get / set the field and some do the same work by
reusing the same fields.

This C++ implementation puts most of these fields (without any accessors) in
this class with default values (`false` for booleans, `nullptr` for pointers and
0 for integers). This makes the class hierarchy shorter (and simpler) to manage
at the cost of increasing the size of each class and assigning (instead of
initializing) the base fields from derived classes.
*/
class Token {
public:
    explicit Token(
        const type::TokenType* type,
        std::size_t beginOffset,
        std::size_t length,
        CommentToken* precedingComment = nullptr
    )
        : type{type},
          beginOffset{beginOffset},
          length{length},
          _precedingComment{precedingComment} {
        setCommentParent(precedingComment);
    }

    virtual ~Token() = default;

    std::string addr() const {
        return std::format("{:X}", reinterpret_cast<std::uintptr_t>(this));
    }

    /*
    Sets the given `token` to be the next one in the stream, effectively linking
    `this.next` to point to `token`:

        `this.next` ---> `token`

    If `setPrevious` is true, the linking also happens in reverse, linking
    `token.prev` to point to `this`:

        `this` <--- `token.prev`

    Returns `token` after linking.
    */
    Token* setNext(Token* token, bool setPrevious = true);

    /*
    Returns the next token in the stream.
    */
    Token* next() const { return _next; }

    /*
    Sets the given `token` to be the previous one in the stream, effectively
    linking `this.prev` to point to `token`:

        `token` <--- `this.prev`

    Unlike `setNext()`, the reverse linking does not take place.

    Returns `token` after linking.
    */
    Token* setPrev(Token* token);

    /*
    Returns the previous token in the stream.
    */
    Token* prev() const { return _prev; }

    /*
    Sets the `precedingComment` to the provided `comment` value.

    Once set, the `precedingComment` and any subsequent comments linked to it
    will all have their `parent` field set to `this`.
    */
    void setPrecedingComment(CommentToken* comment);

    /*
    Returns the first comment in a stream of comments that precede `this` token,
    or `nullptr` if no comments precede `this` token.

    Let's assume that after scanning the following source code, `this` token
    represents the `class` keyword after 2 comments:
    `// one`
    `// two`
    `class`   <-- `this` token

    Calling `precedingComment()` returns a `CommentToken` pointing to the first
    comment `// one`. Calling `next()` on that returns a `CommentToken` pointing
    to the second comment, `// two`. Calling `next()` on that returns `nullptr`
    as there are no more comments afterwards.

    Both `CommentToken` objects will have their `parent` field set to `this` as
    their parent.
    */
    CommentToken* precedingComment() const { return _precedingComment; }

    /*
    Returns `true` if `this` token's `type` is in any of the provided `types`,
    `false` otherwise.
    */
    bool matchesAny(std::initializer_list<const type::TokenType*> types) const;

    /*
    Returns `true` if `this` token's `type` is the same as the provided
    parameter value, `false` otherwise.
    */
    bool isA(const type::TokenType* type) const { return this->type == type; }

    /*
    The offset from the start of the source code to the first character of this
    token.
    */
    std::size_t beginOffset{0};

    /*
    The number of characters in this token.

    This is equivalent to `endOffset` - `beginOffset`.
    */
    std::size_t length{0};

    /*
    The offset from the start of the source code to one position past the last
    character of this token.
    */
    std::size_t endOffset{0};

    /*
    The token directly before `this` synthetic token, or `nullptr` if `this` is
    not a synthetic `)`, `]`, `}` or `>` token.
    */
    Token* beforeSynthetic{nullptr};

    /*
    The token that corresponds to `this` one, or `nullptr` if this token is not
    the first in a matching pair group of tokens (such as in parentheses).

    For example, if `this` token is an opening `(` token, then `endToken` is its
    closing `)` token. However if this token is an opening `(` token (or some
    other token), then `endToken` will be `nullptr`.
    */
    Token* endToken{nullptr};

    /*
    The `TokenType` associated with this token.

    This may be a keyword, identifier etc.
    */
    const type::TokenType* type{nullptr};

    /*
    The lexeme associated with this token as scanned from the source code.

    For typess such as `StringToken`, string quotation marks are included.
    */
    std::string lexeme{};

    /*
    The string value associated with this token.

    For symbols and keywords, the value is the token type / keyword associated
    with it. For `StringToken` types it's an empty string.
    */
    std::string_view stringValue{""};

    /*
    `true` if this token is an EOF (end-of-file), `false` otherwise.
    */
    bool isEof : 1 {false};

    /*
    `true` if this token is an identifier, `false` otherwise.

    Some keywords may also be identifiers depending on the context.
    */
    bool isIdentifier : 1 {false};

    /*
    `true` if this token is a keyword, `false` otherwise.
    */
    bool isKeyword : 1 {false};

    /*
    `true` if this token is a keyword or identifier, `false` otherwise.
    */
    bool isKeywordOrIdentifier : 1 {false};

    /*
    `true` if this token is a modifier (such as `const`, `static`, `final` etc),
    `false` otherwise.
    */
    bool isModifier : 1 {false};

    /*
    `true` if this token is an operator, `false` otherwise.
    */
    bool isOperator : 1 {false};

    /*
    `true` if this token is a synthetic one, `false` otherwise.

    A synthetic token is introduced / inserted into the token stream by the
    parser to help recover from an error.

    Since synthetic tokens aren't scanned from the source, they have a `length`
    of zero.
    */
    bool isSynthetic : 1 {false};

    /*
    `true` if this token is a keyword used in a top-level declaration, `false`
    otherwise. Top-level declaration keywords include the likes of `class`,
    `enum`, `import` etc.
    */
    bool isTopLevelKeyword : 1 {false};

    /*
    `true` if this token is a user-definable operator (such as `+`) on classes,
    mixins etc, `false` otherwise.
    */
    bool isUserDefinableOperator : 1 {false};

    /*
    Initializes some of the derived fields.

    This needs to be called as the last statement in the derived class'
    constructor body to properly initialize the rest of the class.
    */
    void init();

private:
    /*
    Sets the `CommentToken::parent` value of the given `comment` to `this`.

    All subsequent comments in the stream (arrived via `comment->next()`) will
    also have their `parent` field set to `this`.
    */
    void setCommentParent(CommentToken* comment);

    Token* _next{nullptr};
    Token* _prev{nullptr};
    CommentToken* _precedingComment{nullptr};
};

/*
Returns the left-most token from the provided list of `tokens`.

A token is left-most if it appears first in the source code. This also means it
has the smallest `Token::beginOffset` value from all other tokens in the list.

A `nullptr` is returned if the list is empty or only contains `nullptr` values.
*/
const Token* lexicallyFirst(std::initializer_list<const Token*> tokens);
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart