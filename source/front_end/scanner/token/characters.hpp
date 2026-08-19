#pragma once

#include "common/errors.hpp"
#include "common/types.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
namespace chars {
/* ASCII control characters */
inline constexpr dart::u8 $EOF = 0;   // EOF ([unused code] instead of -1)
inline constexpr dart::u8 $STX = 2;   // Start of text
inline constexpr dart::u8 $BS = 8;    // Backspace
inline constexpr dart::u8 $TAB = 9;   // Horizontal tab
inline constexpr dart::u8 $LF = 10;   // Line feed
inline constexpr dart::u8 $VTAB = 11; // Vertical tab
inline constexpr dart::u8 $FF = 12;   // Form feed
inline constexpr dart::u8 $CR = 13;   // Carriage return

/* ASCII printable characters */
inline constexpr dart::u8 $SPACE = ' ';       // Space
inline constexpr dart::u8 $BANG = '!';        // Exclamation mark
inline constexpr dart::u8 $DQ = '"';          // Double quotation marks
inline constexpr dart::u8 $HASH = '#';        // Number sign / hash
inline constexpr dart::u8 $$ = '$';           // Dollar
inline constexpr dart::u8 $PERCENT = '%';     // Percent
inline constexpr dart::u8 $AMPERSAND = '&';   // Ampersand
inline constexpr dart::u8 $SQ = '\'';         // Single quote
inline constexpr dart::u8 $OPEN_PAREN = '(';  // Open parenthesis / bracket
inline constexpr dart::u8 $CLOSE_PAREN = ')'; // Close parenthesis / bracket
inline constexpr dart::u8 $STAR = '*';        // Star / asterisk
inline constexpr dart::u8 $PLUS = '+';        // Plus
inline constexpr dart::u8 $COMMA = ',';       // Comma
inline constexpr dart::u8 $MINUS = '-';       // Minus
inline constexpr dart::u8 $PERIOD = '.';      // Period, dot or full stop
inline constexpr dart::u8 $SLASH = '/';       // Slash or divide

inline constexpr dart::u8 $0 = '0';
inline constexpr dart::u8 $1 = '1';
inline constexpr dart::u8 $2 = '2';
inline constexpr dart::u8 $3 = '3';
inline constexpr dart::u8 $4 = '4';
inline constexpr dart::u8 $5 = '5';
inline constexpr dart::u8 $6 = '6';
inline constexpr dart::u8 $7 = '7';
inline constexpr dart::u8 $8 = '8';
inline constexpr dart::u8 $9 = '9';

inline constexpr dart::u8 $COLON = ':';     // Colon
inline constexpr dart::u8 $SEMICOLON = ';'; // Semicolon
inline constexpr dart::u8 $LT = '<';        // Less than
inline constexpr dart::u8 $EQ = '=';        // Equals
inline constexpr dart::u8 $GT = '>';        // Greater than
inline constexpr dart::u8 $QUESTION = '?';  // Question mark
inline constexpr dart::u8 $AT = '@';        // At symbol

inline constexpr dart::u8 $A = 'A';
inline constexpr dart::u8 $B = 'B';
inline constexpr dart::u8 $C = 'C';
inline constexpr dart::u8 $D = 'D';
inline constexpr dart::u8 $E = 'E';
inline constexpr dart::u8 $F = 'F';
inline constexpr dart::u8 $G = 'G';
inline constexpr dart::u8 $H = 'H';
inline constexpr dart::u8 $I = 'I';
inline constexpr dart::u8 $J = 'J';
inline constexpr dart::u8 $K = 'K';
inline constexpr dart::u8 $L = 'L';
inline constexpr dart::u8 $M = 'M';
inline constexpr dart::u8 $N = 'N';
inline constexpr dart::u8 $O = 'O';
inline constexpr dart::u8 $P = 'P';
inline constexpr dart::u8 $Q = 'Q';
inline constexpr dart::u8 $R = 'R';
inline constexpr dart::u8 $S = 'S';
inline constexpr dart::u8 $T = 'T';
inline constexpr dart::u8 $U = 'U';
inline constexpr dart::u8 $V = 'V';
inline constexpr dart::u8 $W = 'W';
inline constexpr dart::u8 $X = 'X';
inline constexpr dart::u8 $Y = 'Y';
inline constexpr dart::u8 $Z = 'Z';

inline constexpr dart::u8 $OPEN_SQUARE_BRACKET = '[';  // Opening bracket
inline constexpr dart::u8 $BACKSLASH = '\\';           // Backslash
inline constexpr dart::u8 $CLOSE_SQUARE_BRACKET = ']'; // Closing bracket
inline constexpr dart::u8 $CARET = '^';                // Caret / circumflex
inline constexpr dart::u8 $_ = '_';                    // Underscore
inline constexpr dart::u8 $BACKPING = '`';             // Grave accent

inline constexpr dart::u8 $a = 'a';
inline constexpr dart::u8 $b = 'b';
inline constexpr dart::u8 $c = 'c';
inline constexpr dart::u8 $d = 'd';
inline constexpr dart::u8 $e = 'e';
inline constexpr dart::u8 $f = 'f';
inline constexpr dart::u8 $g = 'g';
inline constexpr dart::u8 $h = 'h';
inline constexpr dart::u8 $i = 'i';
inline constexpr dart::u8 $j = 'j';
inline constexpr dart::u8 $k = 'k';
inline constexpr dart::u8 $l = 'l';
inline constexpr dart::u8 $m = 'm';
inline constexpr dart::u8 $n = 'n';
inline constexpr dart::u8 $o = 'o';
inline constexpr dart::u8 $p = 'p';
inline constexpr dart::u8 $q = 'q';
inline constexpr dart::u8 $r = 'r';
inline constexpr dart::u8 $s = 's';
inline constexpr dart::u8 $t = 't';
inline constexpr dart::u8 $u = 'u';
inline constexpr dart::u8 $v = 'v';
inline constexpr dart::u8 $w = 'w';
inline constexpr dart::u8 $x = 'x';
inline constexpr dart::u8 $y = 'y';
inline constexpr dart::u8 $z = 'z';

inline constexpr dart::u8 $OPEN_CURLY_BRACKET = '{';  // Opening brace
inline constexpr dart::u8 $BAR = '|';                 // Vertical bar
inline constexpr dart::u8 $CLOSE_CURLY_BRACKET = '}'; // Closing brace
inline constexpr dart::u8 $TILDE = '~';               // Equivalency sign
inline constexpr dart::u8 $DEL = 127;                 // Delete

/* ASCII extended codes */
inline constexpr dart::u8 $NBSP = 160; // Non-breaking space
inline constexpr dart::u16 $LS = 0x2028;
inline constexpr dart::u16 $PS = 0x2029;

inline constexpr dart::u16 $FIRST_SURROGATE = 0xD800;
inline constexpr dart::u16 $LAST_SURROGATE = 0xDFFF;
inline constexpr dart::u32 $LAST_CODE_POINT = 0x10FFFF;

/*
The unicode replacement character U+FFFD aka �
*/
inline constexpr dart::u16 $UNICODE_REPLACEMENT_CHAR = 0xFFFD;

inline constexpr bool isDigit(const dart::u8& code) {
    return $0 <= code && code <= $9;
}

inline constexpr bool isHexDigit(const dart::u8& code) {
    return std::isxdigit(code);
}

inline constexpr dart::u8 hexDigitValue(const dart::u8& hex) {
    assert::assert(isDigit(hex), "Character '{}' is not hexadecimal", hex);

    if (hex <= $9) return hex - $0;
    return (hex | ($a ^ $A)) - ($a - 10); // TODO: Learn how this works
}
} // namespace chars
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart