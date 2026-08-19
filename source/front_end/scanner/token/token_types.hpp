#pragma once

#include "precedence.hpp"
#include "token_constants.hpp"
#include "token_type.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
namespace type {
/*
The total number of defined `TokenType` constants.
*/
inline constexpr std::size_t TOKENTYPE_COUNT = 82;

// clang-format off
/*
Type for a token that marks the start or end of the input.
*/
inline constexpr TokenType END_OF_FILE = TokenType(
    0,
    "",
    "EOF",
    precedence::NONE,
    token::constants::EOF_TOKEN
);

inline constexpr TokenType DOUBLE = TokenType(
    1,
    "double",
    "DOUBLE",
    precedence::NONE,
    token::constants::DOUBLE_TOKEN,
    nullptr, false, false, false, false, false,
    true // stringValueShouldBeNull
);

inline constexpr TokenType DOUBLE_WITH_SEPARATORS = TokenType(
    2,
    "double",
    "DOUBLE_WITH_SEPARATORS",
    precedence::NONE,
    token::constants::DOUBLE_TOKEN,
    nullptr, false, false, false, false, false,
    true // stringValueShouldBeNull
);

inline constexpr TokenType HEXADECIMAL = TokenType(
    3,
    "hexadecimal",
    "HEXADECIMAL",
    precedence::NONE,
    constants::HEXADECIMAL_TOKEN,
    nullptr, false, false, false, false, false,
    true // stringValueShouldBeNull
);

inline constexpr TokenType HEXADECIMAL_WITH_SEPARATORS = TokenType(
    4,
    "hexadecimal",
    "HEXADECIMAL_WITH_SEPARATORS",
    precedence::NONE,
    constants::HEXADECIMAL_TOKEN,
    nullptr, false, false, false, false, false,
    true // stringValueShouldBeNull
);

inline constexpr TokenType IDENTIFIER = TokenType(
    5,
    "identifier",
    "IDENTIFIER",
    precedence::NONE,
    constants::IDENTIFIER_TOKEN,
    nullptr, false, false, false, false, false,
    true // stringValueShouldBeNull
);

inline constexpr TokenType INT = TokenType(
    6,
    "int",
    "INT",
    precedence::NONE,
    constants::INT_TOKEN,
    nullptr, false, false, false, false, false,
    true // stringValueShouldBeNull
);

inline constexpr TokenType INT_WITH_SEPARATORS = TokenType(
    7,
    "int",
    "INT_WITH_SEPARATORS",
    precedence::NONE,
    constants::INT_TOKEN,
    nullptr, false, false, false, false, false,
    true // stringValueShouldBeNull
);

inline constexpr TokenType MULTI_LINE_COMMENT = TokenType(
    8,
    "comment",
    "MULTI_LINE_COMMENT",
    precedence::NONE,
    constants::COMMENT_TOKEN,
    nullptr, false, false, false, false, false,
    true // stringValueShouldBeNull
);

inline constexpr TokenType SCRIPT_TAG = TokenType(
    9,
    "script",
    "SCRIPT_TAG",
    precedence::NONE,
    constants::SCRIPT_TOKEN
);

inline constexpr TokenType SINGLE_LINE_COMMENT = TokenType(
    10,
    "comment",
    "SINGLE_LINE_COMMENT",
    precedence::NONE,
    constants::COMMENT_TOKEN,
    nullptr, false, false, false, false, false,
    true // stringValueShouldBeNull
);

inline constexpr TokenType STRING = TokenType(
    11,
    "string",
    "STRING",
    precedence::NONE,
    constants::STRING_TOKEN,
    nullptr, false, false, false, false, false,
    true // stringValueShouldBeNull
);

inline constexpr TokenType AMPERSAND = TokenType(
    12,
    "&",
    "AMPERSAND",
    precedence::BITWISE_AND,
    constants::AMPERSAND_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType AMPERSAND_AMPERSAND = TokenType(
    13,
    "&&",
    "AMPERSAND_AMPERSAND",
    precedence::LOGICAL_AND,
    constants::AMPERSAND_AMPERSAND_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true    // isOperator
);

inline constexpr TokenType AMPERSAND_AMPERSAND_EQ = TokenType(
    14,
    "&&=",
    "AMPERSAND_AMPERSAND_EQ",
    precedence::ASSIGNMENT,
    constants::AMPERSAND_AMPERSAND_EQ_TOKEN,
    &AMPERSAND_AMPERSAND,
    false, false,
    true    // isOperator
);

inline constexpr TokenType AMPERSAND_EQ = TokenType(
    15,
    "&=",
    "AMPERSAND_EQ",
    precedence::ASSIGNMENT,
    constants::AMPERSAND_EQ_TOKEN,
    &AMPERSAND,
    false, false,
    true    // isOperator
);

inline constexpr TokenType AT = TokenType(
    16,
    "@",
    "AT",
    precedence::NONE,
    constants::AT_TOKEN
);

inline constexpr TokenType BANG = TokenType(
    17,
    "!",
    "BANG",
    precedence::PREFIX,
    constants::BANG_TOKEN,
    nullptr, false, false,
    true    // isOperator
);

inline constexpr TokenType BANG_EQ = TokenType(
    18,
    "!=",
    "BANG_EQ",
    precedence::EQUALITY,
    constants::BANG_EQ_TOKEN,
    nullptr, false, false,
    true    // isOperator
);

inline constexpr TokenType BANG_EQ_EQ = TokenType(
    19,
    "!==",
    "BANG_EQ_EQ",
    precedence::EQUALITY,
    constants::BANG_EQ_EQ_TOKEN
);

inline constexpr TokenType BAR = TokenType(
    20,
    "|",
    "BAR",
    precedence::BITWISE_OR,
    constants::BAR_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType BAR_BAR = TokenType(
    21,
    "||",
    "BAR_BAR",
    precedence::LOGICAL_OR,
    constants::BAR_BAR_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true    // isOperator
);

/*
Not yet part of the language, and not supported by the scanner.
*/
inline constexpr TokenType BAR_BAR_EQ = TokenType(
    22,
    "||=",
    "BAR_BAR_EQ",
    precedence::ASSIGNMENT,
    constants::BAR_BAR_EQ_TOKEN,
    &BAR_BAR,
    false, false,
    true    // isOperator
);

inline constexpr TokenType BAR_EQ = TokenType(
    23,
    "|=",
    "BAR_EQ",
    precedence::ASSIGNMENT,
    constants::BAR_EQ_TOKEN,
    &BAR,
    false, false,
    true    // isOperator
);

inline constexpr TokenType COLON = TokenType(
    24,
    ":",
    "COLON",
    precedence::NONE,
    constants::COLON_TOKEN
);

inline constexpr TokenType COMMA = TokenType(
    25,
    ",",
    "COMMA",
    precedence::NONE,
    constants::COMMA_TOKEN
);

inline constexpr TokenType CARET = TokenType(
    26,
    "^",
    "CARET",
    precedence::BITWISE_XOR,
    constants::CARET_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType CARET_EQ = TokenType(
    27,
    "^=",
    "CARET_EQ",
    precedence::ASSIGNMENT,
    constants::CARET_EQ_TOKEN,
    &CARET,
    false, false,
    true   // isOperator
);

inline constexpr TokenType CLOSE_CURLY_BRACKET = TokenType(
    28,
    "}",
    "CLOSE_CURLY_BRACKET",
    precedence::NONE,
    constants::CLOSE_CURLY_BRACKET_TOKEN
);

inline constexpr TokenType CLOSE_PAREN = TokenType(
    29,
    ")",
    "CLOSE_PAREN",
    precedence::NONE,
    constants::CLOSE_PAREN_TOKEN
);

inline constexpr TokenType CLOSE_SQUARE_BRACKET = TokenType(
    30,
    "]",
    "CLOSE_SQUARE_BRACKET",
    precedence::NONE,
    constants::CLOSE_SQUARE_BRACKET_TOKEN
);

inline constexpr TokenType EQ = TokenType(
    31,
    "=",
    "EQ",
    precedence::ASSIGNMENT,
    constants::EQ_TOKEN,
    nullptr, false, false,
    true   // isOperator
);

inline constexpr TokenType EQ_EQ = TokenType(
    32,
    "==",
    "EQ_EQ",
    precedence::EQUALITY,
    constants::EQ_EQ_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

/*
This is not supported by Dart, is nevertheless parsed by the scanner in order to
support better recovery when a JavaScript snippet is pasted into a Dart file.
*/
inline constexpr TokenType EQ_EQ_EQ = TokenType(
    33,
    "===",
    "EQ_EQ_EQ",
    precedence::EQUALITY,
    constants::EQ_EQ_EQ_TOKEN
);

inline constexpr TokenType FUNCTION = TokenType(
    34,
    "=>",
    "FUNCTION",
    precedence::NONE,
    constants::FUNCTION_TOKEN
);

inline constexpr TokenType GT = TokenType(
    35,
    ">",
    "GT",
    precedence::RELATIONAL,
    constants::GT_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType GT_EQ = TokenType(
    36,
    ">=",
    "GT_EQ",
    precedence::RELATIONAL,
    constants::GT_EQ_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType GT_GT = TokenType(
    37,
    ">>",
    "GT_GT",
    precedence::SHIFT,
    constants::GT_GT_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType GT_GT_EQ = TokenType(
    38,
    ">>=",
    "GT_GT_EQ",
    precedence::ASSIGNMENT,
    constants::GT_GT_EQ_TOKEN,
    &GT_GT,
    false, false,
    true   // isOperator
);

inline constexpr TokenType GT_GT_GT = TokenType(
    39,
    ">>>",
    "GT_GT_GT",
    precedence::SHIFT,
    constants::GT_GT_GT_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType GT_GT_GT_EQ = TokenType(
    40,
    ">>>=",
    "GT_GT_GT_EQ",
    precedence::ASSIGNMENT,
    constants::GT_GT_GT_EQ_TOKEN,
    &GT_GT_GT,
    false, false,
    true   // isOperator
);

inline constexpr TokenType HASH = TokenType(
    41,
    "#",
    "HASH",
    precedence::NONE,
    constants::HASH_TOKEN
);

inline constexpr TokenType INDEX = TokenType(
    42,
    "[]",
    "INDEX",
    precedence::SELECTOR,
    constants::INDEX_TOKEN,
    nullptr, false, false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType INDEX_EQ = TokenType(
    43,
    "[]=",
    "INDEX_EQ",
    precedence::NONE,
    constants::INDEX_EQ_TOKEN,
    nullptr, false, false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType LT = TokenType(
    44,
    "<",
    "LT",
    precedence::RELATIONAL,
    constants::LT_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType LT_EQ = TokenType(
    45,
    "<=",
    "LT_EQ",
    precedence::RELATIONAL,
    constants::LT_EQ_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType LT_LT = TokenType(
    46,
    "<<",
    "LT_LT",
    precedence::SHIFT,
    constants::LT_LT_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType LT_LT_EQ = TokenType(
    47,
    "<<=",
    "LT_LT_EQ",
    precedence::ASSIGNMENT,
    constants::LT_LT_EQ_TOKEN,
    &LT_LT,
    false, false,
    true   // isOperator
);

inline constexpr TokenType MINUS = TokenType(
    48,
    "-",
    "MINUS",
    precedence::ADDITIVE,
    constants::MINUS_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType MINUS_EQ = TokenType(
    49,
    "-=",
    "MINUS_EQ",
    precedence::ASSIGNMENT,
    constants::MINUS_EQ_TOKEN,
    &MINUS,
    false, false,
    true   // isOperator
);

inline constexpr TokenType MINUS_MINUS = TokenType(
    50,
    "--",
    "MINUS_MINUS",
    precedence::POSTFIX,
    constants::MINUS_MINUS_TOKEN,
    nullptr, false, false,
    true   // isOperator
);

inline constexpr TokenType OPEN_CURLY_BRACKET = TokenType(
    51,
    "{",
    "OPEN_CURLY_BRACKET",
    precedence::NONE,
    constants::OPEN_CURLY_BRACKET_TOKEN
);

inline constexpr TokenType OPEN_PAREN = TokenType(
    52,
    "(",
    "OPEN_PAREN",
    precedence::SELECTOR,
    constants::OPEN_PAREN_TOKEN
);

inline constexpr TokenType OPEN_SQUARE_BRACKET = TokenType(
    53,
    "[",
    "OPEN_SQUARE_BRACKET",
    precedence::SELECTOR,
    constants::OPEN_SQUARE_BRACKET_TOKEN
);

inline constexpr TokenType PERCENT = TokenType(
    54,
    "%",
    "PERCENT",
    precedence::MULTIPLICATIVE,
    constants::PERCENT_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType PERCENT_EQ = TokenType(
    55,
    "%=",
    "PERCENT_EQ",
    precedence::ASSIGNMENT,
    constants::PERCENT_EQ_TOKEN,
    &PERCENT,
    false, false,
    true   // isOperator
);

inline constexpr TokenType PERIOD = TokenType(
    56,
    ".",
    "PERIOD",
    precedence::SELECTOR,
    constants::PERIOD_TOKEN
);

inline constexpr TokenType PERIOD_PERIOD = TokenType(
    57,
    "..",
    "PERIOD_PERIOD",
    precedence::CASCADE,
    constants::PERIOD_PERIOD_TOKEN,
    nullptr, false, false,
    true   // isOperator
);

inline constexpr TokenType PLUS = TokenType(
    58,
    "+",
    "PLUS",
    precedence::ADDITIVE,
    constants::PLUS_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType PLUS_EQ = TokenType(
    59,
    "+=",
    "PLUS_EQ",
    precedence::ASSIGNMENT,
    constants::PLUS_EQ_TOKEN,
    &PLUS,
    false, false,
    true   // isOperator
);

inline constexpr TokenType PLUS_PLUS = TokenType(
    60,
    "++",
    "PLUS_PLUS",
    precedence::POSTFIX,
    constants::PLUS_PLUS_TOKEN,
    nullptr, false, false,
    true   // isOperator
);

inline constexpr TokenType QUESTION = TokenType(
    61,
    "?",
    "QUESTION",
    precedence::CONDITIONAL,
    constants::QUESTION_TOKEN,
    nullptr, false, false,
    true   // isOperator
);

inline constexpr TokenType QUESTION_PERIOD = TokenType(
    62,
    "?.",
    "QUESTION_PERIOD",
    precedence::SELECTOR,
    constants::QUESTION_PERIOD_TOKEN,
    nullptr, false, false,
    true   // isOperator
);

inline constexpr TokenType QUESTION_QUESTION = TokenType(
    63,
    "??",
    "QUESTION_QUESTION",
    precedence::IF_NULL,
    constants::QUESTION_QUESTION_TOKEN,
    nullptr,
    true,  // isBinaryOperator
    false,
    true   // isOperator
);

inline constexpr TokenType QUESTION_QUESTION_EQ = TokenType(
    64,
    R"(??=)",
    "QUESTION_QUESTION_EQ",
    precedence::ASSIGNMENT,
    constants::QUESTION_QUESTION_EQ_TOKEN,
    &QUESTION_QUESTION,
    false, false,
    true   // isOperator
);

inline constexpr TokenType SEMICOLON = TokenType(
    65,
    ";",
    "SEMICOLON",
    precedence::NONE,
    constants::SEMICOLON_TOKEN
);

inline constexpr TokenType SLASH = TokenType(
    66,
    "/",
    "SLASH",
    precedence::MULTIPLICATIVE,
    constants::SLASH_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType SLASH_EQ = TokenType(
    67,
    "/=",
    "SLASH_EQ",
    precedence::ASSIGNMENT,
    constants::SLASH_EQ_TOKEN,
    &SLASH,
    false, false,
    true   // isOperator
);

inline constexpr TokenType STAR = TokenType(
    68,
    "*",
    "STAR",
    precedence::MULTIPLICATIVE,
    constants::STAR_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType STAR_EQ = TokenType(
    69,
    "*=",
    "STAR_EQ",
    precedence::ASSIGNMENT,
    constants::STAR_EQ_TOKEN,
    &STAR,
    false, false,
    true   // isOperator
);

inline constexpr TokenType STRING_INTERPOLATION_EXPRESSION = TokenType(
    70,
    "${",
    "STRING_INTERPOLATION_EXPRESSION",
    precedence::NONE,
    constants::STRING_INTERPOLATION_TOKEN
);

inline constexpr TokenType STRING_INTERPOLATION_IDENTIFIER = TokenType(
    71,
    "$",
    "STRING_INTERPOLATION_IDENTIFIER",
    precedence::NONE,
    constants::STRING_INTERPOLATION_IDENTIFIER_TOKEN
);

inline constexpr TokenType TILDE = TokenType(
    72,
    "~",
    "TILDE",
    precedence::PREFIX,
    constants::TILDE_TOKEN,
    nullptr, false, false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType TILDE_SLASH = TokenType(
    73,
    "~/",
    "TILDE_SLASH",
    precedence::MULTIPLICATIVE,
    constants::TILDE_SLASH_TOKEN,
    nullptr,
    true,   // isBinaryOperator
    false,
    true,   // isOperator
    false,
    true    // isUserDefinableOperator
);

inline constexpr TokenType TILDE_SLASH_EQ = TokenType(
    74,
    "~/=",
    "TILDE_SLASH_EQ",
    precedence::ASSIGNMENT,
    constants::TILDE_SLASH_EQ_TOKEN,
    &TILDE_SLASH,
    false, false,
    true   // isOperator
);

inline constexpr TokenType BACKPING = TokenType(
    75,
    "`",
    "BACKPING",
    precedence::NONE,
    constants::BACKPING_TOKEN
);

inline constexpr TokenType BACKSLASH = TokenType(
    76,
    "\\",
    "BACKSLASH",
    precedence::NONE,
    constants::BACKSLASH_TOKEN
);

inline constexpr TokenType PERIOD_PERIOD_PERIOD = TokenType(
    77,
    "...",
    "PERIOD_PERIOD_PERIOD",
    precedence::NONE,
    constants::PERIOD_PERIOD_PERIOD_TOKEN
);

inline constexpr TokenType PERIOD_PERIOD_PERIOD_QUESTION = TokenType(
    78,
    "...?",
    "PERIOD_PERIOD_PERIOD_QUESTION",
    precedence::NONE,
    constants::PERIOD_PERIOD_PERIOD_QUESTION_TOKEN
);

inline constexpr TokenType QUESTION_PERIOD_PERIOD = TokenType(
    79,
    "?..",
    "QUESTION_PERIOD_PERIOD",
    precedence::CASCADE,
    constants::QUESTION_PERIOD_PERIOD_TOKEN
);

/*
Token type used by error tokens.
*/
inline constexpr TokenType BAD_INPUT = TokenType(
    80,
    "malformed input",
    "BAD_INPUT",
    precedence::NONE,
    constants::BAD_INPUT_TOKEN,
    nullptr, false, false, false, false, false,
    true    // stringValueShouldBeNull
);

/*
Token type used by synthetic tokens created during parser recovery (non-analyzer
case).
*/
inline constexpr TokenType RECOVERY = TokenType(
    81,
    "recovery",
    "RECOVERY",
    precedence::NONE,
    constants::RECOVERY_TOKEN,
    nullptr, false, false, false, false, false,
    true    // stringValueShouldBeNull
);
// clang-format on
} // namespace type
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart