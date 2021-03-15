#pragma once

#include <cstring>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <exception>

#define BIG_INTEGER_DIVISION_IMPLEMENTED
#define kModule 10

class BigIntegerOverflow : public std::exception {
    const char* what() const noexcept override {
        return "Result is too large";
    }
};

class BigIntegerDivisionByZero : public std::runtime_error {
public:
    BigIntegerDivisionByZero() : std::runtime_error("BigIntegerDivisionByZero") {
    }
};

class BigIntegerSqrtOfNegativeNumber : public std::runtime_error {
public:
    BigIntegerSqrtOfNegativeNumber() : std::runtime_error("BigIntegerSqrtOfNegativeNumber") {
    }
};

template <int N = 30000>
class BigInteger {
public:
    int* digits_;
    int len_ = 1;
    bool neg_;
    int capacity_ = N;
    BigInteger();
    BigInteger(int);          // NOLINT
    BigInteger(uint32_t);     // NOLINT
    BigInteger(uint64_t);     // NOLINT
    BigInteger(int64_t);      // NOLINT
    BigInteger(const char*);  // NOLINT
    BigInteger(int, bool);
    BigInteger(const BigInteger<N>&);
    ~BigInteger();

    BigInteger<N>& operator++();
    BigInteger<N> operator++(int);
    BigInteger<N>& operator--();
    BigInteger<N> operator--(int);
    BigInteger<N> operator-() const;
    BigInteger<N> operator+() const;
    BigInteger<N>& operator%=(const BigInteger<N>&);
    BigInteger<N>& operator-=(BigInteger<N>&);
    BigInteger<N> operator%(const BigInteger<N>&) const;
    BigInteger<N>& operator/=(const BigInteger<N>&);
    BigInteger<N>& operator*=(const BigInteger<N>&);
    BigInteger<N> operator/(const BigInteger<N>&) const;
    BigInteger<N> operator-(const BigInteger<N>& s) const;
    BigInteger<N> operator*(const BigInteger<N>&) const;
    BigInteger<N> operator+(const BigInteger<N>&) const;
    BigInteger<N>& operator=(const BigInteger<N>&);
    BigInteger<N>& operator+=(const BigInteger<N>&);
    BigInteger<N>& operator-=(const BigInteger<N>&);

    BigInteger<N> Sqrt() const;

    bool operator!=(const BigInteger<N>&) const;
    bool operator<=(const BigInteger<N>&) const;
    bool operator>=(const BigInteger<N>&) const;
    bool operator<(const BigInteger<N>&) const;
    bool operator>(const BigInteger<N>&) const;
    bool operator==(const BigInteger<N>&) const;
    explicit operator bool() const {
        return GetNumber(*this) == 0;
    }

    void CheckCapacity();
    bool IsNegative() {
        return neg_;
    }
};

template <int N>
void BigInteger<N>::CheckCapacity() {
    if (len_ > capacity_) {
        throw BigIntegerOverflow{};
    }
}

template <int N>
BigInteger<N> GetNumber(const BigInteger<N>& a) {
    BigInteger<N> other(a);
    other.neg_ = false;
    return other;
}

template <int N>
void Swap(BigInteger<N>& f, BigInteger<N>& s) {
    BigInteger<N> tmp = f;
    f = s;
    s = tmp;
}

template <typename T>
int Lenght(T number) {
    int len = 0;
    (number == 0) ? len = 1 : len = 0;
    while (number != 0) {
        number /= kModule;
        len++;
    }
    return len;
}

template <typename T>
void Abs(T& a) {
    (a < 0) ? a *= -1 : a *= 1;
}

template <int N>
BigInteger<N>::BigInteger() {
    capacity_ = N;
    CheckCapacity();
    len_ = 1;
    digits_ = new int[1];
    digits_[0] = 0;
    neg_ = false;
}

template <int N>
BigInteger<N>::BigInteger(int number) {
    (number >= 0) ? neg_ = false : neg_ = true, Abs(number);
    len_ = Lenght(number);
    capacity_ = N;
    CheckCapacity();
    digits_ = new int[len_];
    digits_[0] = number % kModule;
    for (int i = 1; i < len_; ++i) {
        digits_[i] = (number /= kModule) % kModule;
    }
}

