#include "scanner_base.hpp"

#include "scanner/utf8_bytes_scanner.hpp"
#include "token/comment_token.hpp"
#include "token/error_token.hpp"

#include <gtest/gtest.h>
#include <string_view>

using dart::front_end::scanner::token::CommentToken;
using dart::front_end::scanner::token::DartDocToken;
using dart::front_end::scanner::token::LanguageVersionToken;
using dart::front_end::scanner::token::UnterminatedToken;

TEST_F(ScannerBase, init) {
    // ARRANGE
    auto scanner = getScanner("");

    // ACT

    // ASSERT
    ASSERT_TRUE(scanner.tokens->isEof);
    ASSERT_TRUE(scanner.tail->isEof);
    ASSERT_TRUE(scanner.errorTail->isEof);
}

TEST_F(ScannerBase, tokenizeLanguageVersionOrSingleLineComment) {
    // ARRANGE
    constexpr auto code = "// @dart = 3.16";
    auto scanner = getScanner(code);

    // ACT
    const auto* tokens = scanner.tokenize();

    // ASSERT
    const auto* token = as<LanguageVersionToken>(tokens->precedingComment());
    ASSERT_NE(token, nullptr);
    ASSERT_EQ(token->major, 3);
    ASSERT_EQ(token->minor, 16);
    ASSERT_STREQ(token->lexeme.c_str(), code);
}

TEST_F(ScannerBase, tokenizeLanguageVersionOrSingleLineComment_Invalid) {
    // ARRANGE
    const auto codes = vector{
        "//",
        "// @",
        "// @d",
        "// @da",
        "// @dar",
        "// @dart",
        "// @dart = ",
        "// @dart = 3",
        "// @dart = 3.",
        "// @dart = 3.a",
        "// @dart = 3.2a",
    };

    for (const auto code : codes) {
        auto scanner = getScanner(code);

        // ACT
        const auto* tokens = scanner.tokenize();

        // ASSERT
        const auto* token =
            as<LanguageVersionToken>(tokens->precedingComment());
        ASSERT_EQ(token, nullptr);
    }
}

TEST_F(ScannerBase, tokenizeSingleLineComment_Normal) {
    // ARRANGE
    constexpr auto code = "// single line comment.";
    auto scanner = getScanner(code);

    // ACT
    const auto* tokens = scanner.tokenize();

    // ASSERT
    const auto* token = as<CommentToken>(tokens->precedingComment());
    ASSERT_NE(token, nullptr);
    ASSERT_STREQ(token->lexeme.c_str(), code);
}

TEST_F(ScannerBase, tokenizeSingleLineComment_DartDoc) {
    // ARRANGE
    constexpr auto code = "/// Dart doc comment.";
    auto scanner = getScanner(code);

    // ACT
    const auto* tokens = scanner.tokenize();

    // ASSERT
    const auto* token = as<DartDocToken>(tokens->precedingComment());
    ASSERT_NE(token, nullptr);
    ASSERT_STREQ(token->lexeme.c_str(), code);
}

TEST_F(ScannerBase, tokenizeMultilineComment_Normal) {
    // ARRANGE
    constexpr auto code =
        R"(
/*
Multiline.
*/
)";

    auto scanner = getScanner(code);

    // ACT
    const auto* tokens = scanner.tokenize();

    // ASSERT
    const auto* token = as<CommentToken>(tokens->precedingComment());
    ASSERT_NE(token, nullptr);
    ASSERT_STREQ(token->lexeme.c_str(), "/*\nMultiline.\n*/");
}

TEST_F(ScannerBase, tokenizeMultilineComment_Invalid_Unterminated) {
    // ARRANGE
    constexpr auto code = "/* Unterminated comment.";
    auto scanner = getScanner(code);

    // ACT
    scanner.tokenize();

    // ASSERT
    ASSERT_TRUE(scanner.hasErrors());
    const auto* error = as<UnterminatedToken>(scanner.errorTail);
    ASSERT_NE(error, nullptr);
    ASSERT_TRUE(error->lexeme.contains("Comment starting with '/*'"));
}

TEST_F(ScannerBase, tokenizeMultilineComment_Nested) {
    // ARRANGE
    constexpr auto code =
        R"(
/*
Outer begin.
  /*
  Inner.
  */
Outer end.
*/
)";

    auto scanner = getScanner(code);

    // ACT
    const auto* tokens = scanner.tokenize();

    // ASSERT
    const auto* token = as<CommentToken>(tokens->precedingComment());
    ASSERT_NE(token, nullptr);
    ASSERT_STREQ(
        token->lexeme.c_str(),
        "/*\nOuter begin.\n  /*\n  Inner.\n  */\nOuter end.\n*/"
    );
}

TEST_F(ScannerBase, tokenizeMultilineComment_Nested_Invalid_Unterminated) {
    // ARRANGE
    constexpr auto code = "/* Outer /*Inner */";
    auto scanner = getScanner(code);

    // ACT
    scanner.tokenize();

    // ASSERT
    ASSERT_TRUE(scanner.hasErrors());
    const auto* error = as<UnterminatedToken>(scanner.errorTail);
    ASSERT_NE(error, nullptr);
    ASSERT_TRUE(error->lexeme.contains("Comment starting with '/*'"));
}

TEST_F(ScannerBase, tokenizeMultilineComment_DartDoc) {
    // ARRANGE
    constexpr auto code =
        R"(
/**
 * Multiline Dart doc.
 */
)";

    auto scanner = getScanner(code);

    // ACT
    const auto* tokens = scanner.tokenize();

    // ASSERT
    const auto* token = as<DartDocToken>(tokens->precedingComment());
    ASSERT_NE(token, nullptr);
    ASSERT_STREQ(token->lexeme.c_str(), "/**\n * Multiline Dart doc.\n */");
}