#pragma once

#include "common/types.hpp"
#include "token/characters.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace internal_utils {
/*
Returns `true` if the provided `next` value is a valid identifier character,
`false` otherwise.
*/
template <typename Int = dart::u8>
inline constexpr bool isIdentifierChar(Int next, bool allowDollar) {
    using namespace token::chars;
    return ($a <= next && next <= $z) || ($A <= next && next <= $Z) ||
           ($0 <= next && next <= $9) || next == $_ ||
           (next == $$ && allowDollar);
}
} // namespace internal_utils
} // namespace scanner
} // namespace front_end
} // namespace dart