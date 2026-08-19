#include "token_type.hpp"
#include "precedence.hpp"
#include "token_types.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
namespace type {
constexpr bool TokenType::isAssociativeOperator() const {
    return this == &type::AMPERSAND ||           //
           this == &type::AMPERSAND_AMPERSAND || //
           this == &type::BAR ||                 //
           this == &type::BAR_BAR ||             //
           this == &type::CARET ||               //
           this == &type::PLUS ||                //
           this == &type::STAR;
}

constexpr bool TokenType::isEqualityOperator() const {
    return this == &type::BANG_EQ || this == &type::EQ_EQ;
}

constexpr bool TokenType::isIncrementOperator() const {
    return this == &type::PLUS_PLUS || this == &type::MINUS_MINUS;
}

constexpr bool TokenType::isRelationalOperator() const {
    return this == &type::LT || this == &type::LT_EQ || this == &type::GT ||
           this == &type::GT_EQ;
}

constexpr bool TokenType::isUnaryPrefixOperator() const {
    return precedence == precedence::PREFIX || //
           this == &type::MINUS ||             //
           this == &type::PLUS_PLUS ||         //
           this == &type::MINUS_MINUS;
}

} // namespace type
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart