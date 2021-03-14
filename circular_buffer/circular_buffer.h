#ifndef DEQUE_CIRCULAR_BUFFER_H
#define DEQUE_CIRCULAR_BUFFER_H

#include "cstddef"
#include <utility>

template <class T>
class CircularBuffer {
    T* buffer_;
    size_t capacity_;
    size_t head_;
    size_t tail_;

public:
    CircularBuffer();
    explicit CircularBuffer(const size_t&);
    CircularBuffer(const CircularBuffer<T>&);
    CircularBuffer<T>& operator=(const CircularBuffer<T>&);
    CircularBuffer(CircularBuffer<T>&&) noexcept;
    CircularBuffer<T>& operator=(CircularBuffer<T>&&) noexcept;
    ~CircularBuffer();
    T operator[](const size_t&) const;
    T& operator[](const size_t&);
    T Front() const;
    T& Front();
    T Back() const;
    T& Back();
    bool Empty() const;
    size_t Size() const;
    size_t Capacity() const;
    CircularBuffer<T>& Clear();
    CircularBuffer<T>& Reserve(const size_t&);
    CircularBuffer<T>& Realloc(const size_t&);
    CircularBuffer<T>& Swap(CircularBuffer<T>&);
    CircularBuffer<T>& PushBack(const T&);
    CircularBuffer<T>& PushFront(const T&);
    CircularBuffer<T>& PopBack();
    CircularBuffer<T>& PopFront();
};

template <class T>
CircularBuffer<T>::CircularBuffer() {
    capacity_ = 1;
    buffer_ = new T[capacity_];
    head_ = 0;
    tail_ = 0;
}

template <class T>
CircularBuffer<T>::CircularBuffer(const size_t& size) {
    buffer_ = new T[size];
    capacity_ = size;
    head_ = 0;
    tail_ = 0;
}

template <class T>
size_t CircularBuffer<T>::Size() const {
    if (head_ < tail_) {
        return tail_ - head_;
    }
    if (head_ == tail_) {
        return 0;
    }
    return capacity_ - head_ + tail_;
}

template <class T>
CircularBuffer<T>::CircularBuffer(const CircularBuffer<T>& circular_buffer) {
    capacity_ = circular_buffer.capacity_;
    buffer_ = new T[capacity_];
    for (size_t i = 0; i < capacity_; ++i) {
        buffer_[i] = circular_buffer.buffer_[i];
    }
    head_ = circular_buffer.head_;
    tail_ = circular_buffer.tail_;
}

template <class T>
CircularBuffer<T>::CircularBuffer(CircularBuffer<T>&& circular_buffer) noexcept
    : buffer_(circular_buffer.buffer_)
    , capacity_(circular_buffer.capacity_)
    , head_(circular_buffer.head_)
    , tail_(circular_buffer.tail_) {
    circular_buffer.head_ = circular_buffer.tail_ = circular_buffer.capacity_ = 0;
    circular_buffer.buffer_ = nullptr;
}

template <class T>
CircularBuffer<T>& CircularBuffer<T>::operator=(const CircularBuffer<T>& circular_buffer) {
    if (this == &circular_buffer) {
        return *this;
    }
    delete[] buffer_;
    capacity_ = circular_buffer.capacity_;
    buffer_ = new T[capacity_];
    for (size_t i = 0; i < capacity_; ++i) {
        buffer_[i] = circular_buffer.buffer_[i];
    }
    head_ = circular_buffer.head_;
    tail_ = circular_buffer.tail_;
    return *this;
}

template <class T>
CircularBuffer<T>& CircularBuffer<T>::operator=(CircularBuffer<T>&& circular_buffer) noexcept {
    if (this != &circular_buffer) {
        delete[] buffer_;
        tail_ = circular_buffer.tail_;
        head_ = circular_buffer.head_;
        capacity_ = circular_buffer.capacity_;
        buffer_ = circular_buffer.buffer_;
        circular_buffer.head_ = circular_buffer.tail_ = circular_buffer.capacity_ = 0;
        circular_buffer.buffer_ = nullptr;
    }
    return *this;
}

template <class T>
CircularBuffer<T>::~CircularBuffer() {
    delete[] buffer_;
}

template <class T>
T& CircularBuffer<T>::operator[](const size_t& index) {
    return buffer_[(head_ + index) % capacity_];
}

template <class T>
T CircularBuffer<T>::operator[](const size_t& index) const {
    return buffer_[(head_ + index) % capacity_];
}

template <class T>
T& CircularBuffer<T>::Front() {
    return buffer_[head_];
}

template <class T>
T CircularBuffer<T>::Front() const {
    return buffer_[head_];
}

template <class T>
T& CircularBuffer<T>::Back() {
    return buffer_[(tail_ - 1 + capacity_) % capacity_];
}

template <class T>
T CircularBuffer<T>::Back() const {
    return buffer_[(tail_ - 1 + capacity_) % capacity_];
}

template <class T>
bool CircularBuffer<T>::Empty() const {
    return Size() == 0;
}

template <class T>
size_t CircularBuffer<T>::Capacity() const {
    return capacity_;
}

template <class T>
CircularBuffer<T>& CircularBuffer<T>::Clear() {
    head_ = 0;
    tail_ = 0;
    return *this;
}

template <class T>
CircularBuffer<T>& CircularBuffer<T>::Realloc(const size_t& cap) {
    T* buffer = new T[cap];
    for (size_t i = 0; i < Size(); ++i) {
        buffer[i] = buffer_[(head_ + i) % capacity_];
    }
    delete[] buffer_;
    buffer_ = buffer;
    tail_ = Size();
    head_ = 0;
    capacity_ = cap;
    return *this;
}

template <class T>
CircularBuffer<T>& CircularBuffer<T>::Reserve(const size_t& new_cap) {
    if (new_cap > capacity_) {
        this->Realloc(new_cap);
    }
    return *this;
}

template <class T>
CircularBuffer<T>& CircularBuffer<T>::Swap(CircularBuffer<T>& other) {
    CircularBuffer<T> tmp = std::move(*this);
    *this = std::move(other);
    other = std::move(tmp);
    return *this;
}

template <class T>
CircularBuffer<T>& CircularBuffer<T>::PushBack(const T& value) {
    capacity_ += (capacity_ == 0);
    if (tail_ == head_ || (tail_ + 1) % capacity_ == head_) {
        this->Realloc(capacity_ * 2);
    }
    buffer_[tail_] = value;
    tail_ = (tail_ + 1) % capacity_;
    return *this;
}

template <class T>
CircularBuffer<T>& CircularBuffer<T>::PushFront(const T& value) {
    capacity_ += (capacity_ == 0);
    if (tail_ == head_ || (head_ - 1 + capacity_) % capacity_ == tail_) {
        this->Realloc(capacity_ * 2);
    }
    head_ = (head_ - 1 + capacity_) % capacity_;
    buffer_[head_] = value;
    return *this;
}

template <class T>
CircularBuffer<T>& CircularBuffer<T>::PopBack() {
    tail_ = (tail_ - 1 + capacity_) % capacity_;
    return *this;
}

template <class T>
CircularBuffer<T>& CircularBuffer<T>::PopFront() {
    head_ = (head_ + 1) % capacity_;
    return *this;
}

#endif  // DEQUE_CIRCULAR_BUFFER_H