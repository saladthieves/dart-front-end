#pragma once

#include "common/types.hpp"

#include <format>
#include <string>

namespace dart {
namespace front_end {
namespace messages {
namespace format {
inline std::string charToUnicode(dart::u16 character) {
    return std::format("U+{:04X}", character);
}
} // namespace format
} // namespace messages
} // namespace front_end
} // namespace dart