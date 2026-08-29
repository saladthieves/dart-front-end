#pragma once

namespace dart {
namespace util {

// TODO: Add docs
template <typename T>
class Link {
public:
    explicit Link(): Link{nullptr, nullptr} { }

    explicit Link(T* head, Link<T>* tail): head{head}, tail{tail} { }

    Link<T>* prepend(T* head);

    bool isEmpty() const { return head == nullptr && tail == nullptr; }

    bool isNotEmpty() const { return !isEmpty(); }

    T* head;
    Link<T>* tail;
};

template <typename T>
Link<T>* Link<T>::prepend(T* head) {
    return new Link<T>(head, this);
}

} // namespace util
} // namespace dart