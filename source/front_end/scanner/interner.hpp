#pragma once

#include <string_view>

namespace dart {
namespace front_end {
namespace scanner {

/*
An interface for defining objects that can intern strings.

A string interner returns the same instance of a string copy without having to
store it. This can make it efficient in processing string that are of the same
value.
*/
class Interner {
public:
     Interner() = default;
    
     virtual ~Interner() = default;
    
    /*
    Returns a string reference that is an identical copy of all the strings that
    have been internet, equal to the given string value.
    */
    virtual std::string_view intern(std::string_view string) const = 0;
};

/*
A null interner that does not intern any string data.
*/
class NullInterner : public Interner {
public:
    virtual std::string_view intern(std::string_view string) const override {
        return string;
    }
};

} // namespace scanner
} // namespace front_end
} // namespace dart