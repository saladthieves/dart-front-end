#pragma once

#include "token/token.hpp"
#include "common/types.hpp"

#include <vector>

namespace dart {
namespace front_end {
namespace scanner {

// TODO: Add docs after understanding what this class is used for
class LineStarts {
public:
    using Int = dart::u32;

    explicit LineStarts(std::size_t numberOfBytesHint)
        : array{createInitialArray(numberOfBytesHint)} {
        // The first line starts at character offset 0.
        add(0);
    }

    // TODO: Disable copying of class

    void add(Int value) { array[arrayLength++] = value; }

    std::size_t getLength() const { return arrayLength; }

private:
    std::vector<Int> array;
    std::size_t arrayLength{0};

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
          hasErrors{hasErrors} {}

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
          enableAugmentations{enableAugmentations} {}

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

// TODO: Implement scan()
} // namespace scanner
} // namespace front_end
} // namespace dart