template <int N>
BigInteger<N>::BigInteger(int64_t number) {
    (number >= 0) ? neg_ = false : neg_ = true, Abs(number);
    len_ = Lenght(number);
    capacity_ = N;
    CheckCapacity();
    digits_ = new int[len_];
    digits_[0] = static_cast<int>(number % kModule);
    for (int i = 1; i < len_; ++i) {
        digits_[i] = static_cast<int>(number /= kModule) % kModule;
    }
}

template <int N>
BigInteger<N>::BigInteger(uint32_t number) {
    (number >= 0) ? neg_ = false : neg_ = true, Abs(number);
    len_ = Lenght(number);
    capacity_ = N;
    CheckCapacity();
    digits_ = new int[len_];
    digits_[0] = static_cast<int>(number % kModule);
    for (int i = 1; i < len_; ++i) {
        digits_[i] = static_cast<int>(number /= kModule) % kModule;
    }
}

template <int N>
BigInteger<N>::BigInteger(uint64_t number) {
    (number >= 0) ? neg_ = false : neg_ = true, Abs(number);
    len_ = Lenght(number);
    capacity_ = N;
    CheckCapacity();
    digits_ = new int[len_];
    digits_[0] = static_cast<int>(number % kModule);
    for (int i = 1; i < len_; ++i) {
        digits_[i] = static_cast<int>(number /= kModule) % kModule;
    }
}

template <int N>
BigInteger<N>::BigInteger(const char* string) {
    capacity_ = N;
    int c = 0;
    if (string[0] == '-') {
        neg_ = true;
        len_ = static_cast<int>(strlen(string)) - 1;
        c = 1;
    } else if (string[0] == '+') {
        neg_ = false;
        len_ = static_cast<int>(strlen(string)) - 1;
        c = 1;
    } else {
        neg_ = false;
        len_ = strlen(string);
    }
    CheckCapacity();
    digits_ = new int[len_];
    for (int i = 0; i < len_; ++i) {
        digits_[i] = string[len_ - i - 1 + c] - '0';
    }
}

template <int N>
BigInteger<N>::BigInteger(int count, bool neg) {
    capacity_ = N;
    neg_ = neg;
    len_ = count;
    digits_ = new int[len_];
    for (int i = 0; i < len_; ++i) {
        digits_[i] = 0;
    }
}

template <int N>
BigInteger<N>::BigInteger(const BigInteger<N>& number) {
    neg_ = number.neg_;
    len_ = number.len_;
    capacity_ = N;
    CheckCapacity();
    digits_ = new int[len_];
    for (int i = 0; i < len_; ++i) {
        digits_[i] = number.digits_[i];
    }
}

template <int N>
BigInteger<N>::~BigInteger() {
    delete[] digits_;
}

template <int N>
BigInteger<N>& BigInteger<N>::operator=(const BigInteger<N>& number) {
    if (&number == this) {
        return *this;
    }
    delete[] digits_;
    len_ = number.len_;
    neg_ = number.neg_;
    capacity_ = N;
    digits_ = new int[len_];
    for (int i = 0; i < len_; ++i) {
        digits_[i] = number.digits_[i];
    }
    return *this;
}

template <int N>
bool BigInteger<N>::operator>(const BigInteger<N>& s) const {
    if (!neg_ && s.neg_) {
        return true;
    }
    if (neg_ && !s.neg_) {
        return false;
    }
    if (!neg_ && !s.neg_) {
        if (len_ > s.len_) {
            return true;
        }
        if (len_ < s.len_) {
            return false;
        }
        for (int i = len_ - 1; i >= 0; --i) {
            if (s.digits_[i] < digits_[i]) {
                return true;
            }
            if (s.digits_[i] > digits_[i]) {
                return false;
            }
        }
        return false;
    }
    if (len_ > s.len_) {
        return false;
    }
    if (len_ < s.len_) {
        return true;
    }
    for (int i = len_ - 1; i >= 0; --i) {
        if (s.digits_[i] < digits_[i]) {
            return false;
        }
        if (s.digits_[i] > digits_[i]) {
            return true;
        }
    }
    return false;
}

template <int N>
bool BigInteger<N>::operator==(const BigInteger<N>& s) const {
    if (!neg_ && s.neg_) {
        return false;
    }
    if (neg_ && !s.neg_) {
        return false;
    }
    if (len_ > s.len_) {
        return false;
    }
    if (len_ < s.len_) {
        return false;
    }
    for (int i = len_ - 1; i >= 0; --i) {
        if (s.digits_[i] < digits_[i]) {
            return false;
        }
        if (s.digits_[i] > digits_[i]) {
            return false;
        }
    }
    return true;
}

template <int N>
bool BigInteger<N>::operator>=(const BigInteger<N>& s) const {
    return *this > s || *this == s;
}

template <int N>
bool BigInteger<N>::operator<(const BigInteger<N>& s) const {
    return !(*this >= s);
}

template <int N>
bool BigInteger<N>::operator<=(const BigInteger<N>& s) const {
    return !(*this > s);
}

template <int N>
bool BigInteger<N>::operator!=(const BigInteger<N>& s) const {
    return !(*this == s);
}

template <int N>
BigInteger<N> BigInteger<N>::operator+(const BigInteger<N>& s) const {
    if (len_ < s.len_) {
        BigInteger<N> copy_s(s);
        return copy_s + *this;
    }
    int hyp = 0;
    int res = 0;
    if (neg_ == s.neg_) {
        BigInteger<N> r(len_ + 1, neg_);
        for (int i = 0; i < s.len_; ++i) {
            res = digits_[i] + s.digits_[i] + hyp;
            r.digits_[i] = res % kModule;
            hyp = res / kModule;
        }
        for (int i = s.len_; i < len_; ++i) {
            res = digits_[i] + hyp;
            r.digits_[i] = res % kModule;
            hyp = res / kModule;
        }
        if (hyp != 0) {
            r.digits_[r.len_ - 1] = hyp;
        } else {
            r.len_--;
        }
        r.CheckCapacity();
        if (GetNumber(r) == BigInteger<N>(0)) {
            r.neg_ = false;
        }
        return r;
    }
    BigInteger<N> copy_f = *this;
    BigInteger<N> copy_s = s;
    if (GetNumber(*this) < GetNumber(s)) {
        Swap(copy_f, copy_s);
    }
    BigInteger<N> r(copy_f.len_, copy_f.neg_);
    for (int i = 0; i < copy_s.len_; ++i) {
        res = copy_f.digits_[i] - copy_s.digits_[i] - hyp;
        res >= 0 ? hyp = 0 : hyp = 1;
        r.digits_[i] = (res + kModule) % kModule;
    }
    for (int i = copy_s.len_; i < copy_f.len_; ++i) {
        res = copy_f.digits_[i] - hyp;
        res >= 0 ? hyp = 0 : hyp = 1;
        r.digits_[i] = (res + kModule) % kModule;
    }
    while (r.len_ > 1 && r.digits_[r.len_ - 1] == 0) {
        --r.len_;
    }
    r.CheckCapacity();
    if (GetNumber(r) == BigInteger<N>(0)) {
        r.neg_ = false;
    }
    return r;
}

template <int N>
BigInteger<N>& BigInteger<N>::operator+=(const BigInteger<N>& s) {
    *this = *this + s;
    return *this;
}

template <int N>
BigInteger<N>& BigInteger<N>::operator++() {
    *this += 1;
    return *this;
}

template <int N>
BigInteger<N> BigInteger<N>::operator++(int) {
    BigInteger<N> old_value(*this);
    *this += 1;
    return old_value;
}
template <int N>
BigInteger<N> BigInteger<N>::operator-(const BigInteger<N>& s) const {
    return *this + -s;
}

template <int N>
BigInteger<N> BigInteger<N>::operator-() const {
    BigInteger<N> other(*this);
    other.neg_ ? other.neg_ = false : other.neg_ = true;
    if (GetNumber(other) == 0) {
        other.neg_ = false;
    }
    return other;
}

template <int N>
BigInteger<N> BigInteger<N>::operator+() const {
    BigInteger<N> other(*this);
    other.neg_ ? other.neg_ = true : other.neg_ = false;
    return other;
}

template <int N>
BigInteger<N>& BigInteger<N>::operator-=(const BigInteger<N>& other) {
    *this = *this + -other;
    return *this;
}

template <int N>
BigInteger<N>& BigInteger<N>::operator-=(BigInteger<N>& other) {
    *this = *this + -other;
    return *this;
}

template <int N>
BigInteger<N>& BigInteger<N>::operator--() {
    *this = *this - 1;
    return *this;
}

template <int N>
BigInteger<N> BigInteger<N>::operator--(int) {
    BigInteger<N> old_value(*this);
    *this = *this - 1;
    return old_value;
}

template <int N>
BigInteger<N> BigInteger<N>::operator*(const BigInteger<N>& s) const {
    bool flag = false;
    if (neg_ != s.neg_) {
        flag = true;
    }
    BigInteger<N> r(len_ + s.len_, flag);
    for (int i = 0; i < len_; ++i) {
        for (int j = 0; j < s.len_; ++j) {
            r.digits_[i + j] += digits_[i] * s.digits_[j];
        }
    }
    for (int i = 0; i < r.len_ - 1; ++i) {
        r.digits_[i + 1] += r.digits_[i] / kModule;
        r.digits_[i] %= kModule;
    }
    r.digits_[r.len_ - 1] %= kModule;
    while (r.len_ > 1 && r.digits_[r.len_ - 1] == 0) {
        r.len_--;
    }
    r.CheckCapacity();
    if (GetNumber(r) == BigInteger<N>(0)) {
        r.neg_ = false;
    }
    return r;
}

template <int N>
BigInteger<N>& BigInteger<N>::operator*=(const BigInteger<N>& other) {
    *this = *this * other;
    return *this;
}

template <int N>
BigInteger<N> ShiftRight(int count, const BigInteger<N>& s) {
    if (s == 0) {
        return s;
    }
    BigInteger<N> merged(count + s.len_, s.neg_);
    for (int i = 0; i < count; ++i) {
        merged.digits_[i] = 0;
    }
    for (int i = count; i < merged.len_; ++i) {
        merged.digits_[i] = s.digits_[i - count];
    }
    merged.CheckCapacity();
    return merged;
}

template <int N>
BigInteger<N> BigInteger<N>::operator/(const BigInteger<N>& second) const {
    BigInteger<N> f = GetNumber(*this);
    BigInteger<N> s = GetNumber(second);
    if (s == 0) {
        throw BigIntegerDivisionByZero{};
    }
    bool flag = false;
    if (neg_ != second.neg_) {
        flag = true;
    }
    BigInteger<N> quotient(f.len_, flag);
    BigInteger<N> remains(0);
    for (int i = f.len_ - 1; i >= 0; --i) {
        remains = ShiftRight(1, remains);
        remains.digits_[0] = f.digits_[i];
        int mult = 0;
        while (remains >= s) {
            remains -= s;
            ++mult;
        }
        quotient.digits_[i] = mult;
    }
    while (quotient.len_ > 1 && quotient.digits_[quotient.len_ - 1] == 0) {
        --quotient.len_;
    }
    if (GetNumber(quotient) == 0) {
        quotient.neg_ = false;
    }
    return quotient;
}

template <int N>
BigInteger<N>& BigInteger<N>::operator/=(const BigInteger<N>& other) {
    *this = *this / other;
    return *this;
}

template <int N>
BigInteger<N> BigInteger<N>::operator%(const BigInteger<N>& s) const {
    return (*this - (*this / s) * s);
}

template <int N>
BigInteger<N>& BigInteger<N>::operator%=(const BigInteger<N>& other) {
    *this = *this % other;
    return *this;
}

template <int N>
BigInteger<N> BigInteger<N>::Sqrt() const {
    if (neg_) {
        throw BigIntegerSqrtOfNegativeNumber{};
    }
    BigInteger<N> left(0);
    BigInteger<N> right(*this);
    BigInteger<N> middle(0);
    BigInteger<N> result(0);
    while (right >= left) {
        middle = (right + left) / 2;
        if (*this >= middle * middle) {
            result = middle;
            left = ++middle;
        } else {
            right = --middle;
        }
    }
    return result;
}

template <int N>
std::istream& operator>>(std::istream& is, BigInteger<N>& number) {
    char string[40000];
    is >> string;
    number = string;
    return is;
}

template <int N>
std::ostream& operator<<(std::ostream& os, const BigInteger<N>& number) {
    if (number.neg_) {
        os << "-";
    }
    for (int i = number.len_ - 1; i >= 0; --i) {
        os << number.digits_[i];
    }
    return os;
}