#pragma once

#include "source_range.hpp"

namespace dart {
namespace front_end {
namespace base {
/*
A syntactic entity represents an object (such as a token or AST node) with a
location and an extent within the source file.
*/
class SyntacticEntity {
public:
    virtual ~SyntacticEntity() = default;

    /*
    Returns the offset from the beginning of the file to the first character in
    the entity's source range.
    */
    virtual std::size_t getOffset() const = 0;

    /*
    Returns the number of characters within the entity's source range.
    */
    virtual std::size_t getLength() const = 0;

    /*
    Returns the offset from the beginning of the file to the position one past
    the last character in the entity's source range.
    */
    virtual std::size_t getEnd() const = 0;

    /*
    Returns a `SourceRange` representing the range of characters this entity
    spans.
    */
    SourceRange sourceRange() const {
        return SourceRange(getOffset(), getLength());
    }
};
} // namespace base
} // namespace front_end
} // namespace dart