#include "scanner.hpp"
#include "common/log.hpp"
#include "utf8_bytes_scanner.hpp"

namespace dart {
namespace front_end {
namespace scanner {
// LineStarts
auto LineStarts::createInitialArray(std::size_t numberOfBytesHint)
    -> std::vector<Int> {

    // Make an assumption of 22 bytes per line.
    const auto length = (numberOfBytesHint / 22) + 1;

    std::vector<Int> array{};
    array.reserve(length); // preallocate
    return array;
}

// scan()
void scan(
    std::span<dart::u8> bytes,
    ScannerConfiguration* configuration,
    bool includeComments,
    LanguageVersionChanged* languageVersionChanged,
    bool allowLazyStrings
) {
    Utf8BytesScanner scanner = Utf8BytesScanner{
        bytes, configuration, includeComments, languageVersionChanged,
        allowLazyStrings
    };

    auto* tokens = scanner.tokenize();

    if (scanner.hasErrors()) {
        log::d("Scanning failed");
    } else {
        log::d("Scanning successful");
    }
}

} // namespace scanner
} // namespace front_end
} // namespace dart