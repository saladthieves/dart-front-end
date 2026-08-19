#pragma once

#include "token_class.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
// clang-format off
/*
Used to indicate that the token type does not belong to any particular class of
tokens.
*/
inline constexpr TokenClass NO_CLASS = TokenClass("NO_CLASS");

/*
Used to indicate that the token type is an additive operator.
*/
inline constexpr TokenClass ADDITIVE_OPERATOR = TokenClass(
    "ADDITIVE_OPERATOR",
    precedence::ADDITIVE
);

/*
Used to indicate that the token type is an assignment operator.
*/
inline constexpr TokenClass ASSIGNMENT_OPERATOR = TokenClass(
    "ASSIGNMENT_OPERATOR",
    precedence::ASSIGNMENT
);

/*
Used to indicate that the token type is a bitwise-and operator.
*/
inline constexpr TokenClass BITWISE_AND_OPERATOR = TokenClass(
    "BITWISE_AND_OPERATOR",
    precedence::BITWISE_AND
);

/*
Used to indicate that the token type is a bitwise-or operator.
*/
inline constexpr TokenClass BITWISE_OR_OPERATOR = TokenClass(
    "BITWISE_OR_OPERATOR",
    precedence::BITWISE_OR
);

/*
Used to indicate that the token type is a bitwise-xor operator.
*/
inline constexpr TokenClass BITWISE_XOR_OPERATOR = TokenClass(
    "BITWISE_XOR_OPERATOR",
    precedence::BITWISE_XOR
);

/*
Used to indicate that the token type is a cascade operator.
*/
inline constexpr TokenClass CASCADE_OPERATOR = TokenClass(
    "CASCADE_OPERATOR",
    precedence::CASCADE
);

/*
Used to indicate that the token type is a conditional operator.
*/
inline constexpr TokenClass CONDITIONAL_OPERATOR = TokenClass(
    "CONDITIONAL_OPERATOR",
    precedence::CONDITIONAL
);

/*
Used to indicate that the token type is an equality operator.
*/
inline constexpr TokenClass EQUALITY_OPERATOR = TokenClass(
    "EQUALITY_OPERATOR",
    precedence::EQUALITY
);

/*
Used to indicate that the token type is an if-null operator.
*/
inline constexpr TokenClass IF_NULL_OPERATOR = TokenClass(
    "IF_NULL_OPERATOR",
    precedence::IF_NULL
);

/*
Used to indicate that the token type is a logical-and operator.
*/
inline constexpr TokenClass LOGICAL_AND_OPERATOR = TokenClass(
    "LOGICAL_AND_OPERATOR",
    precedence::LOGICAL_AND
);

/*
Used to indicate that the token type is a logical-or operator.
*/
inline constexpr TokenClass LOGICAL_OR_OPERATOR = TokenClass(
    "LOGICAL_OR_OPERATOR",
    precedence::LOGICAL_OR
);

/*
Used to indicate that the token type is a multiplicative operator.
*/
inline constexpr TokenClass MULTIPLICATIVE_OPERATOR = TokenClass(
    "MULTIPLICATIVE_OPERATOR",
    precedence::MULTIPLICATIVE
);

/*
Used to indicate that the token type is a relational operator.
*/
inline constexpr TokenClass RELATIONAL_OPERATOR = TokenClass(
    "RELATIONAL_OPERATOR",
    precedence::RELATIONAL
);

/*
Used to indicate that the token type is a shift operator.
*/
inline constexpr TokenClass SHIFT_OPERATOR = TokenClass(
    "SHIFT_OPERATOR",
    precedence::SHIFT
);

/*
Used to indicate that the token type is a unary postfix operator.
*/
inline constexpr TokenClass UNARY_POSTFIX_OPERATOR = TokenClass(
    "UNARY_POSTFIX_OPERATOR",
    precedence::POSTFIX
);

/*
Used to indicate that the token type is a unary prefix operator.
*/
inline constexpr TokenClass UNARY_PREFIX_OPERATOR = TokenClass(
    "UNARY_PREFIX_OPERATOR",
    precedence::PREFIX
);
// clang-format on
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart