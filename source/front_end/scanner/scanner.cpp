#include "scanner.hpp"

namespace dart {
namespace front_end {
namespace scanner {
auto LineStarts::createInitialArray(std::size_t numberOfBytesHint)
    -> std::vector<Int> {
    // Make an assumption of 22 bytes per line.
    const auto numberOfLines = (numberOfBytesHint / 22) + 1;
    return std::vector<Int>(numberOfLines);
}

} // namespace scanner
} // namespace front_end
} // namespace dart