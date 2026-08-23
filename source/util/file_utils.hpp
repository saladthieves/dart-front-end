#pragma once

#include "common/log.hpp"
#include "common/types.hpp"

#include <filesystem>
#include <fstream>
#include <vector>

namespace dart {
namespace util {
namespace file {
inline std::vector<dart::u8> readFileAsBytes(std::string_view path) {
    const auto file = std::filesystem::absolute(path);

    if (!std::filesystem::exists(file)) {
        log::e("The provided Dart file path does not exist: `{}`", path);
        return {};
    }

    const std::size_t length = std::filesystem::file_size(path);
    if (length == 0) {
        log::e("The provided Dart file is empty: `{}`", path);
        return {};
    }

    std::ifstream stream{file, std::ios_base::binary};
    if (!stream.is_open()) {
        log::e("Failed to open Dart file: `{}`", path);
        return {};
    }

    std::vector<dart::u8> bytes(length);

    stream.read(reinterpret_cast<char*>(bytes.data()), length);
    if (stream.fail() || length != stream.gcount()) {
        log::e("Failed to read from Dart file stream: `{}`", path);
        bytes.clear();
        return {};
    }

    stream.close();
    
    return bytes;
}

} // namespace file
} // namespace util
} // namespace dart