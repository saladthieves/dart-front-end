#pragma once

#include "precedence.hpp"
#include "token_type.hpp"

#include "common/types.hpp"

#include <string_view>

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
namespace keyword {
/*
The style of a keyword, either as reserved, built-in or pseudo.
*/
enum class KeywordStyle : dart::u8 {
    /*
    A reserved keyword.
    */
    RESERVED = 1,

    /*
    A built-in keyword.
    */
    BUILTIN,

    /*
    A pseudo keyword, which can be used as an identifier in some situations.
    */
    PSEUDO
};

/*
A `TokenType` to specifically represent a keyword in Dart.
*/
class Keyword : public type::TokenType {
public:
    constexpr explicit Keyword(
        dart::u8 index,
        std::string_view lexeme,
        std::string_view name,
        KeywordStyle keywordStyle,
        bool isModifier = false,
        bool isTopLevelKeyword = false,
        dart::u8 precedence = precedence::NONE
    )
        : TokenType(
              index,
              lexeme,
              name,
              precedence,
              constants::KEYWORD_TOKEN,
              nullptr,
              false,
              isModifier,
              false,
              isTopLevelKeyword
          ),
          keywordStyle{keywordStyle} {}

    constexpr virtual bool isBuiltIn() const override {
        return keywordStyle == KeywordStyle::BUILTIN;
    }

    constexpr virtual bool isPseudo() const override {
        return keywordStyle == KeywordStyle::PSEUDO;
    }

    /*
    Returns `true` if this keyword is pseudo or built-in, `false` otherwise.
    */
    constexpr bool isBuiltInOrPseudo() const {
        return isBuiltIn() || isPseudo();
    }

    constexpr virtual bool isReservedWord() const override {
        return keywordStyle == KeywordStyle::RESERVED;
    }

    // TODO: Implement std::formatter<Keyword>
    // TODO: Implement _createKeywordMap();

    /*
    The style of the keyword.
    */
    const KeywordStyle keywordStyle;
};
} // namespace keyword
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart