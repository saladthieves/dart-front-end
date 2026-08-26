#pragma once

namespace dart {
namespace util {

// TODO: Add docs
template <typename T>
class Link {
public:
    explicit Link(T* head = nullptr, Link<T>* tail = nullptr)
        : head{head},
          tail{tail} { }

    bool isEmpty() const { return head == nullptr && tail == nullptr; }

    bool isNotEmpty() const { return !isEmpty(); }

    T* head{nullptr};
    Link<T>* tail{nullptr};
};

} // namespace util
} // namespace dart