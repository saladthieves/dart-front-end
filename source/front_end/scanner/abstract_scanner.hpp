#pragma once

#include "scanner.hpp"

#include "token/begin_token.hpp"
#include "token/comment_token.hpp"
#include "token/error_token.hpp"
#include "token/string_token.hpp"
#include "token/token.hpp"
#include "token/token_factory.hpp"
#include "token/token_impl.hpp"
#include "token/token_type.hpp"
#include "util/link.hpp"

#include <cstddef>
#include <memory>

namespace dart {
namespace front_end {
namespace scanner {

// TODO: Organize function calls

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
          allowLazyStrings{allowLazyStrings},
          groupingStack{new util::Link<token::BeginToken>()} {
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
          ) { }

          /*
    virtual ~AbstractScanner() override {
        // Remove comments
        if (comments != nullptr) {
            token::Token* head = comments;
            while (head != nullptr) {
                auto* next = head->getNext();
                delete head;
                head = next;
            }
        }

        // Remove tokens
        if (tokens != nullptr) {
            token::Token* head = tokens;
            while (head != nullptr) {
                if (head->getPrecedingComments() != nullptr) {
                    token::Token* commentHead = head->getPrecedingComments();
                    while (commentHead != nullptr) {
                        auto* next = commentHead->getNext();
                        delete commentHead;
                        commentHead = next;
                    }
                }
                auto* next = head->getNext();
                delete head;
                head = next;
            }
        }

        // Delete groupingStack
        delete groupingStack;

        // Delete lineStarts
        delete lineStarts;
    }
    */
    
    virtual const LineStarts* getLineStarts() const override {
        return lineStarts;
    }

    virtual bool hasErrors() const override { return _hasErrors; }

    virtual void setHasErrors(bool value) override { _hasErrors = value; }

    virtual void setConfiguration(const ScannerConfiguration* config) override;

    /*
    Returns `true` when at EOF, `false` otherwise.
    */
    // TODO: Update docs after implementation
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

    This effectively notifies that a new token starts at the current offset.
    */
    void beginToken() { tokenStart = getStringOffset(); }

    // TODO: Add docs
    const token::Token* getFirstToken() { return tokens->getNext(); }

    /*
    Skip past all spaces and returns the latest character that's not a space.

    Spaces are strictly denoted by the $SP character, so other whitespace
    characters (such as newline, carriage return) are not considered.
    */
    virtual Int skipSpaces() = 0;

    /*
    Advances the scanner to the next character and then returns that character.
    Depending on the scanner implementation, the character may be a UTF-8 byte
    or a UTF-16 code unit.

    The implementation also ensures that if the scanner is already out of bounds
    (past the end), an `$EOF` is returned.
    */
    virtual Int advance() = 0;

    // TODO: Add docs
    Int advanceAfterError();

    // TODO: Add docs
    virtual Int peek() const = 0;

    /*
    Returns the current character which is also the same one `advance()`
    previously returned.
    */
    virtual Int current() const = 0;

    // TODO: Add docs
    virtual Int currentAsUnicode(Int next) = 0;

    /*
    Scans from the current byte offset advancing until reaching an end-of-line
    (EOL) character or EOF, whichever comes first. An EOL character is either a
    $LF (line feed) or a $CR (carriage return).

    Returns `true` if only ASCII characters were skipped over, `false` if not.
    */
    virtual bool scanUntilLineEnd() = 0;

    // TODO: Add docs once we know how this works.
    virtual void handleUnicode(std::size_t startScanOffset) = 0;

    /*
    Notifies when a line feed character ($LF) or '\n' is encountered in a
    multi-line string or comment.
    */
    void lineFeedInMultiLine() {
        lineStarts->add(getStringOffset() + 1); // +1, new line starts after \n
    }

    virtual const token::Token* tokenize() override;

    // TODO: Add docs
    Int scanHeaderLookingForLanguageVersion(Int next);

    // TODO: Add docs
    Int bigHeaderSwitch(Int next);

    // TODO: Add docs
    Int bigSwitch(Int next);

    // TODO: Add docs
    Int tokenizeSlashOrComment(Int next);

    // TODO: Add docs
    void tokenizeSingleLineCommentAppend(
        bool asciiOnly, std::size_t start, bool isDartDoc
    );

    // TODO: Add docs
    Int tokenizeMultiLineComment(Int next, std::size_t start);

    // TODO: Add docs
    Int tokenizeSingleLineComment(Int next, std::size_t start);

    // TODO: Add docs
    Int
    tokenizeSingleLineCommentRest(Int next, std::size_t start, bool isDartDoc);

    // TODO: Add docs
    Int tokenizeLanguageVersionOrSingleLineComment(Int next);

    // TODO: Add docs
    Int tokenizeRawStringKeywordOrIdentifier(Int next);

    // TODO: Add docs
    Int tokenizeKeywordOrIdentifier(Int next, bool allowDollar);

    // TODO: Add docs
    Int tokenizeString(Int next, std::size_t start, bool isRaw);

    // TODO: Add docs
    Int
    tokenizeMultiLineString(Int quoteChar, std::size_t quoteStart, bool isRaw);

    // TODO: Add docs
    Int tokenizeSingleLineRawString(
        Int next, Int quoteChar, std::size_t quoteStart
    );

    // TODO: Add docs
    Int
    tokenizeSingleLineString(Int next, Int quoteChar, std::size_t quoteStart);

    // TODO: Add docs
    Int tokenizeMultiLineRawString(Int quoteChar, std::size_t quoteStart);

    // TODO: Add docs
    Int tokenizeStringInterpolation(std::size_t start, bool asciiOnly);

