#include "abstract_scanner.hpp"

#include "token/characters.hpp"
#include "token/error_token.hpp"
#include "token/token_constants.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace {
using namespace token::chars;
using namespace token::constants;

using Int = Scanner<>::Int;
using token::Token;
using token::NonAsciiIdentifierToken;

using token::buildUnexpectedCharacterToken;
} // namespace

// AbstractScanner
void AbstractScanner::setConfiguration(const ScannerConfiguration* config) {
    if (config != nullptr) {
        enableTripleShift = config->enableTripleShift;
        enableAugmentations = config->enableAugmentations;
    }
}

const token::Token* AbstractScanner::tokenize() {
    while (!atEndOfFile()) {}
    return nullptr;
}

Int AbstractScanner::scanHeaderLookingForLanguageVersion(Int next) {
    if (next == $EOF) return next;

    Token* oldTail = tail;
    next = bigHeaderSwitch(next);
    if (next != $EOF && tail->getKind() == SCRIPT_TOKEN) {
        oldTail = tail;
        next = bigHeaderSwitch(next);
    }

    while (next != $EOF && tail == oldTail) {
        next = bigHeaderSwitch(next);
    }

    return next;
}

Int AbstractScanner::bigHeaderSwitch(Int next) {
    if (next != $SLASH) return bigSwitch(next);
}

Int AbstractScanner::bigSwitch(Int next) {
    beginToken(); // A new token starts at `getStringOffset()` | `tokenStart`
    if (next == $SPACE || next == $TAB || next == $CR) {
        return skipSpaces();
    }

    // The character cannot be determined
    next = currentAsUnicode(next);

    return unexpected(next);
}

Int AbstractScanner::unexpected(Int character) {
    auto* errorToken = buildUnexpectedCharacterToken(character, tokenStart);

    if (dynamic_cast<NonAsciiIdentifierToken*>(errorToken)) {
        
    }
}

} // namespace scanner
} // namespace front_end
} // namespace dart