#pragma once

#include "common/types.hpp"
#include "test_util/utils.hpp"
#include "token/token.hpp"
#include "utf8_bytes_scanner.hpp"

#include <gtest/gtest.h>
#include <span>
#include <string_view>
#include <vector>

class ScannerBase : public testing::Test {
    using Token = dart::front_end::scanner::token::Token;
    using Utf8BytesScannner = dart::front_end::scanner::Utf8BytesScanner;

protected:
    virtual void SetUp() override { bytes.clear(); }

    virtual void TearDown() override { }

    Utf8BytesScannner
    getScanner(std::string_view code, bool includeComments = true) {
        return Utf8BytesScannner{
            toBytes(code),
            nullptr,
            includeComments,
        };
    }

    std::span<dart::u8> toBytes(std::string_view code) {
        return test::readToBytes(bytes, code);
    }

    template <typename T>
    static inline constexpr const T* as(const Token* token) {
        return dynamic_cast<const T*>(token);
    }

    std::vector<dart::u8> bytes{};
};