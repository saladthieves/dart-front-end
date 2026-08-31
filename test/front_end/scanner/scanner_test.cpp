#include "scanner_base.hpp"

#include "scanner/utf8_bytes_scanner.hpp"

#include <gtest/gtest.h>

TEST_F(ScannerBase, initScanner) {
    // ARRANGE
    auto scanner = getScanner("");

    // ACT

    // ASSERT
    ASSERT_TRUE(scanner.tokens->isEof);
    ASSERT_TRUE(scanner.tail->isEof);
    ASSERT_TRUE(scanner.errorTail->isEof);
}

// TODO: Test token start and end of file in entire chain
// TODO: Test keywords
// TODO: Test identifier tokenization (without $)
// TODO: Test identifier tokenization (with $)
// TODO: Test unexpected
// TODO: Test LineStarts

// TODO: Add test for single error token
// TODO: Add test for multiple error tokens in a stream
// TODO: Add test for synthetic token insertions

// TODO: Test error tokens and ordering (prepending)
// TODO: Test that errors are added at the start
// TODO: Test that successful tokenization has no errors
