#ifndef MATRIX_ARRAY_MATRIX_ARRAY_H
#define MATRIX_ARRAY_MATRIX_ARRAY_H
#define MATRIX_ARRAY_SQUARE_MATRIX_IMPLEMENTED

#include <util/constants.h>
#include <iomanip>
#include <exception>

class MatrixArrayDivisionByZero : public std::runtime_error {
public:
    MatrixArrayDivisionByZero() : std::runtime_error("MatrixArrayDivisionByZero") {
    }
};

class MatrixArrayIsDegenerateError : public std::runtime_error {
public:
    MatrixArrayIsDegenerateError() : std::runtime_error("MatrixArrayIsDegenerateError") {
    }
};

class MatrixArrayOutOfRange : public std::out_of_range {
public:
    MatrixArrayOutOfRange() : std::out_of_range("MatrixArrayOutOfRange") {
    }
};

template <class T, size_t N, size_t M>
struct MatrixArray {
    T buffer_[N][M];
    size_t RowsNumber() const;
    size_t ColumnsNumber() const;
    T operator()(const size_t&, const size_t&) const;
    T& operator()(const size_t&, const size_t&);
    T At(const size_t&, const size_t&) const;
    T& At(const size_t&, const size_t&);
    MatrixArray<T, M, N> GetTransposed() const;
    bool operator==(const MatrixArray<T, N, M>&) const;
    bool operator!=(const MatrixArray<T, N, M>&) const;
    MatrixArray<T, M, N>& Transpose();
    T Trace() const;
    T Determinant() const;
    T MyDeterminant(size_t);
    MatrixArray<T, N, M>& ElemTran(
        const size_t&, const size_t&,
        T);  // add to the i-th column of the matrix A its j-th column, multiplied by the number λ
    MatrixArray<T, N, M>& Inverse();
    MatrixArray<T, N, M> GetInversed() const;
    MatrixArray<T, N, M>& ToUnit();
    MatrixArray<T, N, M>& Copy(const MatrixArray<T, N, M>&);
    MatrixArray<T, N, M> MatrixWithoutRowAndCol(const size_t&, const size_t&, const size_t&);
};

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& MatrixArray<T, N, M>::Copy(const MatrixArray<T, N, M>& m) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            buffer_[i][j] = m.buffer_[i][j];
        }
    }
    return *this;
}

template <class T, size_t N, size_t M>
size_t MatrixArray<T, N, M>::RowsNumber() const {
    return N;
}

template <class T, size_t N, size_t M>
size_t MatrixArray<T, N, M>::ColumnsNumber() const {
    return M;
}

template <class T, size_t N, size_t M>
T MatrixArray<T, N, M>::operator()(const size_t& r, const size_t& c) const {
    return buffer_[r][c];
}

template <class T, size_t N, size_t M>
T& MatrixArray<T, N, M>::operator()(const size_t& r, const size_t& c) {
    return buffer_[r][c];
}

template <class T, size_t N, size_t M>
T MatrixArray<T, N, M>::At(const size_t& row, const size_t& column) const {
    if (row >= RowsNumber()) {
        throw MatrixArrayOutOfRange{};
    }
    if (column >= ColumnsNumber()) {
        throw MatrixArrayOutOfRange{};
    }
    return buffer_[row][column];
}

template <class T, size_t N, size_t M>
T& MatrixArray<T, N, M>::At(const size_t& row, const size_t& column) {
    if (row >= RowsNumber()) {
        throw MatrixArrayOutOfRange{};
    }
    if (column >= ColumnsNumber()) {
        throw MatrixArrayOutOfRange{};
    }
    return buffer_[row][column];
}

template <class T, size_t N, size_t M>
MatrixArray<T, M, N> MatrixArray<T, N, M>::GetTransposed() const {
    MatrixArray<T, M, N> transposed;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            transposed.buffer_[j][i] = buffer_[i][j];
        }
    }
    return transposed;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M> operator+(const MatrixArray<T, N, M>& f, const MatrixArray<T, N, M>& s) {
    MatrixArray<T, N, M> sum;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            sum.buffer_[i][j] = f.buffer_[i][j] + s.buffer_[i][j];
        }
    }
    return sum;
}

template <class T, size_t N, size_t M, size_t K>
MatrixArray<T, N, K> operator*(const MatrixArray<T, N, M>& f, const MatrixArray<T, M, K>& s) {
    MatrixArray<T, N, K> mult;
    T cell = 0;
    for (size_t i = 0; i < N; ++i) {
        for (size_t l = 0; l < K; ++l) {
            cell = 0;
            for (size_t j = 0; j < M; ++j) {
                cell += f.buffer_[i][j] * s.buffer_[j][l];
            }
            mult.buffer_[i][l] = cell;
        }
    }
    return mult;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& operator+=(MatrixArray<T, N, M>& f, const MatrixArray<T, N, M>& s) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            f.buffer_[i][j] += s.buffer_[i][j];
        }
    }
    return f;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& operator-=(MatrixArray<T, N, M>& f, const MatrixArray<T, N, M>& s) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            f.buffer_[i][j] -= s.buffer_[i][j];
        }
    }
    return f;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& operator*=(MatrixArray<T, N, M>& f, const MatrixArray<T, M, M>& s) {
    MatrixArray<T, N, M> mult;
    T cell = 0;
    for (size_t i = 0; i < N; ++i) {
        for (size_t l = 0; l < M; ++l) {
            cell = 0;
            for (size_t j = 0; j < M; ++j) {
                cell += f.buffer_[i][j] * s.buffer_[j][l];
            }
            mult.buffer_[i][l] = cell;
        }
    }
    f.Copy(mult);
    return f;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M> operator*(const MatrixArray<T, N, M>& f, const int32_t& digit) {
    MatrixArray<T, N, M> mult;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            mult.buffer_[i][j] = f.buffer_[i][j] * digit;
        }
    }
    return mult;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M> operator-(const MatrixArray<T, N, M>& f, const MatrixArray<T, N, M>& s) {
    return f + (s * -1);
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M> operator*(const int32_t& digit, const MatrixArray<T, N, M>& f) {
    return f * digit;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M> operator/(const MatrixArray<T, N, M>& f, const int32_t& digit) {
    if (digit == 0) {
        throw MatrixArrayDivisionByZero{};
    }
    MatrixArray<T, N, M> div;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            div.buffer_[i][j] = f.buffer_[i][j] / digit;
        }
    }
    return div;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& operator*=(MatrixArray<T, N, M>& f, const int32_t& digit) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            f.buffer_[i][j] *= digit;
        }
    }
    return f;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& operator/=(MatrixArray<T, N, M>& f, const int32_t& digit) {
    if (digit == 0) {
        throw MatrixArrayDivisionByZero{};
    }
    MatrixArray<T, N, M> div;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            div.buffer_[i][j] = f.buffer_[i][j] / digit;
        }
    }
    f.Copy(div);
    return f;
}

template <class T, size_t N, size_t M>
bool MatrixArray<T, N, M>::operator==(const MatrixArray<T, N, M>& m) const {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            if (buffer_[i][j] != m.buffer_[i][j]) {
                return false;
            }
        }
    }
    return true;
}

template <class T, size_t N, size_t M>
bool MatrixArray<T, N, M>::operator!=(const MatrixArray<T, N, M>& m) const {
    return !(*this == m);
}

template <class T, size_t N, size_t M>
std::istream& operator>>(std::istream& is, MatrixArray<T, N, M>& m) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            is >> m.buffer_[i][j];
        }
    }
    return is;
}

template <class T, size_t N, size_t M>
std::ostream& operator<<(std::ostream& os, const MatrixArray<T, N, M>& m) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            os << m.buffer_[i][j];
            if (j != M - 1) {
                os << " ";
            }
        }
        os << "\n";
    }
    return os;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& MatrixArray<T, N, M>::ToUnit() {
    auto zero = kZero<T>;
    auto one = kOne<T>;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            if (i == j) {
                buffer_[i][j] = one;
            } else {
                buffer_[i][j] = zero;
            }
        }
    }
    return *this;
}

template <class T>
void Swap(T& f, T& s) {
    T tmp = f;
    f = s;
    s = tmp;
}

template <class T, size_t N, size_t M>
MatrixArray<T, M, N>& MatrixArray<T, N, M>::Transpose() {
    static_assert(N == M, "NotSquareMatrix");
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i; j < N; ++j) {
            Swap(buffer_[i][j], buffer_[j][i]);
        }
    }
    return *this;
}

template <class T, size_t N, size_t M>
T MatrixArray<T, N, M>::Trace() const {
    static_assert(N == M, "NotSquareMatrix");
    auto trace = kZero<T>;
    for (size_t i = 0; i < N; ++i) {
        trace += buffer_[i][i];
    }
    return trace;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& MatrixArray<T, N, M>::ElemTran(const size_t& r, const size_t& c, T v) {
    static_assert(N == M, "NotSquareMatrix");
    MatrixArray<T, N, M> unit;
    unit.ToUnit();
    unit.buffer_[r][c] = v;
    *this *= unit;
    return *this;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M> MatrixArray<T, N, M>::MatrixWithoutRowAndCol(const size_t& row, const size_t& col,
                                                                  const size_t& n) {
    size_t d = 0;
    size_t k = 0;
    MatrixArray<T, N, M> copy;
    copy.Copy(*this);
    for (size_t i = 0; i < n; ++i) {
        if (i == row) {
            d = 1;
        }
        k = 0;
        for (size_t j = 0; j < n; ++j) {
            if (j == col) {
                k = 1;
            }
            copy.buffer_[i][j] = copy.buffer_[i + d][j + k];
        }
    }
    return copy;
}

template <class T, size_t N, size_t M>
T MatrixArray<T, N, M>::Determinant() const {
    static_assert(N == M, "NotSquareMatrix");
    MatrixArray<T, N, M> copy;
    copy.Copy(*this);
    return copy.MyDeterminant(N);
}

template <class T, size_t N, size_t M>
T MatrixArray<T, N, M>::MyDeterminant(size_t n) {
    T zero = kZero<T>;
    T one = kOne<T>;
    T determinant = zero;
    T degree = one;
    if (n == 1) {
        return buffer_[0][0];
    }
    if (n == 2) {
        return buffer_[0][0] * buffer_[n - 1][n - 1] - buffer_[0][n - 1] * buffer_[n - 1][0];
    }
    for (size_t j = 0; j < n; ++j) {
        determinant += degree * buffer_[0][j] * MatrixWithoutRowAndCol(0, j, n - 1).MyDeterminant(n - 1);
        degree = -degree;
    }
    return determinant;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& MatrixArray<T, N, M>::Inverse() {
    static_assert(N == M, "NotSquareMatrix");
    T zero = kZero<T>;
    T one = kOne<T>;
    MatrixArray<T, N, M> unit;
    unit.ToUnit();
    for (size_t i = 0; i < N; i++) {
        for (size_t j = i; j < N; j++) {
            if (buffer_[i][j] != zero) {
                T div = (one / buffer_[i][j]);
                ElemTran(j, j, div);
                unit.ElemTran(j, j, div);
                for (size_t k = j + 1; k < N; k++) {
                    if (buffer_[i][k] != zero) {
                        div = -(buffer_[i][k] / buffer_[i][j]);
                        ElemTran(j, k, div);
                        unit.ElemTran(j, k, div);
                    }
                }
                for (size_t h = 0; h < N; ++h) {
                    Swap(buffer_[h][i], buffer_[h][j]);
                    Swap(unit.buffer_[h][i], unit.buffer_[h][j]);
                }
                break;
            }
        }
    }
    T determinant = one;
    for (size_t i = 0; i < N; ++i) {
        determinant *= buffer_[i][i];
    }
    if (determinant == zero) {
        throw MatrixArrayIsDegenerateError{};
    }
    for (size_t i = 0; i < N; ++i) {
        for (int32_t k = i - 1; k >= 0; --k) {
            if (buffer_[i][k] != zero) {
                T div = -(buffer_[i][k] / buffer_[i][i]);
                ElemTran(i, k, div);
                unit.ElemTran(i, k, div);
            }
        }
    }
    this->Copy(unit);
    return *this;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M> MatrixArray<T, N, M>::GetInversed() const {
    MatrixArray<T, N, M> copy;
    copy.Copy(*this);
    copy.Inverse();
    return copy;
}

template <class T, size_t N, size_t M, size_t K>
MatrixArray<T, N, K> SolveLinearSystem(const MatrixArray<T, N, M>& a, const MatrixArray<T, N, K>& b) {
    static_assert(N == M, "NotSquareMatrix");
    static_assert(K == 1, "NotColumns");
    if (a.Determinant() == kZero<T>) {
        throw MatrixArrayIsDegenerateError{};
    }
    return a.GetInversed() * b;
}

#endif