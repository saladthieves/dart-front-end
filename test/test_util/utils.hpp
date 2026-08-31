#pragma once

#include "common/types.hpp"

#include <iterator>
#include <span>
#include <string_view>
#include <vector>

using std::span;
using std::string_view;
using std::vector;

namespace test {
inline span<dart::u8> readToBytes(vector<dart::u8>& bytes, string_view code) {
    bytes.clear();
    bytes.reserve(code.length());
    std::copy(code.begin(), code.end(), std::back_inserter(bytes));
    return span{bytes.begin(), bytes.end()};
}
} // namespace test