#pragma once

namespace dart {
namespace util {

// TODO: Add docs
template <typename T>
class Link {
public:
    explicit Link(const T* head = nullptr, const Link<T>* tail = nullptr)
        : head{head},
          tail{tail} {}

    bool isEmpty() const { return head == nullptr && tail == nullptr; }

    bool isNotEmpty() const { return !isEmpty(); }

    const T* head;
    const Link<T>* tail;
};

} // namespace util
} // namespace dart