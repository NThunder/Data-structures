#ifndef ARRAY_ARRAY_H
#define ARRAY_ARRAY_H
#define ARRAY_ARRAY_TRAITS_IMPLEMENTED

#include <iterator>
#include <stdexcept>
#include <cstdint>

class ArrayOutOfRange : public std::out_of_range {
public:
    ArrayOutOfRange() : std::out_of_range("ArrayOutOfRange") {
    }
};

template <class T, int32_t N>
struct Array {
    T buffer_[N];
    const T& operator[](const int32_t&) const;
    T& operator[](const int32_t&);
    T At(const int32_t&) const;
    T& At(const int32_t&);
    T Back() const;
    T& Back();
    T Front() const;
    T& Front();
    T* Data();
    T const* Data() const;
    int32_t Size() const;
    bool Empty() const;
    void Fill(const T&);
    void Swap(Array<T, N>&);
    void swap(Array<T, N>&);  //  NOLINT
    bool operator>(const Array<T, N>&) const;
    bool operator<(const Array<T, N>&) const;
    bool operator==(const Array<T, N>&) const;
    bool operator>=(const Array<T, N>&) const;
    bool operator<=(const Array<T, N>&) const;
    bool operator!=(const Array<T, N>&) const;
};

template <class T, int32_t N>
const T& Array<T, N>::operator[](const int32_t& i) const {
    return buffer_[i];
}

template <class T, int32_t N>
T& Array<T, N>::operator[](const int32_t& i) {
    return buffer_[i];
}

template <class T, int32_t N>
T Array<T, N>::At(const int32_t& i) const {
    if (i < 0 || i >= N) {
        throw ArrayOutOfRange{};
    }
    return buffer_[i];
}

template <class T, int32_t N>
T& Array<T, N>::At(const int32_t& i) {
    if (i < 0 || i >= N) {
        throw ArrayOutOfRange{};
    }
    return buffer_[i];
}

template <class T, int32_t N>
T& Array<T, N>::Front() {
    return buffer_[0];
}

template <class T, int32_t N>
T Array<T, N>::Front() const {
    return buffer_[0];
}

template <class T, int32_t N>
T& Array<T, N>::Back() {
    return buffer_[N - 1];
}

template <class T, int32_t N>
T Array<T, N>::Back() const {
    return buffer_[N - 1];
}

template <class T, int32_t N>
T const* Array<T, N>::Data() const {
    return buffer_;
}

template <class T, int32_t N>
T* Array<T, N>::Data() {
    return buffer_;
}

template <class T, int32_t N>
int32_t Array<T, N>::Size() const {
    return N;
}

template <class T, int32_t N>
bool Array<T, N>::Empty() const {
    return Size() == 0;
}

template <class T, int32_t N>
void Array<T, N>::Fill(const T& value) {
    for (int32_t i = 0; i < Size(); ++i) {
        buffer_[i] = value;
    }
}

template <class T, int32_t N>
void Array<T, N>::Swap(Array<T, N>& other) {
    for (int32_t i = 0; i < Size(); ++i) {
        T tmp = buffer_[i];
        buffer_[i] = other.buffer_[i];
        other.buffer_[i] = tmp;
    }
}

template <class T, int32_t N>
void Array<T, N>::swap(Array<T, N>& other) {  //  NOLINT
    Swap(other);
}

template <class T, int32_t N>
bool Array<T, N>::operator>(const Array<T, N>& other) const {
    for (int32_t i = 0; i < Size(); ++i) {
        if (buffer_[i] < other.buffer_[i]) {
            return false;
        }
        if (buffer_[i] > other.buffer_[i]) {
            return true;
        }
    }
    return false;
}

template <class T, int32_t N>
bool Array<T, N>::operator==(const Array<T, N>& other) const {
    for (int32_t i = 0; i < Size(); ++i) {
        if (buffer_[i] < other.buffer_[i]) {
            return false;
        }
        if (buffer_[i] > other.buffer_[i]) {
            return false;
        }
    }
    return true;
}

template <class T, int32_t N>
bool Array<T, N>::operator>=(const Array<T, N>& other) const {
    return (*this > other) || (*this == other);
}

template <class T, int32_t N>
bool Array<T, N>::operator<(const Array<T, N>& other) const {
    return !(*this >= other);
}

template <class T, int32_t N>
bool Array<T, N>::operator<=(const Array<T, N>& other) const {
    return !(*this > other);
}

template <class T, int32_t N>
bool Array<T, N>::operator!=(const Array<T, N>& other) const {
    return !(*this == other);
}

template <class T, int32_t N>
int32_t GetSize(const T (&)[N]) {
    return N;
}

template <class T>
int32_t GetSize(const T&) {
    return 0;
}

template <class T>
int32_t GetRank(const T&) {
    return 0;
}

template <class T, int32_t N>
int32_t GetRank(const T (&other)[N]) {
    return 1 + GetRank(other[0]);
}

template <class T>
int32_t GetNumElements(const T&) {
    return 1;
}

template <class T, int32_t N>
int32_t GetNumElements(const T (&other)[N]) {
    return N * GetNumElements(other[0]);
}

#endif  // ARRAY_ARRAY_H