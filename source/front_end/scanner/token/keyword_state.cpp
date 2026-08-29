#include "keyword_state.hpp"
#include "characters.hpp"
#include "keywords.hpp"

#include "common/errors.hpp"
#include "token/keyword.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace keyword {
namespace {
using namespace token::chars;
}

KeywordState::KeywordState() {
    // TODO: Consider converting `ALL_KEYWORDS` to a static extent `std::span`
    constexpr std::size_t length = 297 * BLOCK_SIZE;
    table.resize(length);

    auto nextEmpty = 2 * BLOCK_SIZE;
    for (std::size_t i = 0; i < token::ALL_KEYWORDS.size(); ++i) {
        auto lexeme = token::ALL_KEYWORDS[i]->lexeme;

        auto offset = BLOCK_SIZE;

        for (std::size_t j = 0; j < lexeme.length(); ++j) {
            dart::u8 charOffset = lexeme[j] - $A;
            auto& link = table[offset + 1 + charOffset];
            if (link == 0) { // New one
                link = nextEmpty;
                offset = nextEmpty;
                nextEmpty += BLOCK_SIZE;
            } else { // Existing one.
                offset = link;
            }
        }

        table[offset + 0] = i + 1;
    }

    assert::assert(nextEmpty == table.size());
}

const token::keyword::Keyword* KeywordState::keyword() const {
    auto indexPlusOne = table[currentOffset];
    if (indexPlusOne == 0) return nullptr;

    return token::ALL_KEYWORDS[indexPlusOne - 1];
}

void KeywordState::next(Int next) {
    currentOffset = table[currentOffset + next - $A + 1];
}
} // namespace keyword
} // namespace scanner
} // namespace front_end
} // namespace dart