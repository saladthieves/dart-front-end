#pragma once

#include <cstddef>

// TODO: Add missing docs
// TODO: Add implementation
namespace dart {
namespace front_end {
namespace base {
/*
Defines an arbitrary range of characters within the source code.
*/
class SourceRange {
public:
    /*
    Creates an empty range, with an `offset` and `length` of zero.
    */
    constexpr SourceRange() = default;

    /*
    Creates a range with the given `offset` and `length`.
    */
    explicit constexpr SourceRange(std::size_t offset, std::size_t length)
        : offset{offset},
          length{length} {}

private:
    /*
    The zero-based index of the first character in the range.
    */
    std::size_t offset{0};

    /*
    The number of characters in the range.
    */
    std::size_t length{0};
};
} // namespace base
} // namespace front_end
} // namespace dart