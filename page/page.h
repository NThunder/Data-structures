#ifndef DEQUE_PAGE_PAGE_H
#define DEQUE_PAGE_PAGE_H

template <class T, int N>
class Page {
    T buffer_[N];
    int number_of_items_front_;
    int number_of_items_back_;

public:
    Page();
    Page(const Page<T, N>&);
    ~Page() = default;
    Page<T, N>& operator=(const Page<T, N>&);
    T operator[](const int&) const;
    T& operator[](const int&);
    T Back() const;
    T& Back();
    T Front() const;
    T& Front();
    bool Empty() const;
    bool Full() const;
    int Size() const;
    bool IsBack() const;
    bool IsFront() const;
    Page<T, N>& Clear();
    Page<T, N>& PushBack(const T&);
    Page<T, N>& PushFront(const T&);
    Page<T, N>& PopBack();
    Page<T, N>& PopFront();
};

template <class T, int N>
Page<T, N>::Page() {
    number_of_items_front_ = 0;
    number_of_items_back_ = 0;
}

template <class T, int N>
Page<T, N>::Page(const Page<T, N>& other) {
    number_of_items_front_ = other.number_of_items_front_;
    number_of_items_back_ = other.number_of_items_back_;
    for (int i = 0; i < N; ++i) {
        buffer_[i] = other.buffer_[i];
    }
}

template <class T, int N>
Page<T, N>& Page<T, N>::operator=(const Page<T, N>& other) {
    number_of_items_front_ = other.number_of_items_front_;
    number_of_items_back_ = other.number_of_items_back_;
    for (int i = 0; i < N; ++i) {
        buffer_[i] = other.buffer_[i];
    }
    return *this;
}

template <class T, int N>
T Page<T, N>::operator[](const int& i) const {
    if (number_of_items_front_ != 0) {
        return buffer_[N - number_of_items_front_ + i];
    }
    return buffer_[i];
}

template <class T, int N>
T& Page<T, N>::operator[](const int& i) {
    if (number_of_items_front_ != 0) {
        return buffer_[N - number_of_items_front_ + i];
    }
    return buffer_[i];
}

template <class T, int N>
T& Page<T, N>::Front() {
    if (number_of_items_front_ == 0) {
        return buffer_[N - 1];
    }
    return buffer_[N - number_of_items_front_];
}

template <class T, int N>
T Page<T, N>::Front() const {
    if (number_of_items_front_ == 0) {
        return buffer_[N - 1];
    }
    return buffer_[N - number_of_items_front_];
}

template <class T, int N>
T& Page<T, N>::Back() {
    if (number_of_items_back_ == 0) {
        return buffer_[0];
    }
    return buffer_[number_of_items_back_ - 1];
}

template <class T, int N>
T Page<T, N>::Back() const {
    if (number_of_items_back_ == 0) {
        return buffer_[0];
    }
    return buffer_[number_of_items_back_ - 1];
}

template <class T, int N>
bool Page<T, N>::Empty() const {
    return number_of_items_back_ == 0 && number_of_items_front_ == 0;
}

template <class T, int N>
bool Page<T, N>::Full() const {
    return number_of_items_front_ + number_of_items_back_ == N;
}

template <class T, int N>
int Page<T, N>::Size() const {
    return number_of_items_front_ + number_of_items_back_;
}

template <class T, int N>
bool Page<T, N>::IsBack() const {
    return number_of_items_front_ == 0 && number_of_items_back_ < N;
}

template <class T, int N>
bool Page<T, N>::IsFront() const {
    return number_of_items_back_ == 0 && number_of_items_front_ < N;
}

template <class T, int N>
Page<T, N>& Page<T, N>::Clear() {
    number_of_items_front_ = 0;
    number_of_items_back_ = 0;
    return *this;
}

template <class T, int N>
Page<T, N>& Page<T, N>::PushBack(const T& value) {
    buffer_[number_of_items_back_] = value;
    ++number_of_items_back_;
    return *this;
}

template <class T, int N>
Page<T, N>& Page<T, N>::PushFront(const T& value) {
    buffer_[N - 1 - number_of_items_front_] = value;
    ++number_of_items_front_;
    return *this;
}

template <class T, int N>
Page<T, N>& Page<T, N>::PopBack() {
    --number_of_items_back_;
    return *this;
}

template <class T, int N>
Page<T, N>& Page<T, N>::PopFront() {
    --number_of_items_front_;
    return *this;
}

#endif  // DEQUE_PAGE_PAGE_H
