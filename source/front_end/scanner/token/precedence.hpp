#pragma once

#include "common/types.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
namespace precedence {
// clang-format off
inline constexpr dart::u8 NONE           = 0;   // Default (lowest)
inline constexpr dart::u8 ASSIGNMENT     = 1;   // =, +=, /=
inline constexpr dart::u8 CASCADE        = 2;   // ..
inline constexpr dart::u8 CONDITIONAL    = 3;   // ?
inline constexpr dart::u8 IF_NULL        = 4;   // ??
inline constexpr dart::u8 LOGICAL_OR     = 5;   // ||
inline constexpr dart::u8 LOGICAL_AND    = 6;   // &&
inline constexpr dart::u8 EQUALITY       = 7;   // !=, !==, ==, ===
inline constexpr dart::u8 RELATIONAL     = 8;   // <, <=, >, >=
inline constexpr dart::u8 BITWISE_OR     = 9;   // |
inline constexpr dart::u8 BITWISE_XOR    = 10;  // ^
inline constexpr dart::u8 BITWISE_AND    = 11;  // &
inline constexpr dart::u8 SHIFT          = 12;  // >>, >>>, <<
inline constexpr dart::u8 ADDITIVE       = 13;  // -, +
inline constexpr dart::u8 MULTIPLICATIVE = 14;  // %, /, *, ~/
inline constexpr dart::u8 PREFIX         = 15;  // !, ~
inline constexpr dart::u8 POSTFIX        = 16;  // ++, --
inline constexpr dart::u8 SELECTOR       = 17;  // [], (, [, ., ?.
// clang-format on
} // namespace precedence
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart