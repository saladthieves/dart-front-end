#pragma once

#include "precedence.hpp"

#include "common/types.hpp"

#include <string_view>

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
/*
Represents a group / class that a particular token belongs to, especially if
they share similar uses.
*/
class TokenClass {
public:
    constexpr explicit TokenClass(
        std::string_view name, dart::u8 precedence = precedence::NONE
    )
        : name{name},
          precedence{precedence} {}

    /*
    The name of the token class.
    */
    const std::string_view name;

    /*
    The precedence of the tokens of this class, or zero if the tokens do not
    represent an operator.
    */
    const dart::u8 precedence;
};
}
} // namespace scanner
} // namespace front_end
} // namespace dart