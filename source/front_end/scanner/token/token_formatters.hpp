#pragma once

#include "token/begin_token.hpp"
#include "token/comment_token.hpp"
#include "token/keyword.hpp"
#include "token/token.hpp"
#include "token/token_type.hpp"
#include "util/format_utils.hpp"

#include <format>
#include <string_view>

namespace std {
using namespace dart::front_end::scanner::token;
using dart::util::fmt::formatAddress;

// TODO: Fix the tab mess
inline constexpr auto TAB = "   ";
inline constexpr auto TAB_2 = "      ";
inline constexpr auto TAB_3 = "         ";

inline constexpr std::string_view name(const Token* token) {
    // CommentToken
    if (dynamic_cast<const DocumentationCommentToken*>(token)) {
        return "DocumentationToken";
    }

    if (dynamic_cast<const DartDocToken*>(token)) {
        return "DartDocToken";
    }

    if (dynamic_cast<const LanguageVersionToken*>(token)) {
        return "LanguageVersionToken";
    }

    if (dynamic_cast<const CommentToken*>(token)) {
        return "CommentToken";
    }

    // Token
    if (dynamic_cast<const BeginToken*>(token)) {
        return "BeginToken";
    }
    if (dynamic_cast<const Token*>(token)) {
        return "Token";
    }

    return "[[[🛑UnknownToken]]]";
}

inline std::string tokenHeader(const Token* token, std::string_view tab) {
    return std::format(
        "{}[ {}: {} ]\n", tab, name(token), formatAddress(token)
    );
}

inline std::string
typeHeader(const type::TokenType* type, std::string_view tab) {
    const auto name =
        dynamic_cast<const keyword::Keyword*>(type) ? "Keyword" : "TokenType";
    return std::format("\n{}[ {}: {} ]\n", tab, name, formatAddress(type));
}

enum DetailsMode : dart::u8 {
    MINIMUM = 1,
    NORMAL,
    FULL
};

// TokenType
template <>
struct std::formatter<type::TokenType> {
    constexpr auto parse(format_parse_context& context) {
        auto iter = begin(context);
        while (iter != context.end() && *iter != '}') {
            switch (*iter) {
                case 'M': mode = DetailsMode::MINIMUM; break;
                case 'N': mode = DetailsMode::NORMAL; break;
                case 'F': mode = DetailsMode::FULL; break;
            }

            ++iter;
        }

        return iter;
    }

    auto format(const type::TokenType& type, format_context& context) const {
        std::string output{};

        if (mode == DetailsMode::MINIMUM) {
            output = type.name;
            return std::format_to(context.out(), "{}", output);
        }

        output += typeHeader(&type, TAB_3);

        if (mode > DetailsMode::MINIMUM) {
            output += std::format("{}index: {}\n", TAB_3, type.index);
            output += std::format("{}name: {}\n", TAB_3, type.name);
            output += std::format("{}lexeme: {}\n", TAB_3, type.lexeme);

            output += std::format("{}kind: {}\n", TAB_3, type.kind);
            output += std::format("{}precedence: {}\n", TAB_3, type.precedence);
            output += std::format("{}isOperator: {}\n", TAB_3, type.isOperator);
            output +=
                std::format("{}isReserved: {}\n", TAB_3, type.isReservedWord());
            output += std::format("{}isModifier: {}\n", TAB_3, type.isModifier);
            output += std::format("{}isKeyword: {}\n", TAB_3, type.isKeyword());
            output += std::format("{}isPseudo: {}\n", TAB_3, type.isPseudo());
            output += std::format("{}isBuiltIn: {}\n", TAB_3, type.isBuiltIn());
        }

        if (mode > DetailsMode::NORMAL) {

            output += std::format(
                "{}isTopLevelKeyword: {}\n", TAB_3, type.isTopLevelKeyword
            );
            output += std::format(
                "{}binaryOperatorOfCompoundAssignment: {}\n", TAB_3,
                formatAddress(type.binaryOperatorOfCompoundAssignment)
            );
            output += std::format(
                "{}isBinaryOperator: {}\n", TAB_3, type.isBinaryOperator
            );
            output += std::format(
                "{}isUserDefinableOperator: {}\n", TAB_3,
                type.isUserDefinableOperator
            );
            output += std::format(
                "{}isAdditiveOperator: {}\n", TAB_3, type.isAdditiveOperator()
            );
            output += std::format(
                "{}isAssignmentOperator: {}\n", TAB_3,
                type.isAssignmentOperator()
            );
            output += std::format(
                "{}isMultiplicativeOperator: {}\n", TAB_3,
                type.isMultiplicativeOperator()
            );
            output += std::format(
                "{}isShiftOperator: {}\n", TAB_3, type.isShiftOperator()
            );
            output += std::format(
                "{}isUnaryPostfixOperator: {}\n", TAB_3,
                type.isUnaryPostfixOperator()
            );
            output += std::format(
                "{}isSelectorOperator: {}\n", TAB_3, type.isSelectorOperator()
            );
            output += std::format(
                "{}isAssociativeOperator: {}\n", TAB_3,
                type.isAssociativeOperator()
            );
            output += std::format(
                "{}isEqualityOperator: {}\n", TAB_3, type.isEqualityOperator()
            );
            output += std::format(
                "{}isIncrementOperator: {}\n", TAB_3, type.isIncrementOperator()
            );
            output += std::format(
                "{}isRelationalOperator: {}\n", TAB_3,
                type.isRelationalOperator()
            );
            output += std::format(
                "{}isUnaryPrefixOperator: {}\n", TAB_3,
                type.isUnaryPrefixOperator()
            );
            output += std::format(
                "{}isUnaryPostfixOperator: {}\n", TAB_3,
                type.isUnaryPostfixOperator()
            );
        }

        output += std::format(
            "{}stringValue: {}\n", TAB_3,
            (type.stringValue != nullptr ? type.stringValue : "nullptr")
        );

        output += std::format("{}------------------------------", TAB_3);
        return std::format_to(context.out(), "{}", output);
    }

    DetailsMode mode{DetailsMode::MINIMUM};
};

// CommentToken
template <>
struct std::formatter<CommentToken> {
    constexpr auto parse(format_parse_context& context) {
        return context.begin();
    }

    auto format(const CommentToken& token, format_context& context) const {
        std::string output{};
        output += tokenHeader(&token, TAB_2);
        output += std::format("{}type: {}\n", TAB_2, *token.type);
        output +=
            std::format("{}_prev: {}\n", TAB_2, formatAddress(token.prev()));
        output += std::format("{}beginOffset: {}\n", TAB_2, token.beginOffset);
        output += std::format("{}length:      {}\n", TAB_2, token.length);
        output += std::format("{}endOffset:   {}\n", TAB_2, token.endOffset);
        output += std::format("{}lexeme: {}\n", TAB_2, token.lexeme);
        output += std::format("{}stringValue: {}\n", TAB_2, token.stringValue);
        output +=
            std::format("{}parent: {}\n", TAB_2, formatAddress(token.parent));
        output +=
            std::format("{}_next:  {}\n", TAB_2, formatAddress(token.next()));
        output += std::format("{}------------------------------\n", TAB_2);

        return std::format_to(context.out(), "{}", output);
    }
};

// Token
template <>
struct std::formatter<Token> {
    constexpr auto parse(format_parse_context& context) {
        return context.begin();
    }

    auto format(const Token& token, format_context& context) const {
        std::string output{};
        output += tokenHeader(&token, TAB);
        output +=
            std::format("{}_prev: {}\n", TAB, formatAddress(token.prev()));
        output += std::format("{}type: {:}\n", TAB, *token.type);
        output += std::format("{}beginOffset: {}\n", TAB, token.beginOffset);
        output += std::format("{}length:      {}\n", TAB, token.length);
        output += std::format("{}endOffset:   {}\n", TAB, token.endOffset);
        output += std::format(
            "{}beforeSynthetic: {}\n", TAB, formatAddress(token.beforeSynthetic)
        );
        if (token.precedingComment() != nullptr) {
            output += std::format("{}_precedingComment:\n", TAB);
            auto* comment = token.precedingComment();
            while (comment != nullptr) {
                output += std::format("{}", *comment);
                comment = dynamic_cast<CommentToken*>(comment->next());
            }
        }

        output +=
            std::format("{}endToken: {}\n", TAB, formatAddress(token.endToken));
        output += std::format("{}lexeme: {}\n", TAB, token.lexeme);
        output += std::format("{}stringValue: {}\n", TAB, token.stringValue);
        output += std::format("{}isEof: {}\n", TAB, token.isEof);
        if (!token.isEof) {
            output +=
                std::format("{}isIdentifier: {}\n", TAB, token.isIdentifier);
            output += std::format("{}isKeyword: {}\n", TAB, token.isKeyword);
            output += std::format(
                "{}isKeywordOrIdentifier: {}\n", TAB,
                token.isKeywordOrIdentifier
            );
            output += std::format("{}isModifier: {}\n", TAB, token.isModifier);
            output += std::format("{}isOperator: {}\n", TAB, token.isOperator);
            output +=
                std::format("{}isSynthetic: {}\n", TAB, token.isSynthetic);
            output += std::format(
                "{}isTopLevelKeyword: {}\n", TAB, token.isTopLevelKeyword
            );
            output += std::format(
                "{}isUserDefinableOperator: {}\n", TAB,
                token.isUserDefinableOperator
            );
        }
        output +=
            std::format("{}_next: {}\n", TAB, formatAddress(token.next()));
        output += std::format("{}------------------------------", TAB);

        return std::format_to(context.out(), "{}", output);
    }
};

} // namespace std