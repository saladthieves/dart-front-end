#pragma once

#include "common/types.hpp"
#include "scanner.hpp"
#include "token/keyword.hpp"

#include <vector>

namespace dart {
namespace front_end {
namespace scanner {
namespace keyword {

// TODO: Add docs
class KeywordState {
    using Int = Scanner<>::Int;
    static constexpr std::size_t BLOCK_SIZE{59};

public:
    explicit KeywordState();

    void reset() { currentOffset = BLOCK_SIZE; }

    bool isNull() const { return currentOffset == 0; }

    const token::keyword::Keyword* keyword() const;

    void next(Int next);

    std::vector<dart::u16> table{};
    std::size_t currentOffset{0};
};
} // namespace keyword
} // namespace scanner
} // namespace front_end
} // namespace dart