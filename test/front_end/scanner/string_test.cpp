#include "scanner_base.hpp"

#include "scanner/utf8_bytes_scanner.hpp"
#include "token/error_token.hpp"
#include "token/string_token.hpp"
#include "token/synthetic_token.hpp"

#include <gtest/gtest.h>

using dart::front_end::scanner::token::StringToken;
using dart::front_end::scanner::token::SyntheticStringToken;
using dart::front_end::scanner::token::UnterminatedString;

TEST_F(ScannerBase, tokenizeSingleLineRawString_SQ) {
    // ARRANGE
    constexpr auto code = "r'Single line raw string.'";
    auto scanner = getScanner(code);

    // ACT
    const auto* tokens = scanner.tokenize();

    // ASSERT
    const auto* token = as<StringToken>(tokens);
    ASSERT_NE(token, nullptr);
    ASSERT_STREQ(token->lexeme.c_str(), code);
}

TEST_F(ScannerBase, tokenizeSingleLineRawString_SQ_Unterminated_Newline) {
    // ARRANGE
    constexpr auto code = R"(r'Single line raw string.
    )";
    auto scanner = getScanner(code);

    // ACT
    const auto* tokens = scanner.tokenize();

    // ASSERT
    ASSERT_TRUE(scanner.hasErrors());
    const auto* error = as<UnterminatedString>(tokens);
    ASSERT_NE(error, nullptr);
    ASSERT_TRUE(error->lexeme.contains("String starting with `r'`"));

    const auto* token = as<SyntheticStringToken>(error->next());
    ASSERT_NE(token, nullptr);
    ASSERT_STREQ(token->lexeme.c_str(), "r'Single line raw string.'");
}

TEST_F(ScannerBase, tokenizeSingleLineRawString_SQ_Unterminated) {
    // ARRANGE
    constexpr auto code = "r'Single line raw string.";
    auto scanner = getScanner(code);

    // ACT
    const auto* tokens = scanner.tokenize();

    // ASSERT
    ASSERT_TRUE(scanner.hasErrors());
    const auto* error = as<UnterminatedString>(tokens);
    ASSERT_NE(error, nullptr);
    ASSERT_TRUE(error->lexeme.contains("String starting with `r'`"));

    const auto* token = as<SyntheticStringToken>(error->next());
    ASSERT_NE(token, nullptr);
    ASSERT_STREQ(token->lexeme.c_str(), "r'Single line raw string.'");
}

// TODO: tokenizeSingleLineRawString_SingleQuote_Invalid (unterminated)
// TODO: tokenizeSingleLineRawString_SingleQuote_Invalid (wrong quote ending)

TEST_F(ScannerBase, tokenizeSingleLineRawString_DQ) {
    // ARRANGE
    constexpr auto code = "r\"Single line raw string.\"";
    auto scanner = getScanner(code);

    // ACT
    const auto* tokens = scanner.tokenize();

    // ASSERT
    const auto* token = as<StringToken>(tokens);
    ASSERT_NE(token, nullptr);
    ASSERT_STREQ(token->lexeme.c_str(), code);
}

// TODO: tokenizeSingleLineRawString_DoubleQuote_Invalid (unterminated)
// TODO: tokenizeSingleLineRawString_DoubleQuote_Invalid (wrong quote ending)

// TODO: Test raw string r" "
// TODO: Test raw string r''' '''
// TODO: Test raw string r""" """
// TODO: Test normal string ' '
// TODO: Test normal string " "
// TODO: Test multiline string ''' '''
// TODO: Test multiline string """ """
// TODO: Test normal string with interpolation '$'
// TODO: Test normal string with interpolation "$"
// TODO: Test multiline string with interpolation '''$'''
// TODO: Test multiline string with interpolation """$"""