    // TODO: Add docs
    virtual token::DartDocToken* createDartDocToken(
        const token::type::TokenType* type,
        std::size_t start,
        bool asciiOnly,
        std::size_t extraOffset = 0
    ) = 0;

    // TODO: Add docs
    virtual token::CommentToken* createCommentToken(
        const token::type::TokenType* type,
        std::size_t start,
        bool asciiOnly,
        std::size_t extraOffset = 0
    ) = 0;

    virtual token::LanguageVersionToken* createLanguageVersionToken(
        std::size_t start, dart::u8 major, dart::u8 minor
    ) = 0;

    // TODO: Add docs
    virtual token::StringToken* createSubstringToken(
        const token::type::TokenType* type,
        std::size_t start,
        bool asciiOnly,
        std::size_t extraOffset,
        bool allowLazy
    ) = 0;

    // TODO: Add docs
    virtual token::StringToken* createSyntheticSubstringToken(
        const token::type::TokenType* type,
        std::size_t start,
        bool asciiOnly,
        std::string_view syntheticChars
    ) = 0;

    // TODO: Add docs
    void appendDartDoc(
        std::size_t start, const token::type::TokenType* type, bool asciiOnly
    );

    // TODO: Add docs
    void appendComment(
        std::size_t start, const token::type::TokenType* type, bool asciiOnly
    );

    // TODO: Add docs
    void appendToCommentStream(token::CommentToken* newComment);

    /*
    Append the given token to the `tail` of the current stream of tokens,
    linking both `tail` and `token` in both directions (`tail` -> `token` and
    `token` -> `tail`). After linking, `token` becomes the new `tail`.
    */
    void appendToken(token::Token* token);

    // TODO: Add docs
    void appendSubstringToken(
        const token::type::TokenType* type,
        std::size_t start,
        bool asciiOnly,
        std::size_t extraOffset = 0
    );

    // TODO: Add docs
    void appendSyntheticSubstringToken(
        const token::type::TokenType* type,
        std::size_t start,
        bool asciiOnly,
        std::string_view syntheticChars
    );

    // TODO: Add docs
    void appendPrecedenceToken(const token::type::TokenType* type) {
        appendToken(token::TokenFactory::simple(type, tokenStart, comments));
    }

    // TODO: Add docs
    void appendEofToken();

    /*
    Prepends the given `errorToken` to the stream of tokens.

    If the most recently appended `tail` and `errorTail` are the same, the error
    token is appended to the stream normally and `errorTail` is updated to point
    to the newly appended `tail`.

    Otherwise, the `errorToken` is inserted between `errorTail->getNext()` and
    `errorTail->getNext()->getPrevious()`, linking to both as follows:

    Before insertion:
    `... [ A ] <-> [ B ] <-> [ errorTail ] <-> [ C ] ...`

    After insertion:
    `... [ A ] <-> [ B ] <-> [ errorTail ] <-> [ errorToken ] <-> [ C ] ...`

    After insertion, `errorToken` becomes the new `errorTail`.
    */
    void prependErrorToken(token::ErrorToken* errorToken);

    // TODO: Add docs
    void unterminatedString(
        Int quoteChar,
        std::size_t quoteStart,
        std::size_t start,
        bool asciiOnly,
        bool isMultiline,
        bool isRaw
    );

    // TODO: Add docs
    void discardOpenLt();

    // TODO: Add docs
    void unmatchedBeginGroup(token::BeginToken* begin);

    const token::type::TokenType*
    closeBraceInfoFor(const token::BeginToken* token) const;

    // TODO: Add docs
    Int unexpected(Int character);

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
    std::size_t tokenStart{0};

    /*
    A pointer to the token of streams created by the scanner as it tokenizes the
    data. This first token is internal / specific to the scanner, and is neither
    exposed to clients nor from the source code.

    The actual first token from the source code can be obtained via the
    `getFirstToken()` call.
    */
    token::Token* tokens{nullptr};

    /*
    A pointer to the most recently scanned token that's also been appended to
    the end of the token stream.

    This may be a regular token, or an error token (in the case there was an
    error).
    */
    token::Token* tail{nullptr};

    /*
    A pointer to the most recently prepended error token in the stream.

    This may or may not be the same as `tail`, depending on the situation. If
    the most recently appended `tail` token is an error, then `errorToken` and
    `tail` are the same.

    If another error token needs to be inserted into the stream, it's inserted
    between `errorTail` and `errorTail->getNext()` such that it links to both.

    See `prependErrorToken()` for more.
    */
    token::Token* errorTail{nullptr};

    token::Token* openBraceWithMissingEndForPossibleRecovery{nullptr};

    std::size_t offsetForCurlyBracketRecoveryStart{0};

    /*
    A pointer to the stream of comment tokens created by this scanner, before
    they are assigned to a non-comment token via the 
    `SimpleToken::precedingComments` field. Once added to a non-comment token,
    this field is set to `nullptr` for more comments.

    If there are no comment tokens (or the field `includeComments` is false), 
    this field is `nullptr`.
    */
    token::CommentToken* comments{nullptr};

    /*
    A pointer to the most recently scanned comment token, or `nullptr` if none.
    */
    token::Token* commentsTail{nullptr};

    // TODO: Add docs once we know how this works.
    util::Link<token::BeginToken>* groupingStack{nullptr};

    bool inRecoveryOption;

    // TODO: Add docs once we know how this works.
    std::size_t recoveryCount{0};

    bool allowLazyStrings;

private:
    bool enableTripleShift{true};
    bool enableAugmentations{false};
    bool _hasErrors{false};
    LineStarts* lineStarts{nullptr};
};

} // namespace scanner
} // namespace front_end
} // namespace dart