#include "error_token.hpp"
#include "characters.hpp"

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
ErrorToken*
buildUnexpectedCharacterToken(dart::u16 character, std::size_t offset) {
    if (character < 0x1F) { // It's an ASCII control character
        return new AsciiControlCharacterToken(character, offset);
    }

    switch (character) {
        case chars::$UNICODE_REPLACEMENT_CHAR: // It's an encoding error (�)
            return new EncodingErrorToken(offset);

        // It's one of the non-ASCII whitespace characters
        case 0x00A0: // No-break space.
        case 0x1680: // Ogham space mark.
        case 0x180E: // Mongolian vowel separator.
        case 0x2000: // En quad.
        case 0x2001: // Em quad.
        case 0x2002: // En space.
        case 0x2003: // Em space.
        case 0x2004: // Three-per-em space.
        case 0x2005: // Four-per-em space.
        case 0x2006: // Six-per-em space.
        case 0x2007: // Figure space.
        case 0x2008: // Punctuation space.
        case 0x2009: // Thin space.
        case 0x200A: // Hair space.
        case 0x200B: // Zero width space.
        case 0x2028: // Line separator.
        case 0x2029: // Paragraph separator.
        case 0x202F: // Narrow no-break space.
        case 0x205F: // Medium mathematical space.
        case 0x3000: // Ideographic space.
        case 0xFEFF: // Zero width no-break space.
            return new NonAsciiWhitespaceToken(character, offset);

        // It's some other non-ASCII character
        default: return new NonAsciiIdentifierToken(character, offset);
    }
}
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart