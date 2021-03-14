#include "rational.h"

Rational::Rational() : numerator_(0), denominator_(1) {
}

Rational::Rational(const int& integer) : numerator_(integer), denominator_(1) {
}

Rational::Rational(const Rational& rational) {
    numerator_ = rational.numerator_;
    denominator_ = rational.denominator_;
}

Rational& Rational::operator=(const Rational& rational) {
    numerator_ = rational.numerator_;
    denominator_ = rational.denominator_;
    return *this;
}

void ToMutualSimplicity(int& num, int& denom) {
    if (denom == 0) {
        throw RationalDivisionByZero{};
    }
    if (num == 0) {
        denom = 1;
    } else {
        int sign_of_num = 1;
        int sign_of_denom = 1;
        if (num < 0) {
            sign_of_num = -1;
            num = num * -1;
        }
        if (denom < 0) {
            sign_of_denom = -1;
            denom = denom * -1;
        }
        int temp_gcd = std::gcd(num, denom);
        while (temp_gcd != 1) {
            num /= temp_gcd;
            denom /= temp_gcd;
            temp_gcd = std::gcd(num, denom);
        }
        num *= sign_of_num * sign_of_denom;
    }
}

Rational::Rational(int num, int denom) {
    ToMutualSimplicity(num, denom);
    numerator_ = num;
    denominator_ = denom;
}

int Rational::GetNumerator() const {
    return numerator_;
}

int Rational::GetDenominator() const {
    return denominator_;
}

void Rational::SetNumerator(int num) {
    ToMutualSimplicity(num, denominator_);
    numerator_ = num;
}

void Rational::SetDenominator(int denom) {
    ToMutualSimplicity(numerator_, denom);
    denominator_ = denom;
}

Rational Rational::operator+(const Rational& r) const {
    int num = numerator_ * r.denominator_ + r.numerator_ * denominator_;
    int denom = denominator_ * r.denominator_;
    return Rational(num, denom);
}

Rational Rational::operator-(const Rational& r) const {
    int num = numerator_ * r.denominator_ - r.numerator_ * denominator_;
    int denom = denominator_ * r.denominator_;
    return Rational(num, denom);
}

Rational Rational::operator/(const Rational& rational) const {
    int num = numerator_ * rational.denominator_;
    int denom = denominator_ * rational.numerator_;
    return Rational(num, denom);
}

Rational Rational::operator*(const Rational& rational) const {
    int num = numerator_ * rational.numerator_;
    int denom = denominator_ * rational.denominator_;
    return Rational(num, denom);
}

Rational& Rational::operator+=(const Rational& rational) {
    *this = *this + rational;
    return *this;
}

Rational& Rational::operator-=(const Rational& rational) {
    *this = *this - rational;
    return *this;
}

Rational& Rational::operator*=(const Rational& rational) {
    *this = *this * rational;
    return *this;
}

Rational& Rational::operator/=(const Rational& rational) {
    *this = *this / rational;
    return *this;
}

Rational Rational::operator+() {
    return Rational(*this);
}

Rational Rational::operator-() {
    return Rational(*this) * -1;
}

Rational& Rational::operator++() {
    *this += 1;
    return *this;
}

Rational Rational::operator++(int) {
    Rational copy(*this);
    *this += 1;
    return copy;
}

Rational& Rational::operator--() {
    *this -= 1;
    return *this;
}

Rational Rational::operator--(int) {
    Rational copy(*this);
    *this -= 1;
    return copy;
}

bool operator>(const Rational& f, const Rational& r) {
    if (f.numerator_ * r.denominator_ > f.denominator_ * r.numerator_) {
        return true;
    }
    return false;
}

bool operator==(const Rational& f, const Rational& r) {
    if (f.numerator_ == r.numerator_ && f.denominator_ == r.denominator_) {
        return true;
    }
    return false;
}

bool operator>=(const Rational& f, const Rational& r) {
    return f > r || f == r;
}

bool operator<(const Rational& f, const Rational& r) {
    return !(f >= r);
}

bool operator<=(const Rational& f, const Rational& r) {
    return !(f > r);
}

bool operator!=(const Rational& f, const Rational& r) {
    return !(f == r);
}

std::istream& operator>>(std::istream& is, Rational& rational) {
    int num = 0;
    int denom = 1;
    is >> num;
    if (is.peek() == '/') {
        is.ignore(1);  //   ignore '/'
        is >> denom;
    }
    rational = Rational(num, denom);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Rational& rational) {
    os << rational.numerator_;
    if (rational.denominator_ != 1) {
        os << '/' << rational.denominator_;
    }
    return os;
}