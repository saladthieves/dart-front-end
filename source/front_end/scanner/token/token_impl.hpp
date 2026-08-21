#pragma once

#include "string_token.hpp"

#include "token/comment_token.hpp"
#include "token/token_type.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
// TODO: Add docs
class StringTokenImpl : public StringToken {
public:
    StringTokenImpl(
        const type::TokenType* type,
        std::string_view value,
        std::size_t offset,
        // TODO: Add support for canonicalization
        CommentToken* precedingComment = nullptr
    )
        : StringToken(type, value, offset, precedingComment) { }

    // TODO: Implement String value();
};

// TODO: Add docs
class CommentTokenImpl : public StringTokenImpl, public CommentToken {
public:
    // TODO: Implement appropriate constructors
};

// TODO: Add docs
class DartDocComment : public CommentTokenImpl {
public:
    // TODO: Implement appropriate constructors
};
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart