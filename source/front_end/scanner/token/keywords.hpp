#pragma once

#include "keyword.hpp"
#include "token_types.hpp"

#include <array>

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
namespace keyword {
/*
The total number of defined `TokenType` constants.
*/
inline constexpr std::size_t KEYWORD_COUNT = 73;

// clang-format off
inline constexpr Keyword ABSTRACT = Keyword(
    82,
    "abstract",
    "ABSTRACT",
    KeywordStyle::BUILTIN,
    true // isModifier
);

inline constexpr Keyword AS = Keyword(
    83,
    "as",
    "AS",
    KeywordStyle::BUILTIN,
    false, false,
    precedence::RELATIONAL   // precedence
);

inline constexpr Keyword ASSERT = Keyword(
    84,
    "assert",
    "ASSERT",
    KeywordStyle::RESERVED
);

inline constexpr Keyword ASYNC = Keyword(
    85,
    "async",
    "ASYNC",
    KeywordStyle::PSEUDO
);

inline constexpr Keyword AUGMENT = Keyword(
    86,
    "augment",
    "AUGMENT",
    KeywordStyle::BUILTIN,
    true    // isModifier
);

inline constexpr Keyword AWAIT = Keyword(
    87,
    "await",
    "AWAIT",
    KeywordStyle::PSEUDO
);

inline constexpr Keyword BASE = Keyword(
    88,
    "base",
    "BASE",
    KeywordStyle::PSEUDO
);

inline constexpr Keyword BREAK = Keyword(
    89,
    "break",
    "BREAK",
    KeywordStyle::RESERVED
);

inline constexpr Keyword CASE = Keyword(
    90,
    "case",
    "CASE",
    KeywordStyle::RESERVED
);

inline constexpr Keyword CATCH = Keyword(
    91,
    "catch",
    "CATCH",
    KeywordStyle::RESERVED
);

inline constexpr Keyword CLASS = Keyword(
    92,
    "class",
    "CLASS",
    KeywordStyle::RESERVED,
    false,
    true    // isTopLevelKeyword
);

inline constexpr Keyword CONST = Keyword(
    93,
    "const",
    "CONST",
    KeywordStyle::RESERVED,
    true    // isModifier
);

inline constexpr Keyword CONTINUE = Keyword(
    94,
    "continue",
    "CONTINUE",
    KeywordStyle::RESERVED
);

inline constexpr Keyword COVARIANT = Keyword(
    95,
    "covariant",
    "COVARIANT",
    KeywordStyle::BUILTIN,
    true    // isModifier
);

inline constexpr Keyword DEFAULT = Keyword(
    96,
    "default",
    "DEFAULT",
    KeywordStyle::RESERVED
);

inline constexpr Keyword DEFERRED = Keyword(
    97,
    "deferred",
    "deferred",
    KeywordStyle::BUILTIN
);

inline constexpr Keyword DO = Keyword(
    98,
    "do",
    "DO",
    KeywordStyle::RESERVED
);

inline constexpr Keyword DYNAMIC = Keyword(
    99,
    "dynamic",
    "DYNAMIC",
    KeywordStyle::BUILTIN
);

inline constexpr Keyword ELSE = Keyword(
    100,
    "else",
    "ELSE",
    KeywordStyle::RESERVED
);

inline constexpr Keyword ENUM = Keyword(
    101,
    "enum",
    "ENUM",
    KeywordStyle::RESERVED,
    false,
    true    // isTopLevelKeyword
);

inline constexpr Keyword EXPORT = Keyword(
    102,
    "export",
    "EXPORT",
    KeywordStyle::BUILTIN,
    false,
    true    // isTopLevelKeyword
);

inline constexpr Keyword EXTENDS = Keyword(
    103,
    "extends",
    "EXTENDS",
    KeywordStyle::RESERVED
);

inline constexpr Keyword EXTENSION = Keyword(
    104,
    "extension",
    "EXTENSION",
    KeywordStyle::BUILTIN,
    false,
    true    // isTopLevelKeyword
);

inline constexpr Keyword EXTERNAL = Keyword(
    105,
    "external",
    "EXTERNAL",
    KeywordStyle::BUILTIN,
    true    // isModifier
);

inline constexpr Keyword FACTORY = Keyword(
    106,
    "factory",
    "FACTORY",
    KeywordStyle::BUILTIN
);

inline constexpr Keyword FALSE = Keyword(
    107,
    "false",
    "FALSE",
    KeywordStyle::RESERVED
);

inline constexpr Keyword FINAL = Keyword(
    108,
    "final",
    "FINAL",
    KeywordStyle::RESERVED,
    true    // isModifier
);

inline constexpr Keyword FINALLY = Keyword(
    109,
    "finally",
    "FINALLY",
    KeywordStyle::RESERVED
);

inline constexpr Keyword FOR = Keyword(
    110,
    "for",
    "FOR",
    KeywordStyle::RESERVED
);

inline constexpr Keyword FUNCTION = Keyword(
    111,
    "Function",
    "FUNCTION",
    KeywordStyle::BUILTIN
);

inline constexpr Keyword GET = Keyword(
    112,
    "get",
    "GET",
    KeywordStyle::BUILTIN
);

inline constexpr Keyword HIDE = Keyword(
    113,
    "hide",
    "HIDE",
    KeywordStyle::PSEUDO
);

inline constexpr Keyword IF = Keyword(
    114,
    "if",
    "IF",
    KeywordStyle::RESERVED
);

inline constexpr Keyword IMPLEMENTS = Keyword(
    115,
    "implements",
    "IMPLEMENTS",
    KeywordStyle::BUILTIN
);

inline constexpr Keyword IMPORT = Keyword(
    116,
    "import",
    "IMPORT",
    KeywordStyle::BUILTIN,
    false,
    true    // isTopLevelKeyword
);

inline constexpr Keyword IN = Keyword(
    117,
    "in",
    "IN",
    KeywordStyle::RESERVED
);

inline constexpr Keyword INOUT = Keyword(
    118,
    "inout",
    "INOUT",
    KeywordStyle::PSEUDO
);

inline constexpr Keyword INTERFACE = Keyword(
    119,
    "interface",
    "INTERFACE",
    KeywordStyle::BUILTIN
);

inline constexpr Keyword IS = Keyword(
    120,
    "is",
    "IS",
    KeywordStyle::RESERVED,
    false, false,
    precedence::RELATIONAL   // precedence
);

inline constexpr Keyword LATE = Keyword(
    121,
    "late",
    "LATE",
    KeywordStyle::BUILTIN,
    true    // isModifier 
);

inline constexpr Keyword LIBRARY = Keyword(
    122,
    "library",
    "LIBRARY",
    KeywordStyle::BUILTIN,
    false,
    true    // isTopLevelKeyword
);

inline constexpr Keyword MIXIN = Keyword(
    123,
    "mixin",
    "MIXIN",
    KeywordStyle::BUILTIN,
    false,
    true    // isTopLevelKeyword
);

inline constexpr Keyword NATIVE = Keyword(
    124,
    "native",
    "NATIVE",
    KeywordStyle::PSEUDO
);

inline constexpr Keyword NEW = Keyword(
    125,
    "new",
    "NEW",
    KeywordStyle::RESERVED
);

inline constexpr Keyword NIL = Keyword(
    126,
    "null",
    "NULL",
    KeywordStyle::RESERVED
);

inline constexpr Keyword OF = Keyword(
    127,
    "of",
    "OF",
    KeywordStyle::PSEUDO
);

inline constexpr Keyword ON = Keyword(
    128,
    "on",
    "ON",
    KeywordStyle::PSEUDO
);

inline constexpr Keyword OPERATOR = Keyword(
    129,
    "operator",
    "OPERATOR",
    KeywordStyle::BUILTIN
);

inline constexpr Keyword OUT = Keyword(
    130,
    "out",
    "OUT",
    KeywordStyle::PSEUDO
);

inline constexpr Keyword PART = Keyword(
    131,
    "part",
    "PART",
    KeywordStyle::BUILTIN,
    false,
    true    // isTopLevelKeyword
);

inline constexpr Keyword PATCH = Keyword(
    132,
    "patch",
    "PATCH",
    KeywordStyle::PSEUDO
);

inline constexpr Keyword REQUIRED = Keyword(
    133,
    "required",
    "REQUIRED",
    KeywordStyle::BUILTIN,
    true    // isModifier
);

inline constexpr Keyword RETHROW = Keyword(
    134,
    "rethrow",
    "RETHROW",
    KeywordStyle::RESERVED
);

inline constexpr Keyword RETURN = Keyword(
    135,
    "return",
    "RETURN",
    KeywordStyle::RESERVED
);

inline constexpr Keyword SEALED = Keyword(
    136,
    "sealed",
    "SEALED",
    KeywordStyle::PSEUDO
);

inline constexpr Keyword SET = Keyword(
    137,
    "set",
    "SET",
    KeywordStyle::BUILTIN
);

inline constexpr Keyword SHOW = Keyword(
    138,
    "show",
    "SHOW",
    KeywordStyle::PSEUDO
);

inline constexpr Keyword SOURCE = Keyword(
    139,
    "source",
    "SOURCE",
    KeywordStyle::PSEUDO
);

inline constexpr Keyword STATIC = Keyword(
    140,
    "static",
    "STATIC",
    KeywordStyle::BUILTIN,
    true    // isModifier
);

inline constexpr Keyword SUPER = Keyword(
    141,
    "super",
    "SUPER",
    KeywordStyle::RESERVED
);

inline constexpr Keyword SWITCH = Keyword(
    142,
    "switch",
    "SWITCH",
    KeywordStyle::RESERVED
);

inline constexpr Keyword SYNC = Keyword(
    143,
    "sync",
    "SYNC",
    KeywordStyle::PSEUDO
);

inline constexpr Keyword THIS = Keyword(
    144,
    "this",
    "THIS",
    KeywordStyle::RESERVED
);

inline constexpr Keyword THROW = Keyword(
    145,
    "throw",
    "THROW",
    KeywordStyle::RESERVED
);

inline constexpr Keyword TRUE = Keyword(
    146,
    "true",
    "TRUE",
    KeywordStyle::RESERVED
);

inline constexpr Keyword TRY = Keyword(
    147,
    "try",
    "TRY",
    KeywordStyle::RESERVED
);

inline constexpr Keyword TYPEDEF = Keyword(
    148,
    "typedef",
    "TYPEDEF",
    KeywordStyle::BUILTIN,
    false,
    true    // isTopLevelKeyword
);

inline constexpr Keyword VAR = Keyword(
    149,
    "var",
    "VAR",
    KeywordStyle::RESERVED,
    true    // isModifier
);

inline constexpr Keyword VOID = Keyword(
    150,
    "void",
    "VOID",
    KeywordStyle::RESERVED
);

inline constexpr Keyword WHEN = Keyword(
    151,
    "when",
    "WHEN",
    KeywordStyle::PSEUDO
);

inline constexpr Keyword WHILE = Keyword(
    152,
    "while",
    "WHILE",
    KeywordStyle::RESERVED
);

inline constexpr Keyword WITH = Keyword(
    153,
    "with",
    "WITH",
    KeywordStyle::RESERVED
);

inline constexpr Keyword YIELD = Keyword(
    154,
    "yield",
    "YIELD",
    KeywordStyle::PSEUDO
);
} // namespace keyword

// TODO: Add docs after implementing and understanding `KeywordState`.
inline constexpr std::array<const keyword::Keyword*, keyword::KEYWORD_COUNT> 
ALL_KEYWORDS {
    &keyword::ABSTRACT,
    &keyword::AS,
    &keyword::ASSERT,
    &keyword::ASYNC,
    &keyword::AUGMENT,
    &keyword::AWAIT,
    &keyword::BASE,
    &keyword::BREAK,
    &keyword::CASE,
    &keyword::CATCH,
    &keyword::CLASS,
    &keyword::CONST,
    &keyword::CONTINUE,
    &keyword::COVARIANT,
    &keyword::DEFAULT,
    &keyword::DEFERRED,
    &keyword::DO,
    &keyword::DYNAMIC,
    &keyword::ELSE,
    &keyword::ENUM,
    &keyword::EXPORT,
    &keyword::EXTENDS,
    &keyword::EXTENSION,
    &keyword::EXTERNAL,
    &keyword::FACTORY,
    &keyword::FALSE,
    &keyword::FINAL,
    &keyword::FINALLY,
    &keyword::FOR,
    &keyword::FUNCTION,
    &keyword::GET,
    &keyword::HIDE,
    &keyword::IF,
    &keyword::IMPLEMENTS,
    &keyword::IMPORT,
    &keyword::IN,
    &keyword::INOUT,
    &keyword::INTERFACE,
    &keyword::IS,
    &keyword::LATE,
    &keyword::LIBRARY,
    &keyword::MIXIN,
    &keyword::NATIVE,
    &keyword::NEW,
    &keyword::NIL,
    &keyword::OF,
    &keyword::ON,
    &keyword::OPERATOR,
    &keyword::OUT,
    &keyword::PART,
    &keyword::PATCH,
    &keyword::REQUIRED,
    &keyword::RETHROW,
    &keyword::RETURN,
    &keyword::SEALED,
    &keyword::SET,
    &keyword::SHOW,
    &keyword::SOURCE,
    &keyword::STATIC,
    &keyword::SUPER,
    &keyword::SWITCH,
    &keyword::SYNC,
    &keyword::THIS,
    &keyword::THROW,
    &keyword::TRUE,
    &keyword::TRY,
    &keyword::TYPEDEF,
    &keyword::VAR,
    &keyword::VOID,
    &keyword::WHEN,
    &keyword::WHILE,
    &keyword::WITH,
    &keyword::YIELD
};

namespace type {
/*
The total number of both `Keyword` and `TokenType` constant definitions.
*/
inline constexpr std::size_t TOTAL_COUNT =
    TOKENTYPE_COUNT + keyword::KEYWORD_COUNT;

/*
Contains all the `TokenType` and `Keyword` constants in a single array, ordered
by their `TokenType.index` value (from 0 - 154).

The `SimpleToken` class and its subclasses use this array as a way to quickly
find a specific token type or keyword via its index.
*/
inline constexpr std::array<const TokenType*, TOTAL_COUNT> ALL_TOKENTYPES{
    &END_OF_FILE,
    &DOUBLE,
    &DOUBLE_WITH_SEPARATORS,
    &HEXADECIMAL,
    &HEXADECIMAL_WITH_SEPARATORS,
    &IDENTIFIER,
    &INT,
    &INT_WITH_SEPARATORS,
    &MULTI_LINE_COMMENT,
    &SCRIPT_TAG,
    &SINGLE_LINE_COMMENT,
    &STRING,
    &AMPERSAND,
    &AMPERSAND_AMPERSAND,
    &AMPERSAND_AMPERSAND_EQ,
    &AMPERSAND_EQ,
    &AT,
    &BANG,
    &BANG_EQ,
    &BANG_EQ_EQ,
    &BAR,
    &BAR_BAR,
    &BAR_BAR_EQ,
    &BAR_EQ,
    &COLON,
    &COMMA,
    &CARET,
    &CARET_EQ,
    &CLOSE_CURLY_BRACKET,
    &CLOSE_PAREN,
    &CLOSE_SQUARE_BRACKET,
    &EQ,
    &EQ_EQ,
    &EQ_EQ_EQ,
    &FUNCTION,
    &GT,
    &GT_EQ,
    &GT_GT,
    &GT_GT_EQ,
    &GT_GT_GT,
    &GT_GT_GT_EQ,
    &HASH,
    &INDEX,
    &INDEX_EQ,
    &LT,
    &LT_EQ,
    &LT_LT,
    &LT_LT_EQ,
    &MINUS,
    &MINUS_EQ,
    &MINUS_MINUS,
    &OPEN_CURLY_BRACKET,
    &OPEN_PAREN,
    &OPEN_SQUARE_BRACKET,
    &PERCENT,
    &PERCENT_EQ,
    &PERIOD,
    &PERIOD_PERIOD,
    &PLUS,
    &PLUS_EQ,
    &PLUS_PLUS,
    &QUESTION,
    &QUESTION_PERIOD,
    &QUESTION_QUESTION,
    &QUESTION_QUESTION_EQ,
    &SEMICOLON,
    &SLASH,
    &SLASH_EQ,
    &STAR,
    &STAR_EQ,
    &STRING_INTERPOLATION_EXPRESSION,
    &STRING_INTERPOLATION_IDENTIFIER,
    &TILDE,
    &TILDE_SLASH,
    &TILDE_SLASH_EQ,
    &BACKPING,
    &BACKSLASH,
    &PERIOD_PERIOD_PERIOD,
    &PERIOD_PERIOD_PERIOD_QUESTION,
    &QUESTION_PERIOD_PERIOD,
    &BAD_INPUT,
    &RECOVERY,

    &keyword::ABSTRACT,
    &keyword::AS,
    &keyword::ASSERT,
    &keyword::ASYNC,
    &keyword::AUGMENT,
    &keyword::AWAIT,
    &keyword::BASE,
    &keyword::BREAK,
    &keyword::CASE,
    &keyword::CATCH,
    &keyword::CLASS,
    &keyword::CONST,
    &keyword::CONTINUE,
    &keyword::COVARIANT,
    &keyword::DEFAULT,
    &keyword::DEFERRED,
    &keyword::DO,
    &keyword::DYNAMIC,
    &keyword::ELSE,
    &keyword::ENUM,
    &keyword::EXPORT,
    &keyword::EXTENDS,
    &keyword::EXTENSION,
    &keyword::EXTERNAL,
    &keyword::FACTORY,
    &keyword::FALSE,
    &keyword::FINAL,
    &keyword::FINALLY,
    &keyword::FOR,
    &keyword::FUNCTION,
    &keyword::GET,
    &keyword::HIDE,
    &keyword::IF,
    &keyword::IMPLEMENTS,
    &keyword::IMPORT,
    &keyword::IN,
    &keyword::INOUT,
    &keyword::INTERFACE,
    &keyword::IS,
    &keyword::LATE,
    &keyword::LIBRARY,
    &keyword::MIXIN,
    &keyword::NATIVE,
    &keyword::NEW,
    &keyword::NIL,
    &keyword::OF,
    &keyword::ON,
    &keyword::OPERATOR,
    &keyword::OUT,
    &keyword::PART,
    &keyword::PATCH,
    &keyword::REQUIRED,
    &keyword::RETHROW,
    &keyword::RETURN,
    &keyword::SEALED,
    &keyword::SET,
    &keyword::SHOW,
    &keyword::SOURCE,
    &keyword::STATIC,
    &keyword::SUPER,
    &keyword::SWITCH,
    &keyword::SYNC,
    &keyword::THIS,
    &keyword::THROW,
    &keyword::TRUE,
    &keyword::TRY,
    &keyword::TYPEDEF,
    &keyword::VAR,
    &keyword::VOID,
    &keyword::WHEN,
    &keyword::WHILE,
    &keyword::WITH,
    &keyword::YIELD
};
// clang-format on
} // namespace type
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart