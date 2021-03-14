#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H

#include <exception>
#include <iomanip>
#include <iostream>

class VectorOutOfRange : public std::out_of_range {
public:
    VectorOutOfRange() : std::out_of_range("VectorOutOfRange") {
    }
};

template <class T>
class Vector {
    T* buffer_;
    size_t size_;
    size_t capacity_;

public:
    Vector();
    explicit Vector(const size_t&);
    Vector(const size_t&, T);
    Vector(const Vector<T>&);
    Vector(Vector<T>&&) noexcept;
    Vector<T>& operator=(Vector<T>&&) noexcept;
    Vector<T>& operator=(const Vector<T>&);
    ~Vector();
    Vector<T>& Clear();
    Vector<T>& PushBack(const T&);
    Vector<T>& PopBack();
    Vector<T>& Resize(const size_t&);
    Vector<T>& Resize(const size_t&, const T&);
    Vector<T>& Reserve(const size_t&);
    Vector<T>& ShrinkToFit();
    Vector<T>& Swap(Vector<T>&);
    T operator[](const size_t&) const;
    T& operator[](const size_t&);
    T At(const size_t&) const;
    T& At(const size_t&);
    T Front() const;
    T& Front();
    T Back() const;
    T& Back();
    bool Empty() const;
    size_t Size() const;
    size_t Capacity() const;
    T* Data() const;
    bool operator>(const Vector<T>&) const;
    bool operator<(const Vector<T>&) const;
    bool operator>=(const Vector<T>&) const;
    bool operator<=(const Vector<T>&) const;
    bool operator==(const Vector<T>&) const;
    bool operator!=(const Vector<T>&) const;
    Vector<T>& Realloc();
};

template <class T>
Vector<T>::Vector() {
    buffer_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}

template <class T>
Vector<T>::Vector(const size_t& size) {
    buffer_ = new T[size];
    size_ = size;
    capacity_ = size;
}

template <class T>
Vector<T>::Vector(const size_t& size, T value) {
    buffer_ = new T[size];
    size_ = size;
    capacity_ = size;
    for (size_t i = 0; i < size_; ++i) {
        buffer_[i] = value;
    }
}

template <class T>
Vector<T>::Vector(const Vector<T>& vector) {
    size_ = vector.size_;
    capacity_ = vector.capacity_;
    buffer_ = new T[capacity_];
    for (size_t i = 0; i < size_; ++i) {
        buffer_[i] = vector.buffer_[i];
    }
}

template <class T>
Vector<T>::Vector(Vector<T>&& vector) noexcept
    : buffer_(vector.buffer_), size_(vector.size_), capacity_(vector.capacity_) {
    vector.size_ = vector.capacity_ = 0;
    vector.buffer_ = nullptr;
}

template <class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& vector) {
    if (this == &vector) {
        return *this;
    }
    delete[] buffer_;
    size_ = vector.size_;
    capacity_ = vector.capacity_;
    buffer_ = new T[capacity_];
    for (size_t i = 0; i < size_; ++i) {
        buffer_[i] = vector.buffer_[i];
    }
    return *this;
}

template <class T>
Vector<T>& Vector<T>::operator=(Vector<T>&& vector) noexcept {
    if (this != &vector) {
        delete[] buffer_;
        size_ = vector.size_;
        capacity_ = vector.capacity_;
        buffer_ = vector.buffer_;
        vector.size_ = vector.capacity_ = 0;
        vector.buffer_ = nullptr;
    }
    return *this;
}

template <class T>
Vector<T>::~Vector() {
    if (buffer_) {
        delete[] buffer_;
    }
}

template <class T>
Vector<T>& Vector<T>::Clear() {
    size_ = 0;
    return *this;
}

template <class T>
Vector<T>& Vector<T>::Realloc() {
    T* buffer = new T[capacity_];
    for (size_t i = 0; i < size_; ++i) {
        buffer[i] = buffer_[i];
    }
    delete[] buffer_;
    buffer_ = buffer;
    return *this;
}

template <class T>
Vector<T>& Vector<T>::PushBack(const T& value) {
    if (size_ == capacity_) {
        capacity_ += (capacity_ == 0);
        capacity_ *= 2;
        this->Realloc();
    }
    buffer_[size_++] = value;
    return *this;
}

