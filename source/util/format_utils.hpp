#pragma once

#include <cstdint>
#include <format>
#include <string>

namespace dart {
namespace util {
namespace fmt {
inline constexpr std::string formatAddress(const void* pointer) {
    if (pointer == nullptr) {
        return "nullptr";
    } else {
        return std::format("0x{:016X}", reinterpret_cast<std::uintptr_t>(pointer));
    }
}
} // namespace fmt
} // namespace util
} // namespace dart

// 000000007FC9DF904370