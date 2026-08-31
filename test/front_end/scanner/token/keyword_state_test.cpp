#include "test_util/utils.hpp"
#include "token/keyword_state.hpp"
#include "token/keywords.hpp"

#include <gtest/gtest.h>

using dart::front_end::scanner::keyword::KeywordState;
using dart::front_end::scanner::token::ALL_KEYWORDS;

TEST(KeywordState, init) {
    // ARRANGE

    // ACT
    auto state = KeywordState{};

    // ASSERT
    ASSERT_EQ(state.currentOffset, 0);
    ASSERT_TRUE(state.isNull());
    ASSERT_EQ(state.keyword(), nullptr);
}

TEST(KeywordState, next) {
    // ARRANGE
    auto state = KeywordState{};

    vector<dart::u8> bytes{};

    for (const auto* keyword : ALL_KEYWORDS) {
        // ACT
        test::readToBytes(bytes, keyword->lexeme);

        state.reset();
        for (const auto& byte : bytes) {
            state.next(byte);
        }

        // ASSERT
        ASSERT_FALSE(state.isNull());
        ASSERT_EQ(state.keyword(), keyword);
    }
}

TEST(KeywordState, nextNull) {
    // ARRANGE
    auto state = KeywordState{};

    vector<dart::u8> bytes{};

    for (const auto* keyword : ALL_KEYWORDS) {
        // ACT
        test::readToBytes(bytes, keyword->lexeme);
        bytes.push_back('a');

        state.reset();
        for (const auto& byte : bytes) {
            state.next(byte);
        }

        // ASSERT
        ASSERT_TRUE(state.isNull());
        ASSERT_EQ(state.keyword(), nullptr);
    }
}