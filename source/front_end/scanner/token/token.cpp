#include "token.hpp"
#include "comment_token.hpp"

#include "token/token_constants.hpp"
#include "token/token_types.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
// Token
Token* Token::setNext(Token* token, bool setPrevious) {
    _next = token;
    if (setPrevious) {
        token->_prev = this;
        token->beforeSynthetic = this;
    }
    return token;
}

Token* Token::setPrev(Token* token) {
    _prev = token;
    return token;
}

void Token::setPrecedingComment(CommentToken* comment) {
    _precedingComment = comment;
    setCommentParent(_precedingComment);
}

bool
Token::matchesAny(std::initializer_list<const type::TokenType*> types) const {
    for (const auto* t : types) {
        if (type == t) return true;
    }
    return false;
}

void Token::setCommentParent(CommentToken* comment) {
    while (comment != nullptr) {
        comment->parent = this;
        comment = dynamic_cast<CommentToken*>(comment->next());
    }
}

void Token::init() {
    endOffset = beginOffset + length;

    isEof = type == &type::END_OF_FILE;
    isIdentifier = type->kind == constants::IDENTIFIER_TOKEN ||
                   type->isBuiltIn() || type->isPseudo();
    isKeyword = type->isKeyword();
    isKeywordOrIdentifier = isKeyword || isIdentifier;
    isModifier = type->isModifier;
    isOperator = type->isOperator;
    isSynthetic = length == 0;
    isTopLevelKeyword = type->isTopLevelKeyword;
    isUserDefinableOperator = type->isUserDefinableOperator;

    if (isKeyword || isSynthetic) {
        lexeme = type->lexeme;
    }

    stringValue = lexeme;
}

// lexicallyFirst
const Token* lexicallyFirst(std::initializer_list<const Token*> tokens) {
    const Token* result{nullptr};
    for (const Token* token : tokens) {
        if (result == nullptr || token->beginOffset < result->beginOffset) {
            result = token;
        }
    }
    return result;
}
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart