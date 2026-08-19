#pragma once

#include "simple_token.hpp"
#include "string_utilities.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
/*
A token whose value is independent of its types.

An example of such a token is a comment token.
*/
class StringToken : public SimpleToken {
public:
    explicit StringToken(
        const type::TokenType* type,
        std::string_view value,
        std::size_t offset,
        CommentToken* precedingComment = nullptr
    )
        : SimpleToken(type, offset, precedingComment),
          value{string_utils::intern(value)} {}

    virtual bool isIdentifier() const override {
        return getKind() == constants::IDENTIFIER_TOKEN;
    }

    virtual std::string_view getLexeme() const override { return value; }

    // TODO: Implement String value();

private:
    /*
    The lexeme represented by this token.
    */
    std::string value;
};
}
} // namespace scanner
} // namespace front_end
} // namespace dart