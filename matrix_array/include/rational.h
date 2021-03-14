#ifndef RATIONAL_RATIONAL_H
#define RATIONAL_RATIONAL_H

#include <cstdint>
#include <cstring>
#include <exception>
#include <iomanip>
#include <iostream>
#include <numeric>

class RationalDivisionByZero : public std::runtime_error {
public:
    RationalDivisionByZero() : std::runtime_error("RationalDivisionByZero") {
    }
};

class Rational {
    int numerator_;
    int denominator_;

public:
    Rational();
    Rational(const int&);  //  NOLINT
    Rational(int, int);
    Rational(const Rational&);
    Rational& operator=(const Rational&);
    int GetNumerator() const;
    int GetDenominator() const;
    void SetNumerator(int);
    void SetDenominator(int);
    Rational operator+(const Rational&) const;
    Rational operator-(const Rational&) const;
    Rational operator/(const Rational&) const;
    Rational operator*(const Rational&) const;
    Rational& operator+=(const Rational&);
    Rational& operator-=(const Rational&);
    Rational& operator/=(const Rational&);
    Rational& operator*=(const Rational&);
    Rational operator+();
    Rational operator-();
    Rational& operator++();
    Rational operator++(int);
    Rational& operator--();
    Rational operator--(int);
    friend bool operator>(const Rational&, const Rational&);
    friend bool operator==(const Rational&, const Rational&);
    friend bool operator<(const Rational&, const Rational&);
    friend bool operator>=(const Rational&, const Rational&);
    friend bool operator<=(const Rational&, const Rational&);
    friend bool operator!=(const Rational&, const Rational&);

    friend std::istream& operator>>(std::istream&, Rational&);
    friend std::ostream& operator<<(std::ostream&, const Rational&);
};

#endif  // RATIONAL_RATIONAL_H