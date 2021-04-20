#ifndef UNIQUE_PTR_UNIQUE_PTR_H
#define UNIQUE_PTR_UNIQUE_PTR_H

template <class T>
class UniquePtr {
    T* pointer_ = nullptr;

public:
    UniquePtr() = default;
    ~UniquePtr();
    explicit UniquePtr(T*);
    UniquePtr(const UniquePtr<T>&) = delete;
    UniquePtr<T>& operator=(const UniquePtr<T>&) = delete;
    UniquePtr(UniquePtr<T>&&) noexcept;
    UniquePtr<T>& operator=(UniquePtr<T>&&) noexcept;
    T* Release();
    UniquePtr<T>& Reset(T* ptr = nullptr);
    UniquePtr<T>& Swap(UniquePtr<T>&);
    T* Get() const;
    const T& operator*() const;
    T* operator->() const;
    explicit operator bool() const;
};

template <class T>
UniquePtr<T>::~UniquePtr() {
    delete pointer_;
}

template <class T>
UniquePtr<T>::UniquePtr(T* ptr) : pointer_(ptr) {
}

template <class T>
UniquePtr<T>::UniquePtr(UniquePtr<T>&& ptr) noexcept {
    pointer_ = ptr.pointer_;
    ptr.pointer_ = nullptr;
}

template <class T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr<T>&& ptr) noexcept {
    pointer_ = ptr.pointer_;
    ptr.pointer_ = nullptr;
    return *this;
}

template <class T>
T* UniquePtr<T>::Release() {
    T* ptr = pointer_;
    pointer_ = nullptr;
    return ptr;
}

template <class T>
UniquePtr<T>& UniquePtr<T>::Reset(T* ptr) {
    delete pointer_;
    pointer_ = ptr;
    return *this;
}

template <class T>
UniquePtr<T>& UniquePtr<T>::Swap(UniquePtr<T>& ptr) {
    T* point = ptr.pointer_;
    ptr.pointer_ = pointer_;
    pointer_ = point;
    return *this;
}

template <class T>
T* UniquePtr<T>::Get() const {
    return pointer_;
}

template <class T>
const T& UniquePtr<T>::operator*() const {
    return *pointer_;
}

template <class T>
T* UniquePtr<T>::operator->() const {
    return pointer_;
}

template <class T>
UniquePtr<T>::operator bool() const {
    return pointer_ != nullptr;
}

template <class T, class... Args>
UniquePtr<T> MakeUnique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

#endif  // UNIQUE_PTR_UNIQUE_PTR_H
