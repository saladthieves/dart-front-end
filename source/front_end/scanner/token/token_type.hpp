#pragma once

#include "precedence.hpp"
#include "token_constants.hpp"

#include "common/types.hpp"

#include <string_view>

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
namespace type {
/*
The base class that represents the types of tokens a scanner can return.

Some token types (such as identifiers, operators, comments etc) are directly
defined by this class, while others (such as keywords) are defined by derived
classes.

For all the token types defined by this class, see `token_types.hpp`.
For all the keyword token types, see `keyword.hpp` and `keywords.hpp`.
*/
class TokenType {
public:
    explicit constexpr TokenType(
        dart::u8 index,
        std::string_view lexeme,
        std::string_view name,
        dart::u8 precedence,
        dart::u8 kind,
        const TokenType* binaryOperatorOfCompoundAssignment = nullptr,
        bool isBinaryOperator = false,
        bool isModifier = false,
        bool isOperator = false,
        bool isTopLevelKeyword = false,
        bool isUserDefinableOperator = false,
        bool stringValueShouldBeNull = false
    )
        : index{index},
          lexeme{lexeme},
          name{name},
          precedence{precedence},
          kind{kind},
          binaryOperatorOfCompoundAssignment{
              binaryOperatorOfCompoundAssignment
          },
          isBinaryOperator{isBinaryOperator},
          isModifier{isModifier},
          isOperator{isOperator},
          isTopLevelKeyword{isTopLevelKeyword},
          isUserDefinableOperator{isUserDefinableOperator},
          stringValue{stringValueShouldBeNull ? nullptr : lexeme.data()} {}

    /*
    Disable copy constructor - `TokenType` and its derivatives should not be
    copy-constructible.
    */
    TokenType(const TokenType& other) = delete;

    /*
    Disable copy-assignment operator - `TokenType` and its derivatives should
    not be copy-assignable.
    */
    TokenType& operator =(const TokenType& other) = delete;

    constexpr virtual ~TokenType() = default;

    /*
    Returns `true` if this token type represents an additive operator.
    */
    constexpr bool isAdditiveOperator() const {
        return precedence == precedence::ADDITIVE;
    }

    /*
    Returns `true` if this token type represents an assignment operator.
    */
    constexpr bool isAssignmentOperator() const {
        return precedence == precedence::ASSIGNMENT;
    }

    /*
    Returns `true` if this token type support associativity. An operator is
    associative if (using addition as an example) the following evaluates to
    `true`:
        (a + b) + c == a + (b + c)

    The result of applying such an operator to operands does not depend on the
    order in which they're evaluated.
    */
    constexpr bool isAssociativeOperator() const;

    /*
    Returns `true` if this token type represents a built-in keyword, or `false`.
    */
    constexpr virtual bool isBuiltIn() const { return false; }

    /*
    Returns `true` if this token type is a reserved word, `false` if not.
    */
    constexpr virtual bool isReservedWord() const { return false; }

    /*
    Returns `true` if this token type represents an equality operator, `false`
    otherwise.
    */
    constexpr bool isEqualityOperator() const;

    /*
    Returns `true` if this token type represents an increment operator, `false`
    otherwise.
    */
    constexpr bool isIncrementOperator() const;

    /*
    Returns `true` if this token type represents a keyword, `false` if not.
    */
    constexpr virtual bool isKeyword() const {
        return kind == constants::KEYWORD_TOKEN;
    }

    /*
    Returns `true` if this token type can be used as an identifier in some
    scenarios.
    */
    constexpr virtual bool isPseudo() const { return false; }

    /*
    Returns `true` if this token type is a multiplicative operator, `false` if
    not.
    */
    constexpr bool isMultiplicativeOperator() const {
        return precedence == precedence::MULTIPLICATIVE;
    }

    /*
    Returns `true` if this token type is a relational operator, `false` if not.
    */
    constexpr bool isRelationalOperator() const;

    /*
    Returns `true` if this token type represents a shift operator, `false`
    otherwise.
    */
    constexpr bool isShiftOperator() const {
        return precedence == precedence::SHIFT;
    }

    /*
    Returns `true` if this token type represents a unary postfix operator, or
    `false` if not.
    */
    constexpr bool isUnaryPostfixOperator() const {
        return precedence == precedence::POSTFIX;
    }

    /*
    Returns `true` if this token type represents a unary prefix operator.
    */
    constexpr bool isUnaryPrefixOperator() const;

    /*
    Returns `true` if this token type represents a selector operator, `false`
    otherwise.
    */
    constexpr bool isSelectorOperator() const {
        return precedence == precedence::SELECTOR;
    }

    /*
    Returns `true` if `this` token type object is the same one as `value` (if
    they're the same pointer values), `false` otherwise.
    */
    constexpr bool isA(const TokenType* value) const { return this == value; }

    // TODO: Implement std::formatter<TokenType>

    /*
    A unique integer value to identify this `TokenType`.
    */
    const dart::u8 index;

    /*
    The binary operator that is invoked by this compound assignment operator,
    or `nullptr` if none.

    For example, if `this` is a `BAR_BAR_EQ` (||=) token type, the pointer
    below would point to `BAR_BAR`.
    */
    const TokenType* const binaryOperatorOfCompoundAssignment;

    /*
    The kind of token, as identified in the `token_constants.hpp` header file.
    */
    const dart::u8 kind;

    /*
    `true` if this token type represents a modifier (such as `const`), `false`
    otherwise.
    */
    const bool isModifier;

    /*
    `true` if this token type is an operator, `false` otherwise.
    */
    const bool isOperator;

    /*
    `true` if this token type is a binary operator, `false` otherwise.
    */
    const bool isBinaryOperator;

    /*
    `true` if this token type is a top-level keyword (such as 'class', 'enum',
    etc) for declarations, `false` otherwise.
    */
    const bool isTopLevelKeyword;

    /*
    `true` if this token type is an operator a user can define, `false` if not.
    */
    const bool isUserDefinableOperator;

    /*
    The lexeme that defines this type of token. If there is more than one
    possible lexeme for this token type, then the value `null` will be used.
    */
    const std::string_view lexeme;

    /*
    The name of this token type.
    */
    const std::string_view name;

    /*
    The precedence of this token type, if it represents an operator. If it does
    not represent an operator, the value defaults to zero (0).
    */
    const dart::u8 precedence;

    /*
    TODO: Add explanation after `Token` implementation.
    */
    const char* const stringValue;
};
} // namespace type
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart