#ifndef DEQUE_DEQUE_H
#define DEQUE_DEQUE_H

#include <page.h>
#include <circular_buffer.h>
#include <iostream>
#include <memory>

const int kPageSize = 100;

template <class T>
class Deque {
    CircularBuffer<std::unique_ptr<Page<T, kPageSize>>> buffer_;

public:
    Deque();
    Deque(const Deque<T>&);
    Deque<T>& operator=(const Deque<T>&);
    ~Deque() = default;
    Deque<T>& Swap(Deque<T>&);
    const T& operator[](size_t) const;
    T& operator[](size_t);
    size_t Size() const;
    Deque<T>& PushBack(const T&);
    Deque<T>& PushFront(const T&);
    Deque<T>& PopBack();
    Deque<T>& PopFront();
    Deque<T>& Clear();
};

template <class T>
Deque<T>::Deque() {
    buffer_[0] = nullptr;
}

template <class T>
Deque<T>::Deque(const Deque<T>& deque) : buffer_(deque.buffer_) {
    for (size_t i = 0; i < deque.buffer_.Size(); ++i) {
        buffer_[i] = std::make_unique<Page<T, kPageSize>>(*deque.buffer_[i]);
    }
}

template <class T>
Deque<T>& Deque<T>::operator=(const Deque<T>& deque) {
    if (this == &deque) {
        return *this;
    }
    buffer_ = deque.buffer_;
    for (size_t i = 0; i < deque.buffer_.Size(); ++i) {
        buffer_[i] = std::make_unique<Page<T, kPageSize>>(*deque.buffer_[i]);
    }
    return *this;
}

template <class T>
size_t Deque<T>::Size() const {
    if (buffer_.Size() == 0) {
        return 0;
    }
    if (buffer_.Size() == 1) {
        return buffer_.Back()->Size();
    }
    if (buffer_.Size() == 2) {
        return buffer_.Front()->Size() + buffer_.Back()->Size();
    }
    return (buffer_.Size() - 2) * kPageSize + buffer_.Front()->Size() + buffer_.Back()->Size();
}

template <class T>
Deque<T>& Deque<T>::Swap(Deque<T>& other) {
    buffer_.Swap(other.buffer_);
    return *this;
}

template <class T>
Deque<T>& Deque<T>::Clear() {
    buffer_.Clear();
    buffer_[0] = nullptr;
    return *this;
}

template <class T>
T& Deque<T>::operator[](size_t index) {
    size_t size = buffer_.Front()->Size();
    if (size > index) {
        return (*(buffer_[index / kPageSize]))[index % kPageSize];
    }
    return (*(buffer_[1 + ((index - size) / kPageSize)]))[(index - size) % kPageSize];
}

template <class T>
const T& Deque<T>::operator[](size_t index) const {
    size_t size = buffer_.Front()->Size();
    if (size > index) {
        return (*(buffer_[index / kPageSize]))[index % kPageSize];
    }
    return (*(buffer_[1 + ((index - size) / kPageSize)]))[(index - size) % kPageSize];
}

template <class T>
Deque<T>& Deque<T>::PushBack(const T& value) {
    if (buffer_[0] == nullptr || buffer_.Back()->Full()) {
        buffer_.PushBack(std::unique_ptr<Page<T, kPageSize>>(new Page<T, kPageSize>));
    }
    buffer_.Back()->PushBack(value);
    return *this;
}

template <class T>
Deque<T>& Deque<T>::PushFront(const T& value) {
    if (buffer_[0] == nullptr || buffer_.Front()->Full()) {
        buffer_.PushFront(std::unique_ptr<Page<T, kPageSize>>(new Page<T, kPageSize>));
    }
    buffer_.Front()->PushFront(value);
    return *this;
}

template <class T>
Deque<T>& Deque<T>::PopBack() {
    if (buffer_.Back()->Empty()) {
        buffer_.PopBack();
    }
    buffer_.Back()->PopBack();
    return *this;
}

template <class T>
Deque<T>& Deque<T>::PopFront() {
    if (buffer_.Front()->Empty()) {
        buffer_.PopFront();
    }
    buffer_.Front()->PopFront();
    return *this;
}

#endif  // DEQUE_DEQUE_H