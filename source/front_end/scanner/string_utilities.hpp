#pragma once

#include "interner.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace string_utils {
inline constexpr NullInterner NULL_INTERNER = NullInterner{};

inline constexpr std::string_view intern(std::string_view string) {
    return NULL_INTERNER.intern(string);
}

} // namespace string_utils
} // namespace scanner
} // namespace front_end
} // namespace dart