#pragma once

#include "common/types.hpp"

namespace dart {
namespace front_end {
namespace messages {
namespace severity {
/*
Severity levels of the common-front-end, from highest to lowest.
*/
enum class CfeSeverity : dart::u8 {
    CONTEXT = 1,
    ERROR,
    IGNORED,
    INTERNAL_PROBLEM,
    WARNING,
    INFO
};
} // namespace severity
} // namespace messages
} // namespace front_end
} // namespace dart