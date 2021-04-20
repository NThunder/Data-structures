#ifndef SHARED_PTR_SHARED_PTR_H
#define SHARED_PTR_SHARED_PTR_H

#include <exception>
#include <iomanip>

class BadWeakPtr : public std::runtime_error {
public:
    BadWeakPtr() : std::runtime_error("BadWeakPtr") {
    }
};

struct Counter {
    size_t owner_ = 0;
    size_t observer_ = 0;
};

template <class T>
class WeakPtr;

template <class T>
class SharedPtr {
    T* pointer_ = nullptr;
    Counter* counter_ = nullptr;
    SharedPtr<T>& CheckingQuantity();

public:
    SharedPtr() = default;
    ~SharedPtr();
    explicit SharedPtr(T*);
    SharedPtr(std::nullptr_t);  //  NOLINT
    SharedPtr(const SharedPtr<T>&);
    SharedPtr<T>& operator=(const SharedPtr<T>&);
    SharedPtr(SharedPtr<T>&&) noexcept;
    SharedPtr<T>& operator=(SharedPtr<T>&&) noexcept;
    explicit SharedPtr(const WeakPtr<T>&);
    SharedPtr<T>& Reset(T* ptr = nullptr);
    SharedPtr<T>& Swap(SharedPtr<T>&);
    T* Get() const;
    size_t UseCount() const;
    const T& operator*() const;
    T* operator->() const;
    explicit operator bool() const;
    friend class WeakPtr<T>;
};

template <class T>
SharedPtr<T>::SharedPtr(T* ptr) : pointer_(ptr) {
    if (pointer_) {
        counter_ = new Counter;
        ++counter_->owner_;
    }
}

template <class T>
SharedPtr<T>::SharedPtr(std::nullptr_t) : pointer_(nullptr) {
}

template <class T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& ptr) : pointer_(ptr.pointer_), counter_(ptr.counter_) {
    if (counter_) {
        ++(counter_->owner_);
    }
}

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& ptr) {
    if (this != &ptr) {
        CheckingQuantity();
        pointer_ = ptr.pointer_;
        counter_ = ptr.counter_;
        if (counter_) {
            ++(counter_->owner_);
        }
    }
    return *this;
}

template <class T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& ptr) noexcept : pointer_(ptr.pointer_), counter_(ptr.counter_) {
    ptr.counter_ = nullptr;
    ptr.pointer_ = nullptr;
}

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& ptr) noexcept {
    if (this != &ptr) {
        CheckingQuantity();
        pointer_ = ptr.pointer_;
        counter_ = ptr.counter_;
        ptr.counter_ = nullptr;
        ptr.pointer_ = nullptr;
    }
    return *this;
}

template <class T>
SharedPtr<T>::SharedPtr(const WeakPtr<T>& ptr) {
    if (ptr.Expired()) {
        throw BadWeakPtr{};
    }
    pointer_ = ptr.pointer_;
    counter_ = ptr.counter_;
    ++(counter_->owner_);
}

template <class T>
SharedPtr<T>& SharedPtr<T>::Reset(T* ptr) {
    CheckingQuantity();
    pointer_ = ptr;
    if (pointer_) {
        counter_ = new Counter;
        ++(counter_->owner_);
    } else {
        counter_ = nullptr;
    }
    return *this;
}

template <class T>
SharedPtr<T>& SharedPtr<T>::CheckingQuantity() {
    if (counter_) {
        if (counter_->owner_ == 1) {
            delete pointer_;
            --(counter_->owner_);
            if (counter_->observer_ == 0) {
                delete counter_;
            }
        } else {
            --(counter_->owner_);
        }
    }
    return *this;
}

template <class T>
SharedPtr<T>::~SharedPtr() {
    CheckingQuantity();
}

template <class T>
SharedPtr<T>& SharedPtr<T>::Swap(SharedPtr<T>& ptr) {
    SharedPtr pointer(std::move(ptr));
    ptr = std::move(*this);
    *this = std::move(pointer);
    return *this;
}

