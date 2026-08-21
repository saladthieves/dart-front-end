#pragma once

#include <format>
#include <stdexcept>
#include <string_view>

namespace dart {
namespace front_end {
namespace scanner {
namespace token {
/*
Returns a closing brace for the provided opening brace, or throws a
`std::logic_error` if the opening brace is unknown.
*/
inline std::string_view closeBraceFor(std::string_view open) {
    if (open == "(") return ")";
    if (open == "[") return "]";
    if (open == "{") return "}";
    if (open == "<") return ">";
    if (open == "${") return "}";

    throw std::logic_error(std::format("Unknown opening brace: `{}`", open));
}

/*
Returns a closing string quote for the provided opening quote or throws a
`std::logic_error` if the opening quote is unknown.
*/
inline std::string_view closeQuoteFor(std::string_view open) {
    if (open == "\"") return open;
    if (open == "'") return open;
    if (open == "\"\"\"") return open;
    if (open == "'''") return open;
    if (open == "r\"") return "\"";
    if (open == "r'") return "'";
    if (open == "r\"\"\"") return "\"\"\"";
    if (open == "r'''") return "'''";

    throw std::logic_error(std::format("Unknown opening quote: `{}`", open));
}
} // namespace token
} // namespace scanner
} // namespace front_end
} // namespace dart