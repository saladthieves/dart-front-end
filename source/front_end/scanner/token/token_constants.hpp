#pragma once

#include "characters.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
namespace constants {
// clang-format off
inline constexpr dart::u8 EOF_TOKEN = 0;

inline constexpr dart::u8 KEYWORD_TOKEN     = chars::$k;
inline constexpr dart::u8 IDENTIFIER_TOKEN  = chars::$a;
inline constexpr dart::u8 SCRIPT_TOKEN      = chars::$b;
inline constexpr dart::u8 BAD_INPUT_TOKEN   = chars::$X;
inline constexpr dart::u8 DOUBLE_TOKEN      = chars::$d;
inline constexpr dart::u8 INT_TOKEN         = chars::$i;
inline constexpr dart::u8 RECOVERY_TOKEN    = chars::$r;
inline constexpr dart::u8 HEXADECIMAL_TOKEN = chars::$x;
inline constexpr dart::u8 STRING_TOKEN      = chars::$SQ;

inline constexpr dart::u8 AMPERSAND_TOKEN               = chars::$AMPERSAND;
inline constexpr dart::u8 BACKPING_TOKEN                = chars::$BACKPING;
inline constexpr dart::u8 BACKSLASH_TOKEN               = chars::$BACKSLASH;
inline constexpr dart::u8 BANG_TOKEN                    = chars::$BANG;
inline constexpr dart::u8 BAR_TOKEN                     = chars::$BAR;
inline constexpr dart::u8 COLON_TOKEN                   = chars::$COLON;
inline constexpr dart::u8 COMMA_TOKEN                   = chars::$COMMA;
inline constexpr dart::u8 EQ_TOKEN                      = chars::$EQ;
inline constexpr dart::u8 GT_TOKEN                      = chars::$GT;
inline constexpr dart::u8 HASH_TOKEN                    = chars::$HASH;
inline constexpr dart::u8 OPEN_CURLY_BRACKET_TOKEN      = chars::$OPEN_CURLY_BRACKET;
inline constexpr dart::u8 OPEN_SQUARE_BRACKET_TOKEN     = chars::$OPEN_SQUARE_BRACKET;
inline constexpr dart::u8 OPEN_PAREN_TOKEN              = chars::$OPEN_PAREN;
inline constexpr dart::u8 LT_TOKEN                      = chars::$LT;
inline constexpr dart::u8 MINUS_TOKEN                   = chars::$MINUS;
inline constexpr dart::u8 PERIOD_TOKEN                  = chars::$PERIOD;
inline constexpr dart::u8 PLUS_TOKEN                    = chars::$PLUS;
inline constexpr dart::u8 QUESTION_TOKEN                = chars::$QUESTION;
inline constexpr dart::u8 AT_TOKEN                      = chars::$AT;
inline constexpr dart::u8 CLOSE_CURLY_BRACKET_TOKEN     = chars::$CLOSE_CURLY_BRACKET;
inline constexpr dart::u8 CLOSE_SQUARE_BRACKET_TOKEN    = chars::$CLOSE_SQUARE_BRACKET;
inline constexpr dart::u8 CLOSE_PAREN_TOKEN             = chars::$CLOSE_PAREN;
inline constexpr dart::u8 SEMICOLON_TOKEN               = chars::$SEMICOLON;
inline constexpr dart::u8 SLASH_TOKEN                   = chars::$SLASH;
inline constexpr dart::u8 TILDE_TOKEN                   = chars::$TILDE;
inline constexpr dart::u8 STAR_TOKEN                    = chars::$STAR;
inline constexpr dart::u8 PERCENT_TOKEN                 = chars::$PERCENT;
inline constexpr dart::u8 CARET_TOKEN                   = chars::$CARET;

inline constexpr dart::u8 STRING_INTERPOLATION_TOKEN = 128;
inline constexpr dart::u8 LT_EQ_TOKEN = STRING_INTERPOLATION_TOKEN + 1;
inline constexpr dart::u8 FUNCTION_TOKEN = LT_EQ_TOKEN + 1;
inline constexpr dart::u8 SLASH_EQ_TOKEN = FUNCTION_TOKEN + 1;
inline constexpr dart::u8 PERIOD_PERIOD_PERIOD_TOKEN = SLASH_EQ_TOKEN + 1;
inline constexpr dart::u8 PERIOD_PERIOD_TOKEN = PERIOD_PERIOD_PERIOD_TOKEN + 1;
inline constexpr dart::u8 EQ_EQ_EQ_TOKEN = PERIOD_PERIOD_TOKEN + 1;
inline constexpr dart::u8 EQ_EQ_TOKEN = EQ_EQ_EQ_TOKEN + 1;
inline constexpr dart::u8 LT_LT_EQ_TOKEN = EQ_EQ_TOKEN + 1;
inline constexpr dart::u8 LT_LT_TOKEN = LT_LT_EQ_TOKEN + 1;
inline constexpr dart::u8 GT_EQ_TOKEN = LT_LT_TOKEN + 1;
inline constexpr dart::u8 GT_GT_EQ_TOKEN = GT_EQ_TOKEN + 1;
inline constexpr dart::u8 INDEX_EQ_TOKEN = GT_GT_EQ_TOKEN + 1;
inline constexpr dart::u8 INDEX_TOKEN = INDEX_EQ_TOKEN + 1;
inline constexpr dart::u8 BANG_EQ_EQ_TOKEN = INDEX_TOKEN + 1;
inline constexpr dart::u8 BANG_EQ_TOKEN = BANG_EQ_EQ_TOKEN + 1;
inline constexpr dart::u8 AMPERSAND_AMPERSAND_TOKEN = BANG_EQ_TOKEN + 1;
inline constexpr dart::u8 AMPERSAND_AMPERSAND_EQ_TOKEN = AMPERSAND_AMPERSAND_TOKEN + 1;
inline constexpr dart::u8 AMPERSAND_EQ_TOKEN = AMPERSAND_AMPERSAND_EQ_TOKEN + 1;
inline constexpr dart::u8 BAR_BAR_TOKEN = AMPERSAND_EQ_TOKEN + 1;
inline constexpr dart::u8 BAR_BAR_EQ_TOKEN = BAR_BAR_TOKEN + 1;
inline constexpr dart::u8 BAR_EQ_TOKEN = BAR_BAR_EQ_TOKEN + 1;
inline constexpr dart::u8 STAR_EQ_TOKEN = BAR_EQ_TOKEN + 1;
inline constexpr dart::u8 PLUS_PLUS_TOKEN = STAR_EQ_TOKEN + 1;
inline constexpr dart::u8 PLUS_EQ_TOKEN = PLUS_PLUS_TOKEN + 1;
inline constexpr dart::u8 MINUS_MINUS_TOKEN = PLUS_EQ_TOKEN + 1;
inline constexpr dart::u8 MINUS_EQ_TOKEN = MINUS_MINUS_TOKEN + 1;
inline constexpr dart::u8 TILDE_SLASH_EQ_TOKEN = MINUS_EQ_TOKEN + 1;
inline constexpr dart::u8 TILDE_SLASH_TOKEN = TILDE_SLASH_EQ_TOKEN + 1;
inline constexpr dart::u8 PERCENT_EQ_TOKEN = TILDE_SLASH_TOKEN + 1;
inline constexpr dart::u8 GT_GT_TOKEN = PERCENT_EQ_TOKEN + 1;
inline constexpr dart::u8 CARET_EQ_TOKEN = GT_GT_TOKEN + 1;
inline constexpr dart::u8 COMMENT_TOKEN = CARET_EQ_TOKEN + 1;
inline constexpr dart::u8 STRING_INTERPOLATION_IDENTIFIER_TOKEN = COMMENT_TOKEN + 1;
inline constexpr dart::u8 QUESTION_PERIOD_TOKEN = STRING_INTERPOLATION_IDENTIFIER_TOKEN + 1;
inline constexpr dart::u8 QUESTION_QUESTION_TOKEN = QUESTION_PERIOD_TOKEN + 1;
inline constexpr dart::u8 QUESTION_QUESTION_EQ_TOKEN = QUESTION_QUESTION_TOKEN + 1;
inline constexpr dart::u8 GENERIC_METHOD_TYPE_ASSIGN_TOKEN = QUESTION_QUESTION_EQ_TOKEN + 1;
inline constexpr dart::u8 GENERIC_METHOD_TYPE_LIST_TOKEN = GENERIC_METHOD_TYPE_ASSIGN_TOKEN + 1;
inline constexpr dart::u8 GT_GT_GT_TOKEN = GENERIC_METHOD_TYPE_LIST_TOKEN + 1;
inline constexpr dart::u8 PERIOD_PERIOD_PERIOD_QUESTION_TOKEN = GT_GT_GT_TOKEN + 1;
inline constexpr dart::u8 GT_GT_GT_EQ_TOKEN = PERIOD_PERIOD_PERIOD_QUESTION_TOKEN + 1;
inline constexpr dart::u8 QUESTION_PERIOD_PERIOD_TOKEN = GT_GT_GT_EQ_TOKEN+ 1;
// clang-format on
} // namespace constants
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart