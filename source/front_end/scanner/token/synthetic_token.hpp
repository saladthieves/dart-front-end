#pragma once

#include "begin_token.hpp"
#include "keyword_token.hpp"
#include "string_token.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
/*
The base class for synthetic tokens.

A synthetic token is one that's generated / inserted into the stream (such as
by a parser to recover from errors), and not one that originates from the source
code.
*/
class SyntheticToken : public Token {
public:
    explicit SyntheticToken(
        const type::TokenType* type,
        std::size_t beginOffset,
        const Token* beforeSynthetic = nullptr
    )
        : Token(type, beginOffset, 0),
          beforeSynthetic{beforeSynthetic} {
        init();
    }

    const Token* beforeSynthetic{nullptr};
};

/*
A synthetic token used to replace another token in the stream.

The token being replaced is kept inside the `replacedToken` field. The offset
and precedingComment values of the replacement token are set from the replaced
one.
*/
class ReplacementToken : public SyntheticToken {
public:
    explicit ReplacementToken(
        const type::TokenType* type, //
        const Token* replacedToken,
        const Token* beforeSynthetic = nullptr
    )
        : SyntheticToken{type, replacedToken->beginOffset, beforeSynthetic},
          replacedToken{replacedToken} {
        setPrecedingComment(replacedToken->precedingComment());
    }

    /*
    The token that `this` token replaces.
    This will normally correspond to what the user originally typed in.
    */
    const Token* replacedToken;
};

/*
A synthetic version of a begin token.
*/
class SyntheticBeginToken : public BeginToken {
public:
    explicit SyntheticBeginToken(
        const type::TokenType* type,
        std::size_t beginOffset,
        CommentToken* precedingComment = nullptr
    )
        : BeginToken(type, beginOffset, 0, precedingComment) { }
};

/*
A synthetic version of a keyword token.
*/
class SyntheticKeywordToken : public KeywordToken {
public:
    explicit SyntheticKeywordToken(
        const type::TokenType* keyword, //
        std::size_t beginOffset
    )
        : KeywordToken(keyword, beginOffset) { }
};

/*
A synthetic version of a string token.
*/
class SyntheticStringToken : public StringToken {
public:
    explicit SyntheticStringToken(
        const type::TokenType* type,
        std::size_t beginOffset,
        std::string_view stringValue
    )
        : StringToken{type, beginOffset, stringValue, 0} { }
};
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart