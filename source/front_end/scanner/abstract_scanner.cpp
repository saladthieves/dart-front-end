#include "abstract_scanner.hpp"
#include "internal_utils.hpp"

#include "common/errors.hpp"
#include "messages/diagnostic_codes.hpp"
#include "token/begin_token.hpp"
#include "token/characters.hpp"
#include "token/comment_token.hpp"
#include "token/error_token.hpp"
#include "token/keyword_token.hpp"
#include "token/keywords.hpp"
#include "token/synthetic_token.hpp"
#include "token/token_constants.hpp"
#include "token/token_factory.hpp"
#include "token/token_types.hpp"

#include <format>
#include <stdexcept>

namespace dart {
namespace front_end {
namespace scanner {
namespace {
using namespace token::chars;
using namespace token::constants;
using namespace token::type;

using Int = Scanner<>::Int;

using token::BeginToken;
using token::ErrorToken;
using token::NonAsciiIdentifierToken;
using token::Token;

using token::buildUnexpectedCharacterToken;
} // namespace

// AbstractScanner
Int AbstractScanner::advanceAfterError() {
    if (atEndOfFile()) {
        return $EOF;
    } else {
        return advance(); // Proceed forward after error
    }
}

void AbstractScanner::setConfiguration(const ScannerConfiguration* config) {
    if (config != nullptr) {
        enableTripleShift = config->enableTripleShift;
        enableAugmentations = config->enableAugmentations;
    }
}

const Token* AbstractScanner::tokenize() {
    Int next = scanHeaderLookingForLanguageVersion(current());
    while (next != $EOF) {
        next = bigSwitch(next);
    }

    // Make sure we're at the end of file and append an EOF token.
    assert::assert(atEndOfFile());
    appendEofToken();

    // Pretend there's an empty line at the end of the file
    lineStarts->add(getStringOffset() + 1);

    return getFirstToken();
}

Int AbstractScanner::scanHeaderLookingForLanguageVersion(Int next) {
    if (next == $EOF) return next;

    Token* oldTail = tail;
    next = bigHeaderSwitch(next);
    if (next != $EOF && tail->type->kind == SCRIPT_TOKEN) {
        oldTail = tail;
        next = bigHeaderSwitch(next);
    }

    while (next != $EOF && tail == oldTail) {
        next = bigHeaderSwitch(next);
    }

    return next;
}

Int AbstractScanner::bigHeaderSwitch(Int next) {
    if (next != $SLASH) {
        return bigSwitch(next);
    }

    beginToken();           // A token begins at the current character ('/')
    if (peek() != $SLASH) { // Not a single line comment, but maybe another kind
        return tokenizeSlashOrComment(next);
    } else { // Possibly a language version comment '// @dart = MAJOR.MINOR'
        return tokenizeLanguageVersionOrSingleLineComment(next);
    }
}

Int AbstractScanner::bigSwitch(Int next) {
    beginToken(); // A new token starts at `getStringOffset()` | `tokenStart`

    // Skip space characters
    if (next == $SPACE || next == $TAB || next == $CR) {
        return skipSpaces();
    }

    // A linefeed / newline character (\n). Record its offset in `lineStarts`
    if (next == $LF) {
        lineStarts->add(getStringOffset() + 1); // +1, new line starts after LF
        return skipSpaces(); // Skip any white spaces at the start of line
    }

    /*
    Setting the 0x20 flag ensures that the value of `nextLower` is within the
    ASCII printable range.
    */
    const Int nextLower = next | 0x20;
    if ($a <= nextLower && nextLower <= $z) {
        if (next == $r) { // A possible raw string, keyword or identifier
            return tokenizeRawStringKeywordOrIdentifier(next);
        }
    }

    /******* STOP HERE AND BEGIN TESTING ******/

    // The character cannot be determined
    next = currentAsUnicode(next);

    return unexpected(next);
}

Int AbstractScanner::tokenizeSlashOrComment(Int next) {
    std::size_t start = getScanOffset(); // Current position of `next` or '/'

    next = advance();                    // Move past the starting '/'
    if (next == $STAR) {                 // A multiline comment /* ..... */
        return tokenizeMultiLineComment(next, start);
    } else if (next == $SLASH) {         // A single line comment //
        return tokenizeSingleLineComment(next, start);
    } else if (next == $EQ) {            // A compound assignment operator '/='
        appendPrecedenceToken(&token::type::SLASH_EQ);
        return advance();
    } else { // A normal '/' operator
        appendPrecedenceToken(&token::type::SLASH);
        return next;
    }
}

void AbstractScanner::tokenizeSingleLineCommentAppend(
    bool asciiOnly, std::size_t start, bool isDartDoc
) {
    if (!asciiOnly) handleUnicode(start);
    if (isDartDoc) { // A single line '///' Dart doc comment
        appendDartDoc(start, &token::type::SINGLE_LINE_COMMENT, asciiOnly);
    } else {         // A single line '//' normal comment
        appendComment(start, &token::type::SINGLE_LINE_COMMENT, asciiOnly);
    }
}

Int AbstractScanner::tokenizeMultiLineComment(Int next, std::size_t start) {
    bool asciiOnlyComment = true;
    bool asciiOnlyLines = true;
    std::size_t unicodeStart = start;
    dart::u16 nesting = 1;

    next = advance();                     // Move past the '*' in '/*'
    const bool isDartDoc = next == $STAR; // A Dart doc comment with '/**'

    while (true) {
        /*
        If we hit an EOF before the multiline comment is terminated with `✶/`,
        it's an error. This applies to both single and multilevel (nested)
        comments.
        */
        if (next == $EOF) {
            if (!asciiOnlyLines) {
                handleUnicode(unicodeStart);
            }

            // Report the unterminated comment token as an error.
            prependErrorToken(new token::UnterminatedToken(
                messages::codes::diag::unterminatedComment(), tokenStart,
                getStringOffset()
            ));
            advanceAfterError();
            break; // Exit loop
        }

        if (next == $STAR) {  // A '/**' multiline | Dart doc comment.
            next = advance(); // Move past the last '*' in '/**'

            /*
            A '/' immediately after a '*', creates a '✶/' which terminates a
            multiline / Dart doc comment (nested or outer).
            */
            if (next == $SLASH) {
                --nesting;
                if (nesting == 0) { // No more nesting, so handle comment
                    if (!asciiOnlyLines) {
                        handleUnicode(unicodeStart);
                    }
                    next = advance();

                    if (isDartDoc) { // Append a Dart doc ('/**') comment
                        appendDartDoc(
                            start, &token::type::MULTI_LINE_COMMENT,
                            asciiOnlyComment
                        );
                    } else { // Append a normal multiline comment
                        appendComment(
                            start, &token::type::MULTI_LINE_COMMENT,
                            asciiOnlyComment
                        );
                    }
                    break; // Exit loop
                } else {
                    next = advance();
                }
            }
        } else if (next == $SLASH) { // Possibly starting a new nested comment.
            next = advance();
            if (next == $STAR) {     // It's a nested comment
                ++nesting;
                next = advance();
            }
        } else if (next == $LF) { // Newline '\n' inside a multiline comment.
            if (!asciiOnlyLines) {
                // Synchronize the string offset in the UTF scanner.
                handleUnicode(unicodeStart);
                asciiOnlyLines = true;
                unicodeStart = getScanOffset();
            }
            lineFeedInMultiLine(); // Notify `LineStarts` accordingly.
            next = advance();
        } else {
            if (next > 127) { // Extended ASCII code
                asciiOnlyLines = false;
                asciiOnlyComment = false;
            }
            next = advance();
        }
    }

    return next;
}

Int AbstractScanner::tokenizeSingleLineComment(Int next, std::size_t start) {
    next = advance();                      // Move past the last '/' in '//'
    const bool isDartDoc = next == $SLASH; // A Dart doc comment with '///'
    return tokenizeSingleLineCommentRest(next, start, isDartDoc);
}

Int AbstractScanner::tokenizeSingleLineCommentRest(
    Int next, std::size_t start, bool isDartDoc
) {
    bool asciiOnly = true;
    if (next > 127) {
        asciiOnly = false;
    }
    if (next == $LF || next == $CR || next == $EOF) {
        tokenizeSingleLineCommentAppend(asciiOnly, start, isDartDoc);
        return next;
    }

    asciiOnly &= scanUntilLineEnd();
    tokenizeSingleLineCommentAppend(asciiOnly, start, isDartDoc);

    return current();
}

Int AbstractScanner::tokenizeLanguageVersionOrSingleLineComment(Int next) {
    assert::assert(next == $SLASH); // Ensure we're on the first '/'

    std::size_t start = getScanOffset();
    next = advance();               // Move after the first '/'
    assert::assert(next == $SLASH); // Ensure we're on the second '/'

    /*
    If the next char is a '/', it's a single line Dart doc comment ('///')
    */
    if (peek() == $SLASH) {
        return tokenizeSingleLineComment(next, start);
    }

    // Move past the second '/' and skip any whitespace
    next = advance();
    while (next == $SPACE) next = advance();

    /*
    Attempt to parse '@dart'.

    Start with the '@' symbol first.
    */
    if (next != $AT) { // Just a normal single line comment, so scan it as one
        return tokenizeSingleLineCommentRest(next, start, false);
    }

    next = advance();
    if (next != $d) { // Just a single line comment starting with '// @'
        return tokenizeSingleLineCommentRest(next, start, false);
    }

    next = advance();
    if (next != $a) { // Just a single line comment starting with '// @d'
        return tokenizeSingleLineCommentRest(next, start, false);
    }

    next = advance();
    if (next != $r) { // Just a single line comment starting with '// @da'
        return tokenizeSingleLineCommentRest(next, start, false);
    }

    next = advance();
    if (next != $t) { // Just a single line comment starting with '// @dar'
        return tokenizeSingleLineCommentRest(next, start, false);
    }

    // '// @dart' found. Skip whitespace
    next = advance();
    while (next == $SPACE) next = advance();

    // Attempt for '='
    if (next != $EQ) { // Still a comment starting with '// @dart'
        return tokenizeSingleLineCommentRest(next, start, false);
    }

    // Attempt to parse the version, starting with the major.
    next = advance();
    while (next == $SPACE) next = advance();

    dart::u8 major{0}; // Dart can never reach beyond v255 but who knows?
    const auto majorStart = getScanOffset();

    while (isDigit(next)) { // Parse major and stop at a non-digit character
        major = (major * 10) + (next - $0);
        next = advance();
    }

    /*
    If `majorStart` is still the same as the current offset, then major was not
    parsed meaning it's probably still a single line comment.
    */
    if (majorStart == getScanOffset()) {
        return tokenizeSingleLineCommentRest(next, start, false);
    }

    // If a period '.' isn't after major, it's probably a comment.
    if (next != $PERIOD) {
        return tokenizeSingleLineCommentRest(next, start, false);
    }
    next = advance();

    // Parse for the minor version
    dart::u8 minor{0};
    const auto minorStart = getScanOffset();
    while (isDigit(next)) {
        minor = (minor * 10) + (next - $0);
        next = advance();
    }
    if (minorStart == getScanOffset()) { // Probably still a comment
        return tokenizeSingleLineCommentRest(next, start, false);
    }

    // Skip trailing spaces.
    while (next == $SPACE) next = advance();

    /*
    If some other non-whitespace character follows the version on the same line,
    it's not a valid version override comment so classify it as a comment.
    */
    if (next != $LF && next != $CR && next != $EOF) {
        return tokenizeSingleLineCommentRest(next, start, false);
    }

    auto* versionToken = createLanguageVersionToken(start, major, minor);

    // Notify the language version changed listener (if any) of the change
    if (languageVersionChanged != nullptr) {
        (*languageVersionChanged)(this, versionToken);
    }

    if (includeComments) {
        appendToCommentStream(versionToken);
    } else {
        delete versionToken;
    }

    return next;
}

Int AbstractScanner::tokenizeRawStringKeywordOrIdentifier(Int next) {
    /*
    `next` is $r (or 'r'), which could be the start of a raw string, a keyword
    (`return` etc) or an identifier.

    Peek at the very next character to decide.
    */
    const Int afterNext = peek();

    if (afterNext == $SQ || afterNext == $DQ) { // It's a raw string
        const std::size_t start = getScanOffset();
        next = advance();
        return tokenizeString(next, start, true);
    }

    return tokenizeKeywordOrIdentifier(next, true);
}

Int AbstractScanner::tokenizeKeywordOrIdentifier(Int next, bool allowDollar) {
    keywordState.reset();
    const auto start = getScanOffset();

    /*
    Initialize the keyword state with the `next` character, allowing for
    uppercase letters. This has the effect of positioning keyword state on the
    first letter of the first possible keyword.
    */
    if ($A <= next && next <= $z) {
        keywordState.next(next);
        next = advance();
    }

    /*
    Keep iterating over each character as long as:
        a) It's in the correct sequence of the letters of a possible keyword.
           AND
        b) It's a lowercase letter.
    */
    while (!keywordState.isNull() && $a <= next && next <= $z) {
        keywordState.next(next);
        next = advance();
    }

    /*
    `keywordState.isNull()` returned `true` first ((a) from above).

    This happens if a new character (not part of the letters of a possible
    keyword) is encountered.

    For example, the string `abstracted` would cause the call to
    `keywordState.isNull()` to return `true` after reaching `e`, since that
    character is the first one encountered not part of the sequence forming a
    valid keyword. So it can only be an identifier.
    */
    if (keywordState.isNull()) {
        return tokenizeIdentifier(next, start, allowDollar);
    }

    const auto* keyword = keywordState.keyword();
    if (keyword == nullptr) { // Still an identifier, not a keyword.
        return tokenizeIdentifier(next, start, allowDollar);
    }

    /*
    If the augmentation feature is disabled, then treat the `augment` keyword as
    an identifier.
    */
    if (!enableAugmentations && keyword == &token::keyword::AUGMENT) {
        return tokenizeIdentifier(next, start, allowDollar);
    }

    /*
    If the character immediately after the keyword characters is an uppercase
    letter, a numeric digit, an underscore, or a $ sign (if allowed), then it's
    an identifier since a keyword can't be any of those things.
    */
    if (($A <= next && next <= $Z) || ($0 <= next && next <= $9) ||
        next == $_ || (allowDollar && next == $$)) {
        return tokenizeIdentifier(next, start, allowDollar);
    } else {
        appendKeywordToken(keyword);
        return next;
    }
}

Int AbstractScanner::tokenizeIdentifier(
    Int next, std::size_t start, bool allowDollar
) {
    if (allowDollar) {
        /*
        Since identifiers can contain a $ sign, the normal case is to allow it.
        */
        if (internal_utils::isIdentifierChar(next, true)) {
            next = passIdentifierCharAllowDollar();
            appendSubstringToken(&token::type::IDENTIFIER, start, true);
        } else { // Identifier ends here
            if (start == getScanOffset()) {
                return unexpected(next);
            } else {
                appendSubstringToken(&token::type::IDENTIFIER, start, true);
            }
        }
    } else {
        while (true) {
            if (internal_utils::isIdentifierChar(next, false)) {
                next = advance();
            } else { // Identifier ends here.
                if (start == getScanOffset()) {
                    return unexpected(next);
                } else {
                    appendSubstringToken(
                        &token::type::IDENTIFIER, start, false
                    );
                }
                break;
            }
        }
    }

    return next;
}

Int AbstractScanner::tokenizeString(Int next, std::size_t start, bool isRaw) {
    const Int quoteChar = next;  // The first ' or " after r (if any).
    next = advance();            // Advance to next character
    if (next == quoteChar) {     // "" or ''
        next = advance();        // Advance once more to next character
        if (next == quoteChar) { // 3 """ or ''' chars - a multiline string.
            return tokenizeMultiLineString(quoteChar, start, isRaw);
        } else {                 // "" or '' - an empty string.
            appendSubstringToken(&token::type::STRING, start, true);
            return next;
        }
    }

    if (isRaw) { // r"" or r'' - a single line raw string.
        return tokenizeSingleLineRawString(next, quoteChar, start);
    } else {     // "" or '' - a normal string.
        return tokenizeSingleLineString(next, quoteChar, start);
    }
}

Int AbstractScanner::tokenizeMultiLineString(
    Int quoteChar, std::size_t quoteStart, bool isRaw
) {
    if (isRaw) return tokenizeMultiLineRawString(quoteChar, quoteStart);

    std::size_t start = quoteStart;

    // Assume only ASCII characters.
    bool asciiOnlyString = true;
    bool asciiOnlyLine = true;

    std::size_t unicodeStart = quoteStart;
    Int next = advance();  // Move past the last ' or " of the three.

    while (next != $EOF) { // We haven't hit the end.
        if (next == $$) {
            if (!asciiOnlyLine) {
                handleUnicode(unicodeStart);
            }
            next = tokenizeStringInterpolation(start, asciiOnlyString);
            start = getScanOffset();
            unicodeStart = start;
            asciiOnlyString = true; // A new string token is created
            asciiOnlyLine = true;
            continue;
        }

        if (next == quoteChar) {     // Maybe closing quotes, so check further
            next = advance();
            if (next == quoteChar) { // Second quote, check further
                next = advance();
                if (next == quoteChar) { // Third quote, closing the string
                    if (!asciiOnlyLine) {
                        handleUnicode(unicodeStart);
                    }

                    // Append the string token and return
                    next = advance();
                    appendSubstringToken(
                        &token::type::STRING, start, asciiOnlyString
                    );
                    return next;
                }
            }
            continue; // No second or third quotes, so proceed normally
        }

        if (next == $BACKSLASH) { // Preserve backslashes
            next = advance();
            if (next == $EOF) break;
        }

        if (next == $LF) {
            if (!asciiOnlyLine) {
                // Synchronize the string offset in the UTF8 scanner.
                handleUnicode(unicodeStart);
                asciiOnlyLine = true;
                unicodeStart = getScanOffset();
            }
            lineFeedInMultiLine(); // Notify LineStarts accordingly
        } else if (next > 127) {   // Extended ASCII code
            asciiOnlyString = false;
            asciiOnlyLine = false;
        }

        next = advance();
    }

    if (!asciiOnlyLine) {
        handleUnicode(unicodeStart);
    }

    /*
    An EOF was hit before the 3 closing quotation marks of the multiline string,
    so this is an unterminated string.
    */
    unterminatedString(
        quoteChar, quoteStart, start, asciiOnlyString,
        true, // isMultiline
        false // isRaw
    );

    return next;
}

Int AbstractScanner::tokenizeSingleLineRawString(
    Int next,              // The char after quoteChar
    Int quoteChar,         // Either ' or "
    std::size_t quoteStart // Position of 'r'
) {
    bool asciiOnly = true;
    while (next != $EOF) {
        // We hit the closing quoteChar of the string.
        if (next == quoteChar) {
            if (!asciiOnly) {
                handleUnicode(quoteStart);
            }
            next = advance();
            appendSubstringToken(&token::type::STRING, quoteStart, asciiOnly);
            return next;
        }

        /*
        We hit a newline '\n' or carriage return before reaching the closing
        quotes of the string. This is only allowed in multiline strings, so this
        string is not terminated.
        */
        if (next == $LF || next == $CR) {
            if (!asciiOnly) {
                handleUnicode(quoteStart);
            }
            unterminatedString(
                quoteChar, quoteStart, quoteStart, asciiOnly,
                false, // isMultiline
                true   // isRaw
            );
            return next;
        }

        if (next > 127) { // Extended ASCII code
            asciiOnly = false;
        }

        next = advance();
    }

    if (!asciiOnly) {
        handleUnicode(quoteStart);
    }

    // We hit an EOF in the `while` loop before the string could terminate.
    unterminatedString(
        quoteChar, quoteStart, quoteStart, asciiOnly,
        false, // isMultiline
        true   // isRaw
    );

    return next;
}

Int AbstractScanner::tokenizeSingleLineString(
    Int next, Int quoteChar, std::size_t quoteStart
) {
    std::size_t start = quoteStart;
    bool asciiOnly = true;

    while (next != quoteChar) {
        if (next == $BACKSLASH) { // Preserve the backlash and proceed.
            next = advance();
        } else if (next == $$) {  // Ignore string interpolation (for now)
            if (!asciiOnly) {
                handleUnicode(start);
            }
            next = tokenizeStringInterpolation(start, asciiOnly);
            start = getScanOffset();
            asciiOnly = true;
            continue;
        }

        // Reaching any of these before `quoteStart` is an unterminated string
        if (next <= $CR && (next == $LF || next == $CR || next == $EOF)) {
            if (!asciiOnly) {
                handleUnicode(start);
            }

            unterminatedString(
                quoteChar, quoteStart, start, asciiOnly,
                false, // isMultiline
                false  // isRaw
            );
            return next;
        }

        if (next > 127) { // Extended ASCII code
            asciiOnly = false;
        }
        next = advance();
    }

    if (!asciiOnly) {
        handleUnicode(start);
    }

    /*
    We've hit the closing quote character of the string, so move past it and
    append it.
    */
    next = advance();
    appendSubstringToken(&token::type::STRING, start, asciiOnly);

    return next;
}

Int AbstractScanner::tokenizeMultiLineRawString(
    Int quoteChar, std::size_t quoteStart
) {
    // Assume only ASCII characters.
    bool asciiOnlyString = true;
    bool asciiOnlyLine = true;

    std::size_t unicodeStart = quoteStart;
    Int next = advance(); // Move past the last ' or " of the three.
    bool success = true;

    const auto loop = [&] -> Int {
        while (next != $EOF) {          // We're not yet at the end.
            while (next != quoteChar) { // And still inside the string
                if (next == $LF) {      // Newline '\n' character
                    if (!asciiOnlyLine) {
                        handleUnicode(unicodeStart);
                        asciiOnlyLine = true;
                        unicodeStart = getScanOffset();
                    }
                    lineFeedInMultiLine();
                } else if (next > 127) { // Extended ASCII code
                    asciiOnlyString = false;
                    asciiOnlyLine = false;
                }
                next = advance();
                if (next == $EOF) { // Exit the lambda
                    success = false;
                    return $EOF;
                }
            }
            // On first closing quotation mark of the string

            next = advance();     // Move to second quotation mark
            if (next == quoteChar) {
                next = advance(); // Move to third quotation mark
                if (next == quoteChar) {
                    if (!asciiOnlyLine) handleUnicode(unicodeStart);

                    next = advance(); // Move past last quotation mark

                    // Append the string and return
                    appendSubstringToken(
                        &token::type::STRING, quoteStart, asciiOnlyString
                    );
                    return next;
                }
            }
        }

        success = false;
        return next;
    };

    next = loop();
    if (success) return next;

    if (!asciiOnlyLine) {
        handleUnicode(unicodeStart);
    }

    // We reached EOF before the string was terminated, so report it
    unterminatedString(
        quoteChar, quoteStart, quoteStart, asciiOnlyLine,
        true, // isMultiline
        true  // isRaw
    );
    return next;
}

Int AbstractScanner::tokenizeStringInterpolation(
    std::size_t start, bool asciiOnly
) {
    // TODO: Figure out what's going on here and add it to function docs
    appendSubstringToken(&token::type::STRING, start, asciiOnly);
    beginToken();                      // Mark that the '$' starts here.
    Int next = advance();
    if (next == $OPEN_CURLY_BRACKET) { // A '${ }' interpolation expression
        return tokenizeInterpolatedExpression(next);
    } else {                           // An '$IDENTIFIER' expression
        return tokenizeInterpolatedIdentifier(next);
    }
}

Int AbstractScanner::tokenizeInterpolatedExpression(Int next) { // next = '{'
    appendBeginGroup(&token::type::STRING_INTERPOLATION_EXPRESSION);
    if (tokenStart == offsetForCurlyBracketRecoveryStart) {
        // TODO: Update explanation once we understand what this does
        discardInterpolation();
        return advance();
    }

    beginToken();     // The expression starts here (on the '{')
    next = advance(); // Move past the '{'
    while (next != $EOF && next != $STX) { // TODO: Investigate $STX
        next = bigSwitch(next);
    }

    if (next == $EOF) {
        beginToken();
        discardInterpolation();
        return next;
    }

    next = advance(); // Move past the $STX
    beginToken();     // The string interpolation suffix begins here.
    return next;
}

Int AbstractScanner::tokenizeInterpolatedIdentifier(Int next) {
    appendPrecedenceToken(&token::type::STRING_INTERPOLATION_IDENTIFIER);

    // TODO: Move to external function
    if ($a <= next && next <= $z || $A <= next && next <= $Z || next == $_) {
        beginToken(); // Identifier starts here

        /*
        An interpolation identifier can only contain one '$' (at the start), so
        pass in `false` for `allowDollar`.
        */
        next = tokenizeKeywordOrIdentifier(next, false);
    } else {
        beginToken(); // The synthetic identifier starts here.
        appendSyntheticSubstringToken(
            &token::type::IDENTIFIER, getScanOffset(), true, ""
        );
        prependErrorToken(new token::UnterminatedToken{
            messages::codes::diag::unexpectedDollarInString(), tokenStart,
            getStringOffset()
        });
    }

    beginToken(); // The string interpolation suffix starts here
    return next;
}

void AbstractScanner::appendBeginGroup(const token::type::TokenType* type) {
    auto* token = new BeginToken{
        type,
        tokenStart,
        type->lexeme.length(),
        comments,
    };
    appendToken(token);

    /*
    The '{', '[', and '${' tokens cannot appear inside type parameters or
    arguments.
    */
    if (type != &token::type::LT && type != &token::type::OPEN_PAREN) {
        discardOpenLt();
    }
    groupingStack = groupingStack->prepend(token);
}

void AbstractScanner::appendDartDoc(
    std::size_t start, const token::type::TokenType* type, bool asciiOnly
) {
    if (!includeComments) return;

    auto* newComment = createDartDocToken(type, start, asciiOnly);
    appendToCommentStream(newComment);
}

void AbstractScanner::appendComment(
    std::size_t start, const token::type::TokenType* type, bool asciiOnly
) {
    if (!includeComments) return;

    auto* newComment = createCommentToken(type, start, asciiOnly);
    appendToCommentStream(newComment);
}

void AbstractScanner::appendToCommentStream(token::CommentToken* newComment) {
    if (comments == nullptr) {
        // Comments stream is empty, so the new comment is both head and tail.
        comments = newComment;
        commentsTail = comments;
    } else {
        // Link `newComment` and `commentsTail` to each other.
        commentsTail->setNext(newComment);
        commentsTail->next()->setPrev(commentsTail);

        // Make `newComment` the new tail
        commentsTail = commentsTail->next();
    }
}

void AbstractScanner::appendToken(Token* token) {
    tail->setNext(token); // connect tail -> token
    token->setPrev(tail); // connect token -> tail
    tail = token;         // make token the new tail

    /*
    The current `comments` stream already belongs to the `token` being appended
    to the stream aka the token already has it in its `_precedingComment` field,
    so remove it from the scanner.
    */
    if (comments != nullptr && comments == token->precedingComment()) {
        comments = nullptr;
        commentsTail = nullptr;
    } else {
        /*
        The caller is responsible for creating the token object being appended,
        along with any preceding comments (if any).
        */
        assert::assert(
            comments == nullptr || //
            token->isSynthetic ||  //
            (dynamic_cast<ErrorToken*>(token) != nullptr)
        );
    }
}

void AbstractScanner::appendSubstringToken(
    const token::type::TokenType* type,
    std::size_t start,
    bool asciiOnly,
    std::size_t extraOffset
) {
    appendToken(createSubstringToken(
        type, start, asciiOnly, extraOffset, allowLazyStrings
    ));
}

void AbstractScanner::appendSyntheticSubstringToken(
    const token::type::TokenType* type,
    std::size_t start,
    bool asciiOnly,
    std::string_view syntheticChars
) {
    appendToken(
        createSyntheticSubstringToken(type, start, asciiOnly, syntheticChars)
    );
}

void
AbstractScanner::appendKeywordToken(const token::keyword::Keyword* keyword) {
    // Type parameters and arguments cannot contain 'this'.
    if (keyword->lexeme == "this") {
        discardOpenLt();
    }
    appendToken(new token::KeywordToken{keyword, tokenStart, comments});
}

void AbstractScanner::appendEofToken() {
    beginToken(); // The closing EOF token begins after everything else
    discardOpenLt();

    // TODO: Investigate how the code beneath works
    if (groupingStack->isNotEmpty() &&
        groupingStack->head->isA(&token::type::OPEN_CURLY_BRACKET) &&
        groupingStack->tail->isEmpty()) {
        // Opening '{' without a closing '}'. User could be typing.
        openBraceWithMissingEndForPossibleRecovery = groupingStack->head;
    }

    while (groupingStack->isNotEmpty()) {
        unmatchedBeginGroup(groupingStack->head);
        auto* tail = groupingStack->tail;
        delete groupingStack;
        groupingStack = tail;
    }

    appendToken(token::TokenFactory::eof(tokenStart, comments));
}

void AbstractScanner::prependErrorToken(ErrorToken* errorToken) {
    setHasErrors(true);
    if (errorTail == tail) {     // Both error and tail tokens are the same
        appendToken(errorToken); // so append the token normally
        errorTail = tail;        // and update errorTail to be in sync
    } else {
        /*
        Connect `errorToken` and `errorTail->next()` together [right-hand side]:

            errorToken => errorTail->next()
            errorToken <= errorTail->next()
        */
        errorToken->setNext(errorTail->next());
        errorToken->next()->setPrev(errorToken);

        /*
        Connect `errorTail` and `errorToken` together [left-hand side]:

            errorTail => errorToken
            errorTail <= errorToken
        */
        errorTail->setNext(errorToken);
        errorToken->setPrev(errorTail);

        // Finally update errorTail to errorToken
        errorTail = errorTail->next();
    }
}

void AbstractScanner::unterminatedString(
    Int quoteChar,
    std::size_t quoteStart,
    std::size_t start,
    bool asciiOnly,
    bool isMultiline,
    bool isRaw
) {
    const auto c = static_cast<char>(quoteChar);
    const auto suffix =
        isMultiline ? std::format("{}{}{}", c, c, c) : std::format("{}", c);
    const auto prefix = isRaw ? std::format("r{}", suffix) : suffix;

    appendSyntheticSubstringToken(
        &token::type::STRING, start, asciiOnly, suffix
    );

    // Report the error on a visible token
    const auto offset = getStringOffset();
    std::size_t errorStart = tokenStart < offset ? tokenStart : quoteStart;
    prependErrorToken(new token::UnterminatedString{
        errorStart,
        prefix,
        offset,
    });
}

void AbstractScanner::discardOpenLt() {
    while (groupingStack->isNotEmpty() &&
           groupingStack->head->type == &token::type::LT) {
        auto* tail = groupingStack->tail;
        delete groupingStack;
        groupingStack = tail;
    }
}

void AbstractScanner::discardInterpolation() {
    while (groupingStack->isNotEmpty()) {
        auto* beginToken = groupingStack->head;
        unmatchedBeginGroup(beginToken);
        auto* tail = groupingStack->tail;
        delete groupingStack;
        groupingStack = tail;

        if (beginToken->type == &token::type::STRING_INTERPOLATION_EXPRESSION) {
            break;
        }
    }
}

void AbstractScanner::unmatchedBeginGroup(token::BeginToken* begin) {
    const auto* type = closeBraceInfoFor(begin);
    appendToken(new token::SyntheticToken(type, tokenStart, tail));
    begin->endToken = tail;
    prependErrorToken(new token::UnmatchedToken(begin));
    ++recoveryCount;
}

const TokenType*
AbstractScanner::closeBraceInfoFor(const BeginToken* token) const {
    const auto& lexeme = token->lexeme;
    if (lexeme == "(") return &token::type::CLOSE_PAREN;
    if (lexeme == "[") return &token::type::CLOSE_SQUARE_BRACKET;
    if (lexeme == "{") return &token::type::CLOSE_CURLY_BRACKET;
    if (lexeme == "<") return &token::type::GT;
    if (lexeme == "${") return &token::type::CLOSE_CURLY_BRACKET;

    throw std::logic_error(
        std::format("Unknown BeginToken lexeme: `{}`", lexeme)
    );
}

Int AbstractScanner::unexpected(Int character) {
    // Create an `ErrorToken` to represent the invalid character
    auto* errorToken = buildUnexpectedCharacterToken(character, tokenStart);

    // The error is related to a non-ASCII identifier character
    if (dynamic_cast<NonAsciiIdentifierToken*>(errorToken)) {
        std::size_t beginOffset;
        std::string stringValue;

        if (tail->isA(&token::type::IDENTIFIER) &&
            tokenStart == tail->beginOffset) {
            beginOffset = tail->beginOffset; // Start of tail
            stringValue = tail->lexeme;
            tail = tail->prev();             // TODO: Delete disconnected token
        } else {
            beginOffset = errorToken->beginOffset;
        }

        stringValue += errorToken->character();
        prependErrorToken(errorToken);

        Int next = advanceAfterError();
        while (internal_utils::isIdentifierChar(next, true)) {
            stringValue += next;
            next = advance();
        }

        auto* token = new token::StringToken{
            &token::type::IDENTIFIER, beginOffset, stringValue,
            stringValue.length(), comments
        };
        appendToken(token);

        return next;
    } else {
        // It's some other character error. Add it to the stream and advance.
        prependErrorToken(errorToken);
        return advanceAfterError();
    }
}
} // namespace scanner
} // namespace front_end
} // namespace dart