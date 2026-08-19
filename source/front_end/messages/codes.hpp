#pragma once

#include "diagnostic.hpp"
#include "severity.hpp"

#include <map>
#include <string>

namespace dart {
namespace front_end {
namespace messages {
namespace codes {

// TODO: Add docs
class Code {
public:
    explicit constexpr Code(
        std::string_view name,
        PseudoSharedCode pseudoSharedCode = PseudoSharedCode::NONE,
        severity::CfeSeverity severity = severity::CfeSeverity::ERROR,
        SharedCode sharedCode = SharedCode::NONE
    )
        : name{name},
          pseudoSharedCode{pseudoSharedCode},
          severity{severity},
          sharedCode{sharedCode} {}

    /*
    A textual representation of the code.
    */
    const std::string_view name;

    /*
    An enum value used to map this `Code` object to an analyzer code.

    If this value is not `NONE`, the analyzer implementation can use it to map
    to a corresponding analyzer code.

    The value isn't actually shared, but rather a pseudo version of `sharedCode`
    which is actually shared.
    */
    const PseudoSharedCode pseudoSharedCode;

    /*
    The severity level of this code.
    */
    const severity::CfeSeverity severity;

    /*
    An enum value used to map this `Code` to a corresponding analyzer code.

    Unlike `pseudoSharedCode`, this error value is shared between the analyzer
    and the common-front-end (CFE), if not `NONE`.
    */
    const SharedCode sharedCode;
};

// TODO: Add docs
class Message {
public:
    using Args = std::map<std::string_view, std::string>;

    Message(
        Code code,
        std::string_view problemMessage,
        const Args& arguments = {},
        std::string_view correctionMessage = ""
    )
        : code{code},
          problemMessage{problemMessage},
          arguments{arguments},
          correctionMessage{correctionMessage} {}

    Code code;
    std::string problemMessage;
    Args arguments;
    std::string correctionMessage;
};
} // namespace codes
} // namespace messages
} // namespace front_end
} // namespace dart