#ifndef ANY_ANY_H
#define ANY_ANY_H

#include <memory>
#include <iomanip>
#include <typeinfo>
#include <string>

class BadAnyCast : public std::bad_cast {

public:
    BadAnyCast() = default;
};

struct TBase;

class Any {
    std::unique_ptr<TBase> pointer_;

public:
    Any();
    Any(const Any&);
    Any& operator=(const Any&);
    Any& operator=(Any&&) noexcept;
    template <class T>
    Any(const T&);  //  NOLINT
    template <class T>
    Any& operator=(const T&);
    Any& Reset();
    Any& Swap(Any&);
    bool HasValue() const;
    template <class T>
    friend T AnyCast(const Any&);
};

template <class T>
struct Derived {
    T value_;
    explicit Derived(const T&);
    explicit Derived(T&&) noexcept;
    std::unique_ptr<TBase> Clone();
    template <class N>
    friend N AnyCast(const Any&);
};

template <class T>
Derived<T>::Derived(const T& obj) : value_(obj) {
}

template <class T>
Derived<T>::Derived(T&& obj) noexcept : value_(std::move(obj)) {
}

template <class T>
std::unique_ptr<TBase> Derived<T>::Clone() {
    return std::make_unique<TBase>(Derived<T>(value_));
}

struct TBaseDescriptor {
    const char* UniqueAddr;
    std::unique_ptr<TBase> (*kClone)(char*);
    void (*Destroyer)(char*);
};

struct TBase {
    TBaseDescriptor Descriptor;
    char BaserStorage[128 - sizeof(TBaseDescriptor)];
    template <typename TBaser>
    explicit TBase(TBaser f) {
        static_assert(sizeof(TBaser) <= sizeof(BaserStorage), "too large Base");
        static const char kUniqueVar = '\0';
        Descriptor.UniqueAddr = &kUniqueVar;
        Descriptor.kClone = [](char* f) -> std::unique_ptr<TBase> { return (reinterpret_cast<TBaser*>(f))->Clone(); };
        Descriptor.Destroyer = [](char* f) { (reinterpret_cast<TBaser*>(f))->~TBaser(); };
        new (BaserStorage) TBaser(std::move(f));
    }
    ~TBase() {
        if (Descriptor.UniqueAddr) {
            Descriptor.Destroyer(BaserStorage);
        }
    }
    std::unique_ptr<TBase> Clone() {
        return Descriptor.kClone(BaserStorage);
    }
};

inline Any::Any() : pointer_(nullptr) {
}

inline Any::Any(const Any& obj) : pointer_((obj.pointer_->Clone())) {
}

inline Any& Any::operator=(const Any& obj) {
    Any temp(obj);
    Swap(temp);
    return *this;
}

inline Any& Any::operator=(Any&& obj) noexcept {
    Swap(obj);
    obj.pointer_ = nullptr;
    return *this;
}

template <class T>
Any::Any(const T& obj) : pointer_(new TBase(Derived<T>(obj))) {
}

template <class T>
Any& Any::operator=(const T& obj) {
    pointer_ = (std::make_unique<TBase>(Derived<T>(obj)));
    return *this;
}

inline Any& Any::Reset() {
    pointer_ = nullptr;
    return *this;
}

inline Any& Any::Swap(Any& obj) {
    pointer_.swap(obj.pointer_);
    return *this;
}

inline bool Any::HasValue() const {
    bool flag = true;
    if (!pointer_) {
        flag = false;
    }
    return flag;
}

template <class T>
T AnyCast(const Any& value) {
    if (!(value.pointer_) ||
        (TBase(Derived<T>(std::move(T())))).Descriptor.UniqueAddr != value.pointer_->Descriptor.UniqueAddr) {
        throw BadAnyCast{};
    }
    return (reinterpret_cast<Derived<T>*>(value.pointer_->BaserStorage))->value_;
}
#endif  // ANY_ANY_H