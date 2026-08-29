#pragma once

#include "common/types.hpp"
#include "token/characters.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace internal_utils { // TODO: Move to utils

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

inline constexpr std::array<bool, 256> IDENTIFIER_TABLE{
    // clang-format off
    false, false, false, false, false, false, false, false, //   0 - 7
    false, false, false, false, false, false, false, false, //   8 - 15
    false, false, false, false, false, false, false, false, //  16 - 23
    false, false, false, false, false, false, false, false, //  24 - 31
    false, false, false, false, true,  false, false, false, //  32 - 39  |  36 = $
    false, false, false, false, false, false, false, false, //  40 - 47 
    true,  true,  true,  true,  true,  true,  true,  true,  //  48 - 55  |  48 = 0 .. 55 = 7 
    true,  true,  false, false, false, false, false, false, //  56 - 63  |  56 = 8, 57 = 9
    false, true,  true,  true,  true,  true,  true,  true,  //  64 - 71  |  65 = A .. 71 = G
    true,  true,  true,  true,  true,  true,  true,  true,  //  72 - 79  |  72 = H .. 79 = O
    true,  true,  true,  true,  true,  true,  true,  true,  //  80 - 87  |  80 = P .. 87 = W
    true,  true,  true,  false, false, false, false, true,  //  88 - 95  |  88 = X .. 90 = Z, 95 = _
    false, true,  true,  true,  true,  true,  true,  true,  //  96 - 103 |  97 = a .. 103 = g
    true,  true,  true,  true,  true,  true,  true,  true,  // 104 - 111 | 104 = h .. 111 = o
    true,  true,  true,  true,  true,  true,  true,  true,  // 112 - 119 | 112 = p .. 119 = w
    true,  true,  true,  false, false, false, false, false, // 120 - 127 | 120 = x .. 122 = z
    false, false, false, false, false, false, false, false, // 128 - 135
    false, false, false, false, false, false, false, false, // 136 - 143
    false, false, false, false, false, false, false, false, // 144 - 151
    false, false, false, false, false, false, false, false, // 152 - 159
    false, false, false, false, false, false, false, false, // 160 - 167
    false, false, false, false, false, false, false, false, // 168 - 175
    false, false, false, false, false, false, false, false, // 176 - 183
    false, false, false, false, false, false, false, false, // 184 - 191
    false, false, false, false, false, false, false, false, // 192 - 199
    false, false, false, false, false, false, false, false, // 200 - 207
    false, false, false, false, false, false, false, false, // 208 - 215
    false, false, false, false, false, false, false, false, // 216 - 223
    false, false, false, false, false, false, false, false, // 224 - 231
    false, false, false, false, false, false, false, false, // 232 - 239
    false, false, false, false, false, false, false, false, // 240 - 247
    false, false, false, false, false, false, false, false, // 248 - 255
    // clang-format on
};

template <typename Int = dart::u8>
inline constexpr bool isIdentifierCharAllowDollarTableLookup(Int next) {
    return IDENTIFIER_TABLE[next];
}
} // namespace internal_utils
} // namespace scanner
} // namespace front_end
} // namespace dart