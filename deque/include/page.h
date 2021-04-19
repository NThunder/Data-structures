#ifndef DEQUE_PAGE_PAGE_H
#define DEQUE_PAGE_PAGE_H

#include <utility>
#include <iostream>

template <class T, size_t N>
class Page {
    T buffer_[N];
    size_t head_;
    size_t tail_;
    size_t number_of_items_front_;
    size_t number_of_items_back_;

public:
    Page();
    Page(const Page<T, N>&);
    ~Page() = default;
    Page<T, N>& operator=(const Page<T, N>&);
    Page(Page<T, N>&&) noexcept;
    Page<T, N>& operator=(Page<T, N>&&) noexcept;
    const T& operator[](size_t) const;
    T& operator[](size_t);
    const T& Back() const;
    T& Back();
    const T& Front() const;
    T& Front();
    bool Empty() const;
    bool Full() const;
    size_t Size() const;
    bool IsBack() const;
    bool IsFront() const;
    Page<T, N>& Clear();
    Page<T, N>& PushBack(const T&);
    Page<T, N>& PushFront(const T&);
    Page<T, N>& PopBack();
    Page<T, N>& PopFront();
    Page<T, N>& Swap(Page<T, N>&);
};

template <class T, size_t N>
Page<T, N>::Page() {
    head_ = 0;
    tail_ = 0;
    number_of_items_front_ = 0;
    number_of_items_back_ = 0;
}

template <class T, size_t N>
size_t Page<T, N>::Size() const {
    return number_of_items_front_ + number_of_items_back_;
}

template <class T, size_t N>
Page<T, N>::Page(const Page<T, N>& page) {
    number_of_items_front_ = page.number_of_items_front_;
    number_of_items_back_ = page.number_of_items_back_;
    for (size_t i = 0; i < N; ++i) {
        buffer_[i] = page.buffer_[i];
    }
    head_ = page.head_;
    tail_ = page.tail_;
}

template <class T, size_t N>
Page<T, N>& Page<T, N>::operator=(const Page<T, N>& page) {
    number_of_items_front_ = page.number_of_items_front_;
    number_of_items_back_ = page.number_of_items_back_;
    for (size_t i = 0; i < N; ++i) {
        buffer_[i] = page.buffer_[i];
    }
    head_ = page.head_;
    tail_ = page.tail_;
    return *this;
}

template <class T, size_t N>
Page<T, N>::Page(Page<T, N>&& page) noexcept
    : head_(page.head_)
    , tail_(page.tail_)
    , number_of_items_front_(page.number_of_items_front_)
    , number_of_items_back_(page.number_of_items_back_) {
    for (size_t i = 0; i < N; ++i) {
        buffer_[i] = std::move(page.buffer_[i]);
    }
    page.Clear();
}

template <class T, size_t N>
Page<T, N>& Page<T, N>::operator=(Page<T, N>&& page) noexcept {
    if (this == &page) {
        return *this;
    }
    number_of_items_front_ = page.number_of_items_front_;
    number_of_items_back_ = page.number_of_items_back_;
    head_ = page.head_;
    tail_ = page.tail_;
    for (size_t i = 0; i < N; ++i) {
        buffer_[i] = std::move(page.buffer_[i]);
    }
    page.Clear();
    return *this;
}

template <class T, size_t N>
T& Page<T, N>::operator[](size_t index) {
    return buffer_[(head_ + index) % N];
}

template <class T, size_t N>
const T& Page<T, N>::operator[](size_t index) const {
    return buffer_[(head_ + index) % N];
}

template <class T, size_t N>
T& Page<T, N>::Front() {
    return buffer_[head_];
}

template <class T, size_t N>
const T& Page<T, N>::Front() const {
    return buffer_[head_];
}

template <class T, size_t N>
T& Page<T, N>::Back() {
    return buffer_[(tail_ - 1 + N) % N];
}

template <class T, size_t N>
const T& Page<T, N>::Back() const {
    return buffer_[(tail_ - 1 + N) % N];
}

template <class T, size_t N>
bool Page<T, N>::Empty() const {
    return Size() == 0;
}

template <class T, size_t N>
Page<T, N>& Page<T, N>::Clear() {
    head_ = 0;
    tail_ = 0;
    number_of_items_front_ = 0;
    number_of_items_back_ = 0;
    return *this;
}

template <class T, size_t N>
Page<T, N>& Page<T, N>::Swap(Page<T, N>& other) {
    Page<T, N> tmp(std::move(*this));
    *this = std::move(other);
    other = std::move(tmp);
    return *this;
}

template <class T, size_t N>
Page<T, N>& Page<T, N>::PushBack(const T& value) {
    buffer_[tail_] = value;
    tail_ = (tail_ + 1) % N;
    ++number_of_items_back_;
    return *this;
}

template <class T, size_t N>
Page<T, N>& Page<T, N>::PushFront(const T& value) {
    head_ = (head_ - 1 + N) % N;
    buffer_[head_] = value;
    ++number_of_items_front_;
    return *this;
}

template <class T, size_t N>
Page<T, N>& Page<T, N>::PopBack() {
    tail_ = (tail_ - 1 + N) % N;
    if (number_of_items_back_ <= 0) {
        --number_of_items_front_;
    } else {
        --number_of_items_back_;
    }
    return *this;
}

template <class T, size_t N>
Page<T, N>& Page<T, N>::PopFront() {
    head_ = (head_ + 1) % N;
    if (number_of_items_front_ <= 0) {
        --number_of_items_back_;
    } else {
        --number_of_items_front_;
    }
    return *this;
}

template <class T, size_t N>
bool Page<T, N>::IsBack() const {
    return number_of_items_back_ < N;
}

template <class T, size_t N>
bool Page<T, N>::IsFront() const {
    return number_of_items_front_ < N;
}

template <class T, size_t N>
bool Page<T, N>::Full() const {
    return number_of_items_front_ + number_of_items_back_ == N;
}

#endif  // DEQUE_PAGE_PAGE_H