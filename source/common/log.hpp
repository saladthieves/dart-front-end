#pragma once

#include <iostream>

namespace dart {
inline namespace common {
namespace log {
using namespace std::string_view_literals;

enum class LogLevel : std::uint8_t {
    FATAL = 1,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
    VERBOSE,
    RAW,
};

// clang-format off
inline constexpr auto LABEL_FATAL   = "F"sv;
inline constexpr auto LABEL_ERROR   = "E"sv;
inline constexpr auto LABEL_WARNING = "W"sv;
inline constexpr auto LABEL_INFO    = "I"sv;
inline constexpr auto LABEL_DEBUG   = "D"sv;
inline constexpr auto LABEL_VERBOSE = "V"sv;
// clang-format on

inline constexpr std::string_view getLevelLabel(const LogLevel& level) {
    switch (level) {
        case LogLevel::FATAL:   return LABEL_FATAL;
        case LogLevel::ERROR:   return LABEL_ERROR;
        case LogLevel::WARNING: return LABEL_WARNING;
        case LogLevel::INFO:    return LABEL_INFO;
        case LogLevel::DEBUG:   return LABEL_DEBUG;
        case LogLevel::VERBOSE: return LABEL_VERBOSE;
        default:                return "";
    }
}

// TODO: Work on the colors
template <typename... T>
inline constexpr void
out(const LogLevel& level, std::format_string<T...> string, T&&... args) {
    using enum LogLevel;
    if (level == RAW) {
        std::println(string, std::forward<T>(args)...);
        return;
    }

    auto& stream = level == ERROR || level == FATAL ? std::cerr : std::cout;
    auto label = getLevelLabel(level);
    auto message = std::format(string, std::forward<T>(args)...);
    std::println(stream, "{} → {}", label, message);
} // namespace

template <typename... T>
inline constexpr void f(std::format_string<T...> string = "", T&&... args) {
    out(LogLevel::FATAL, string, std::forward<T>(args)...);
}

template <typename... T>
inline constexpr void e(std::format_string<T...> string = "", T&&... args) {
    out(LogLevel::ERROR, string, std::forward<T>(args)...);
}

template <typename... T>
inline constexpr void w(std::format_string<T...> string = "", T&&... args) {
    out(LogLevel::WARNING, string, std::forward<T>(args)...);
}

template <typename... T>
inline constexpr void i(std::format_string<T...> string = "", T&&... args) {
    out(LogLevel::INFO, string, std::forward<T>(args)...);
}

template <typename... T>
inline constexpr void d(std::format_string<T...> string = "", T&&... args) {
    out(LogLevel::DEBUG, string, std::forward<T>(args)...);
}

template <typename... T>
inline constexpr void v(std::format_string<T...> string = "", T&&... args) {
    out(LogLevel::VERBOSE, string, std::forward<T>(args)...);
} // namespace log
} // namespace log
} // namespace common
} // namespace dart