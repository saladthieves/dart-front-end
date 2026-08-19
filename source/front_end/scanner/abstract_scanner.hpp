#pragma once

#include "scanner.hpp"

#include "token/token_factory.hpp"
#include "token/begin_token.hpp"
#include "token/comment_token.hpp"
#include "util/link.hpp"

#include <cstddef>
#include <functional>

namespace dart {
namespace front_end {
namespace scanner {

/*
Callback definition for a language version change.
*/
using LanguageVersionChanged = std::function<void(
    const Scanner<>* scanner, //
    const token::LanguageVersionToken* languageVersion
)>;

/*
Abstract implementation of a scanner.

All other scanner types inherit from this class.
*/
class AbstractScanner : public Scanner<> {
    AbstractScanner(
        const ScannerConfiguration* config,
        bool includeComments,
        const LanguageVersionChanged* languageVersionChanged,
        token::Token* newEofToken, // TODO: Remember to delete token from heap
        std::size_t numberOfBytesHint,
        bool allowLazyStrings = true
    )
        : lineStarts{new LineStarts(numberOfBytesHint)},
          inRecoveryOption{false},
          tokens{newEofToken},
          tail{newEofToken},
          errorTail{newEofToken},
          includeComments{includeComments},
          languageVersionChanged{languageVersionChanged},
          allowLazyStrings{allowLazyStrings} {
        setConfiguration(config);
    }

public:
    explicit AbstractScanner(
        const ScannerConfiguration* config,
        bool includeComments,
        const LanguageVersionChanged* languageVersionChanged,
        std::size_t numberOfBytesHint,
        bool allowLazyStrings = true
    )
        : AbstractScanner(
              config,
              includeComments,
              languageVersionChanged,
              token::TokenFactory::eof(0), // TODO: Check for Dart passes in -1
              numberOfBytesHint,
              allowLazyStrings
          ) {}

    virtual ~AbstractScanner() override {
        delete lineStarts; //
    }

    virtual const LineStarts* getLineStarts() const override {
        return lineStarts;
    }

    virtual bool hasErrors() const override { return _hasErrors; }

    virtual void setConfiguration(const ScannerConfiguration* config) override;

    /*
    Returns `true` when at EOF, `false` otherwise.
    */
    virtual bool atEndOfFile() const = 0;

    /*
    Returns the current scan offset.

    For a string scanner, this is the offset into the source string. For a UTF8
    byte scanner, this is the offset into the byte list.
    */
    virtual std::size_t getScanOffset() const = 0;

    /*
    Returns the current string offset.

    For a string scanner, this is identical to `getScanOffset()`. For a UTF8
    scanner, this is computed based on the UTF8 characters encountered so far.
    */
    virtual std::size_t getStringOffset() const = 0;

    /*
    Sets the `tokenStart` value to the current string offset.

    This effectively notifies that a new token begins at the current offset.
    */
    void beginToken() { tokenStart = getStringOffset(); }

    /*
    Skip past all spaces and returns the latest character that's not a space
    character.
    */
    virtual Int skipSpaces() = 0;

    virtual const token::Token* tokenize() override;

    // TODO: Add docs
    Int bigHeaderSwitch(Int next);

    // TODO: Add docs
    Int bigSwitch(Int next);

    // TODO: Add docs
    Int unexpected(Int character);

    // TODO: Add docs
    virtual Int currentAsUnicode(Int next) = 0;

    /*
    A flag indicating whether character sequences '&&=' and '||=' should be
    tokenized as assignment operators `AMPERSAND_AMPERSAND_EQ_TOKEN` and
    `BAR_BAR_EQ_TOKEN`, respectively.
    */
    static inline bool LAZY_ASSIGNMENT_ENABLED{false};

    /*
    Whether to include comments in the tokenization process (`true`) or not
    (`false`).
    */
    bool includeComments;

    /*
    Callback invoked when the scanner detects a language version comment (such
    as '@dart = 2.1') in the source code, so that the listener can update the
    configuration based on the specified language version.
    */
    const LanguageVersionChanged* languageVersionChanged{nullptr};

    /*
    The string offset for the next token that will be created.

    Depending on the type of scanner, `stringOffset` and `scanOffset` can mean
    different things, especially since one string character can be encoded with
    multiple UTF8 bytes.
    */
    std::size_t tokenStart;

    /*
    A pointer to the token of streams created by the scanner as it tokenizes the
    data. This first token is internal / specific to the scanner, and is neither
    exposed to clients nor from the source code.

    The actual first token from the source code can be obtained via the
    `getFirstToken()` call.
    */
    token::Token* tokens{nullptr};

    /*
    A pointer to the most recently scanned token.
    */
    token::Token* tail{nullptr};

    /*
    A pointer to the most recently prepended error token.
    */
    // TODO: Update docs once we know more about where error tokens are added.
    token::Token* errorTail{nullptr};

    token::Token* openBraceWithMissingEndForPossibleRecovery{nullptr};

    std::size_t offsetForCurlyBracketRecoveryStart;

    /*
    A pointer to the stream of comment tokens created by this scanner, before
    they are assigned to the `SimpleToken::precedingComments` field of a
    non-comment token (that follows the comments).

    If there are no comment tokens, this field is `nullptr`.
    */
    token::CommentToken* comments{nullptr};

    /*
    A pointer to the most recently scanned comment token, or `nullptr` if none.
    */
    token::Token* commentsTail{nullptr};

    // TODO: Add docs once we know how this works.
    const util::Link<token::BeginToken> groupingStack{};

    bool inRecoveryOption;

    std::size_t recoveryCount{0};

    bool allowLazyStrings;

private:
    // TODO: Add docs
    Int scanHeaderLookingForLanguageVersion(Int next);

    bool enableTripleShift{true};
    bool enableAugmentations{false};
    bool _hasErrors{false};
    LineStarts* lineStarts;
};

} // namespace scanner
} // namespace front_end
} // namespace dart