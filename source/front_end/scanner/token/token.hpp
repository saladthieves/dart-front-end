#pragma once

#include "keyword.hpp"

#include "base/syntactic_entity.hpp"
#include "token/token_type.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
// Forward declarations
class CommentToken;

/*
An interface representing a single token scanned from the input source text.
Each token keeps track of which token comes before it and which comes after, in
a doubly-linked list like structure.
*/
class Token : public base::SyntacticEntity {
public:
    // TODO: Add factory constructors

    Token() = default;

    // Token types (and sub-types) should not be copyable
    Token(const Token& other) = delete;
    Token& operator=(const Token& other) = delete;

    /*
    Returns the number of characters parsed by this token.
    */
    virtual std::size_t getCharCount() const = 0;

    /*
    Returns the character offset of this token's start from the source text.
    */
    virtual std::size_t getCharOffset() const = 0;

    /*
    Returns the character offset of this token's end from the source text.
    */
    virtual std::size_t getCharEnd() const = 0;

    /*
    Returns the synthetic token before `this` token, or nullptr if this token is
    not a synthetic ')', ']', '}' or '>' token.
    */
    virtual const Token* getBeforeSynthetic() const = 0;

    /*
    Sets the provided token before `this` synthetic ')', ']', '}' or '>' token,
    or ignored otherwise.
    */
    virtual void setBeforeSynthetic(const Token* previous) = 0;

    virtual std::size_t getEnd() const override = 0;

    /*
    Returns the token that corresponds to this one, or `nullptr` if this token
    is not the first in a matching pair group of tokens, such as in parentheses.

    For example, if this token represents a '(' token, 'getEndGroup()' will
    return the corresponding closing ')' token. If this token is a ')' token or
    something else, then 'getEndGroup()' will return `nullptr`.
    */
    virtual const Token* getEndGroup() const { return nullptr; }

    /*
    Returns `true` if this token is an end of file, `false` if not.
    */
    virtual bool isEof() const = 0;

    /*
    Returns `true` if this token is an identifier, `false` otherwise. This may
    also be a keyword (such as `PSEUDO`) depending on its usage context.
    */
    virtual bool isIdentifier() const = 0;

    /*
    Returns `true` if this token is a keyword, `false` otherwise.
    */
    virtual bool isKeyword() const = 0;

    /*
    Returns `true` if this token is a keyword or identifier, `false` otherwise.
    */
    virtual bool isKeywordOrIdentifier() const = 0;

    /*
    Returns `true` if this token is a modifier (such as `const`, `static`,
    `final`, `abstract` etc), or `false` if not.
    */
    virtual bool isModifier() const = 0;

    /*
    Returns `true` if this token represents an operator, `false` otherwise.
    */
    virtual bool isOperator() const = 0;

    /*
    Returns `true` if this token is a synthetic one, `false` otherwise.
    A synthetic token is a token introduced / inserted into the source by the
    parser in order to recover from an error in the code.

    It typically has a length of zero i.e.: `getLength()` returns zero.
    */
    virtual bool isSynthetic() const = 0;

    /*
    Returns `true` if this is a keyword used in a top-level declaration, `false`
    otherwise. Top-level declaration keywords include `class`, `enum`, `import`
    etc.
    */
    virtual bool isTopLevelKeyword() const = 0;

    /*
    Returns `true` if this is a user-definable operator (such as `PLUS` etc),
    `false` otherwise.
    */
    virtual bool isUserDefinableOperator() const = 0;

    /*
    Returns the corresponding `Keyword` (if this is a keyword token), `nullptr`
    if not.
    */
    virtual const keyword::Keyword* getKeyword() const = 0;

    // TODO: Add docs after `getType()`
    virtual dart::u8 getKind() const = 0;

    virtual std::size_t getLength() const override = 0;

    /*
    Returns the lexeme that represents this token.

    For `StringToken` types, this includes the quotation mmarks, explicit
    escapes etc.
    */
    // TODO: Check if this should be std::string instead.
    virtual std::string_view getLexeme() const = 0;

    /*
    Returns the next token in the token stream.
    */
    virtual Token* getNext() const = 0;

    /*
    Sets the next token in the token stream.

    This only modifies the `next` field internally.
    */
    virtual void setNext(Token* next) = 0;

    virtual std::size_t getOffset() const override = 0;

    /*
    Sets the offset from the beginning of the file to the first character of the
    token.
    */
    virtual void setOffset(std::size_t offset) = 0;

    /*
    Returns the first in the list of comments tokens that precede this token,
    or `nullptr` if no comment tokens precede this token. Subsequent comment
    tokens in the stream can be reached with `getNext()` (on the comment token)
    until `nullptr` is returned.

    Given a source input of two comments preceding `this` token:
    `// first`

    `// second`
    
    `[token]`   <-- this token
    
    Calling `getPrecedingComments()` will return `// first`. Calling it again
    will return `// second`. Calling it a third time will return `nullptr` since
    there are no more comment tokens before `this`.
    */
    virtual CommentToken* getPrecedingComments() const = 0;

    /*
    Returns the previous token in the token stream.
    */
    virtual Token* getPrevious() const = 0;

    /*
    Sets the previous token in the token stream.

    This only modifies the `previous` field internally.
    */
    virtual void setPrevious(Token* previous) = 0;

    // TODO: Add docs after understanding implementation.
    virtual const char* getStringValue() const = 0;

    /*
    Returns the `TokenType` associated with this token.
    */
    virtual const type::TokenType* getType() const = 0;

    /*
    Returns the `TokenType::index` value of the `TokenType` object associated
    with this token.
    */
    virtual dart::u8 getTypeIndex() const = 0;

    /*
    Returns `true` if this token's `TokenType` is in any of the given types,
    `false` otherwise.
    */
    virtual bool
    matchesAny(std::initializer_list<const type::TokenType*> types) const = 0;

    /*
    Sets the next token in the token stream to be the parameter, returning back
    the parameter to the caller.

    This links both `this` token and the one passed in by connecting:
        - `this` token's `next` value to the parameter.
        - the parameter's `previous` value to `this`.
    */
    virtual Token* setNextToken(Token* token) = 0;

    /*
    Sets the next token in the token stream to be the parameter, returning back
    the parameter to the caller.

    This only links `this` token to the one passed in by:
        - connecting `this` token's `next` value to the parameter.
        - leaving the parameter's `previous` value unchanged.
    */
    virtual Token* setNextWithoutSettingPrevious(Token* token) = 0;

    // TODO: Implement std::formatter<Token> per base

    // TODO: Implement Object::value()

    /*
    Returns `true` if the token type associated with `this` token is the same as
    the provided token type, `false` if not.
    */
    bool isA(const type::TokenType* type) const { return getType()->isA(type); }

    /*
    Returns the token (from the given list) that appears first (left-most) in
    the source code, or `nullptr` if the list is empty.

    A token appears first (left-most) in the source code if it has the smallest
    offset from the start of the source code.
    */
    static const Token*
    lexicallyFirst(std::initializer_list<const Token*> tokens) {
        const Token* result{nullptr};
        for (const Token* token : tokens) {
            if (result == nullptr || token->getOffset() < result->getOffset()) {
                result = token;
            }
        }

        return result;
    }
};
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart