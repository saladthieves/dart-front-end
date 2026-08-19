#pragma once

#include "log.hpp"

#include <format>
#include <stdexcept>
#include <string_view>

namespace dart {
inline namespace common {
inline namespace assert {

class AssertionError : public std::logic_error {
public:
    explicit constexpr AssertionError(std::string_view message)
        : std::logic_error(message.data()) {}
};

template <typename... T>
inline constexpr void
assert(bool condition, std::format_string<T...> string, T&&... args) {
    if (condition) return;
    
    using namespace std::string_view_literals;
    const auto data = std::format(string, std::forward<T>(args)...);
    const auto message = std::format("Assertion failed: {}", data);
    throw AssertionError(message);
}

template <typename... T>
inline constexpr void assert(bool condition) {
    if (condition) return;
    
    using namespace std::string_view_literals;
    constexpr auto message = "Assertion failed."sv;
    dart::log::f(message);
    throw AssertionError(message);
}

} // namespace assert
} // namespace common
} // namespace dart