template <class T>
Vector<T>& Vector<T>::PopBack() {
    --size_;
    return *this;
}

template <class T>
Vector<T>& Vector<T>::Resize(const size_t& new_size) {
    if (new_size > capacity_) {
        capacity_ = new_size;
        this->Realloc();
    }
    size_ = new_size;
    return *this;
}

template <class T>
Vector<T>& Vector<T>::Resize(const size_t& new_size, const T& value) {
    if (new_size > capacity_) {
        capacity_ = new_size;
        this->Realloc();
    }
    if (new_size > size_) {
        for (; size_ < new_size;) {
            buffer_[size_] = value;
            ++size_;
        }
    } else {
        size_ = new_size;
    }
    return *this;
}

template <class T>
Vector<T>& Vector<T>::Reserve(const size_t& new_cap) {
    if (new_cap > capacity_) {
        capacity_ = new_cap;
        this->Realloc();
    }
    return *this;
}

template <class T>
Vector<T>& Vector<T>::ShrinkToFit() {
    if (capacity_ > size_) {
        capacity_ = size_;
        this->Realloc();
    }
    return *this;
}

template <class T>
Vector<T>& Vector<T>::Swap(Vector<T>& other) {
    Vector<T> tmp(std::move(*this));
    *this = std::move(other);
    other = std::move(tmp);
    return *this;
}

template <class T>
T& Vector<T>::operator[](const size_t& index) {
    return buffer_[index];
}

template <class T>
T Vector<T>::operator[](const size_t& index) const {
    return buffer_[index];
}

template <class T>
T& Vector<T>::At(const size_t& index) {
    if (index >= size_) {
        throw VectorOutOfRange{};
    }
    return buffer_[index];
}

template <class T>
T Vector<T>::At(const size_t& index) const {
    if (index >= size_) {
        throw VectorOutOfRange{};
    }
    return buffer_[index];
}

template <class T>
T& Vector<T>::Front() {
    return buffer_[0];
}

template <class T>
T Vector<T>::Front() const {
    return buffer_[0];
}

template <class T>
T& Vector<T>::Back() {
    return buffer_[size_ - 1];
}

template <class T>
T Vector<T>::Back() const {
    return buffer_[size_ - 1];
}

template <class T>
bool Vector<T>::Empty() const {
    return size_ == 0;
}

template <class T>
size_t Vector<T>::Size() const {
    return size_;
}

template <class T>
size_t Vector<T>::Capacity() const {
    return capacity_;
}

template <class T>
T* Vector<T>::Data() const {
    return buffer_;
}

template <class T>
T Max(T f, T s) {
    T max = f;
    if (f < s) {
        max = s;
    }
    return max;
}

template <class T>
bool Vector<T>::operator>(const Vector<T>& other) const {
    size_t size = Max(size_, other.size_);
    for (size_t i = 0; i < size; ++i) {
        if (i < size_ && i < other.size_) {
            if (buffer_[i] > other.buffer_[i]) {
                return true;
            }
            if (buffer_[i] < other.buffer_[i]) {
                return false;
            }
            continue;
        }
        return (i < size_);
    }
    return false;
}

template <class T>
bool Vector<T>::operator==(const Vector<T>& other) const {
    size_t size = Max(size_, other.size_);
    for (size_t i = 0; i < size; ++i) {
        if (i < size_ && i < other.size_) {
            if (buffer_[i] > other.buffer_[i]) {
                return false;
            }
            if (buffer_[i] < other.buffer_[i]) {
                return false;
            }
            continue;
        }
        return false;
    }
    return true;
}

template <class T>
bool Vector<T>::operator>=(const Vector<T>& other) const {
    return (*this > other || *this == other);
}

template <class T>
bool Vector<T>::operator<(const Vector<T>& other) const {
    return !(*this >= other);
}

template <class T>
bool Vector<T>::operator<=(const Vector<T>& other) const {
    return !(*this > other);
}

template <class T>
bool Vector<T>::operator!=(const Vector<T>& other) const {
    return !(*this == other);
}

#endif  // VECTOR_VECTOR_H