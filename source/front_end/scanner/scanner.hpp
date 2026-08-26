#pragma once

#include "common/types.hpp"
#include "token/comment_token.hpp"
#include "token/token.hpp"

#include <functional>
#include <span>
#include <vector>

namespace dart {
namespace front_end {
namespace scanner {
/*
Stores the line starting position offsets in the source code.
For example, given the following source code:
    `// @dart = 3.10`
    `var name = "Jack";`
    `int code = 007;`

A `LineStarts` will contain the following position offsets (from the start) to
represent where each line starts. Each offset is on the first character of the
line:
    `0`  - for the first `/` on the first line.
    `16` - for the `v` on the second line.
    `35` - for the `i` on the third line.

A `Scanner` inserts a new line after the last one, making the entry at `51`
*/
class LineStarts {
public:
    /*
    The Dart implementation of `LineStarts` internally uses either a 16-bit
    (Uint16List) or a 32-bit (Uint32List) array to store the the line starting
    offset values added via the `add()` function.

    The implementation starts with the 16-bit array to store the values. If an
    offset value that cannot fit in 16 bits (larger than 2^16 / 65535) is added,
    the array is switched to a 32-bit array and the value is then added.

    This C++ implementation just uses a 32-bit vector.
    */
    using Int = dart::u32;

    explicit LineStarts(std::size_t numberOfBytesHint)
        : array{createInitialArray(numberOfBytesHint)} {
        // The first line starts at character offset 0.
        add(0);
    }

    // TODO: Disable copying of class

    void add(Int value) { array.push_back(value); }

    std::vector<Int> array;

private:
    /*
    Creates and returns an array used to store the line starting offset values.

    The `numberOfBytesHint` value is used to estimate the array size, which is
    then preallocated in the array.
    */
    static std::vector<Int> createInitialArray(std::size_t numberOfBytesHint);
};

/*
Represents the scanner's result.
*/
class ScannerResult {
public:
    explicit ScannerResult(
        const token::Token* tokens, LineStarts lineStarts, bool hasErrors
    )
        : tokens{tokens},
          lineStarts{lineStarts},
          hasErrors{hasErrors} { }

    // TODO: Add member docs
    const token::Token* tokens;
    LineStarts lineStarts;
    bool hasErrors;
};

/*
Configuration regarding which tokens the scanner can produce based on the Dart
language level e.g.: enabling / disabling newer features.
*/
class ScannerConfiguration {
public:
    ScannerConfiguration() = default;

    ScannerConfiguration(bool enableTripleShift, bool enableAugmentations)
        : enableTripleShift{enableTripleShift},
          enableAugmentations{enableAugmentations} { }

    /*
    Experimental flag to enable scanning '>>>' tokens.
    */
    bool enableTripleShift{false};

    /*
    Experimental flag for enabling 'augment' as a built-in identifier.
    */
    bool enableAugmentations{false}; // "I never asked for this."
};

/*
Interface that represents a scanner.

The typename parameter 'Character' represents an individual character that the
scanner works on. This can be a UTF8 byte or UTF16 unit code, depending on the
implementation of the scanner itself.
*/
template <typename Character = dart::u8>
class Scanner {
public:
    using Int = Character;
    virtual ~Scanner() = default;

    /*
    Returns `true` if there was an error during the tokenization process, or
    `false` if not.
    */
    virtual bool hasErrors() const = 0;

    /*
    Sets whether an error occurred during the tokenization process (`true`) or
    not (`false`).
    */
    virtual void setHasErrors(bool value) = 0;

    virtual const LineStarts* getLineStarts() const = 0;

    /*
    Sets the scanner configuration for which tokens to produce.
    */
    virtual void setConfiguration(const ScannerConfiguration* config) = 0;

    /*
    Processes the tokens based on the provided configuration.
    */
    virtual const token::Token* tokenize() = 0;
};

/*
Callback definition for a language version change.
*/
using LanguageVersionChanged = std::function<void(
    const Scanner<>* scanner, const token::LanguageVersionToken* languageVersion
)>;

// TODO: Add docs
void scan(
    std::span<dart::u8> bytes,
    ScannerConfiguration* configuration,
    bool includeComments = false,
    LanguageVersionChanged* languageVersionChanged = nullptr,
    bool allowLazyStrings = true
);

} // namespace scanner
} // namespace front_end
} // namespace dart