template <class T>
T* SharedPtr<T>::Get() const {
    return pointer_;
}

template <class T>
size_t SharedPtr<T>::UseCount() const {
    if (!counter_) {
        return 0;
    }
    return counter_->owner_;
}

template <class T>
const T& SharedPtr<T>::operator*() const {
    return *pointer_;
}

template <class T>
T* SharedPtr<T>::operator->() const {
    return pointer_;
}

template <class T>
SharedPtr<T>::operator bool() const {
    return pointer_ != nullptr;
}

template <class T>
class WeakPtr {
    T* pointer_ = nullptr;
    Counter* counter_ = nullptr;
    WeakPtr<T>& CheckingQuantity();

public:
    WeakPtr() = default;
    ~WeakPtr();
    WeakPtr(const WeakPtr<T>&);
    WeakPtr<T>& operator=(const WeakPtr<T>&);
    WeakPtr(WeakPtr<T>&&) noexcept;
    WeakPtr<T>& operator=(WeakPtr<T>&&) noexcept;
    WeakPtr(const SharedPtr<T>&);  //  NOLINT
    WeakPtr<T>& Reset();
    WeakPtr<T>& Swap(WeakPtr<T>&);
    size_t UseCount() const;
    bool Expired() const;
    SharedPtr<T> Lock();
    friend class SharedPtr<T>;
};

template <class T>
WeakPtr<T>::WeakPtr(const WeakPtr<T>& ptr) : pointer_(ptr.pointer_), counter_(ptr.counter_) {
    if (counter_) {
        ++counter_->observer_;
    }
}

template <class T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<T>& ptr) {
    if (this != &ptr) {
        CheckingQuantity();
        pointer_ = ptr.pointer_;
        counter_ = ptr.counter_;
        if (counter_) {
            ++counter_->observer_;
        }
    }
    return *this;
}

template <class T>
WeakPtr<T>::WeakPtr(WeakPtr<T>&& ptr) noexcept : pointer_(ptr.pointer_), counter_(ptr.counter_) {
    ptr.counter_ = nullptr;
    ptr.pointer_ = nullptr;
}

template <class T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T>&& ptr) noexcept {
    if (this != &ptr) {
        CheckingQuantity();
        pointer_ = ptr.pointer_;
        counter_ = ptr.counter_;
        ptr.counter_ = nullptr;
        ptr.pointer_ = nullptr;
    }
    return *this;
}

template <class T>
WeakPtr<T>::WeakPtr(const SharedPtr<T>& ptr) : pointer_(ptr.pointer_), counter_(ptr.counter_) {
    if (counter_) {
        ++(counter_->observer_);
    }
}

template <class T>
WeakPtr<T>& WeakPtr<T>::CheckingQuantity() {
    if (counter_) {
        if (counter_->observer_ == 1 && counter_->owner_ == 0) {
            delete counter_;
        } else {
            --(counter_->observer_);
        }
    }
    return *this;
}

template <class T>
WeakPtr<T>::~WeakPtr() {
    CheckingQuantity();
}

template <class T>
WeakPtr<T>& WeakPtr<T>::Reset() {
    CheckingQuantity();
    counter_ = nullptr;
    pointer_ = nullptr;
    return *this;
}

template <class T>
WeakPtr<T>& WeakPtr<T>::Swap(WeakPtr<T>& ptr) {
    WeakPtr pointer(std::move(ptr));
    ptr = std::move(*this);
    *this = std::move(pointer);
    return *this;
}

template <class T>
size_t WeakPtr<T>::UseCount() const {
    if (!counter_) {
        return 0;
    }
    return counter_->owner_;
}

template <class T>
bool WeakPtr<T>::Expired() const {
    return !UseCount();
}

template <class T>
SharedPtr<T> WeakPtr<T>::Lock() {
    if (Expired()) {
        return nullptr;
    }
    return SharedPtr<T>(*this);
}

template <class T, class... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

#endif
