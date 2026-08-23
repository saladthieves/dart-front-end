#include "scanner_main.hpp"
#include "scanner.hpp"

#include "common/log.hpp"
#include "token/comment_token.hpp"
#include "util/file_utils.hpp"

#include <string_view>

namespace dart {
namespace front_end {
namespace scanner {
int run(int argc, const char* argv[]) {
    if (argc <= 1) {
        log::e("No Dart source file provided path provided.");
        return 65;
    }

    const std::string_view path{argv[1]};
    auto bytes = util::file::readFileAsBytes(path); 
    if (bytes.empty()) {
        return 65;
    }

    ScannerConfiguration* config = new ScannerConfiguration{};

    LanguageVersionChanged callback =
        [](const Scanner<>* scanner, const token::LanguageVersionToken* token) {
            log::d("Language changed: {}.{}", token->major, token->minor);
        };

    scan(bytes, config, true, &callback, false);

    delete config;

    return 0;
}
} // namespace scanner
} // namespace front_end
} // namespace dart