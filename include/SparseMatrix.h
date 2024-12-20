
#ifndef INC_4LABA_SPARSEMATRIX_H
#define INC_4LABA_SPARSEMATRIX_H
#include <iostream>
#include <unordered_map>
#include <cmath>
#include <stdexcept>
#include <utility>


struct MatrixIndex {
    size_t row, col;
    bool operator==(const MatrixIndex &other) const {
        return row == other.row && col == other.col;
    }
};

struct MatrixIndexHash {
    std::size_t operator()(const MatrixIndex &idx) const {
        std::size_t h1 = std::hash<size_t>()(idx.row);
        std::size_t h2 = std::hash<size_t>()(idx.col);
        return h1 ^ (h2 + 0x9e3779b97f4a7c16ULL + (h1 << 6) + (h1 >> 2));
    }
};

template<typename T>
class SparseMatrix {
private:
    std::unordered_map<MatrixIndex, T, MatrixIndexHash> data;
    size_t rows_, cols_;

public:
    using iterator = typename std::unordered_map<MatrixIndex, T, MatrixIndexHash>::iterator;
    using const_iterator = typename std::unordered_map<MatrixIndex, T, MatrixIndexHash>::const_iterator;

    SparseMatrix(size_t rows = 0, size_t cols = 0) : rows_(rows), cols_(cols) {}

    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }

    void resize(size_t newRows, size_t newCols) {
        rows_ = newRows;
        cols_ = newCols;
        for (auto it = data.begin(); it != data.end();) {
            if (it->first.row >= newRows || it->first.col >= newCols) {
                it = data.erase(it);
            } else {
                ++it;
            }
        }
    }

    T operator()(size_t r, size_t c) const {
        if (r >= rows_ || c >= cols_) throw std::out_of_range("Index out of range");
        MatrixIndex idx{r, c};
        auto it = data.find(idx);
        return (it != data.end()) ? it->second : T{};
    }

    T& at(size_t r, size_t c) {
        if (r >= rows_ || c >= cols_) throw std::out_of_range("Index out of range");
        return data[MatrixIndex{r, c}];
    }

    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }
    const_iterator begin() const { return data.begin(); }
    const_iterator end() const { return data.end(); }

    SparseMatrix transpose() const {
        SparseMatrix result(cols_, rows_);
        for (auto &kv : data) {
            MatrixIndex transIdx{kv.first.col, kv.first.row};
            result.data[transIdx] = kv.second;
        }
        return result;
    }

    SparseMatrix& operator*=(const T& scalar) {
        for (auto it = data.begin(); it != data.end();) {
            it->second *= scalar;
            if (it->second == T{}) {
                it = data.erase(it);
            } else {
                ++it;
            }
        }
        return *this;
    }

    SparseMatrix& operator+=(const T& scalar) {
        for (size_t r = 0; r < rows_; ++r) {
            for (size_t c = 0; c < cols_; ++c) {
                T val = (*this)(r, c) + scalar;
                if (val == T{}) {
                    data.erase(MatrixIndex{r, c});
                } else {
                    at(r, c) = val;
                }
            }
        }
        return *this;
    }

    SparseMatrix& pow_elements(const T& p) {
        for (auto it = data.begin(); it != data.end();) {
            T val = std::pow(it->second, p);
            if (val == T{}) {
                it = data.erase(it);
            } else {
                it->second = val;
                ++it;
            }
        }
        return *this;
    }

    SparseMatrix operator+(const SparseMatrix &other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_)
            throw std::runtime_error("Matrix sizes do not match");
        SparseMatrix result(rows_, cols_);
        for (auto &kv : data) {
            result.data[kv.first] = kv.second;
        }
        for (auto &kv : other.data) {
            result.data[kv.first] += kv.second;
            if (result.data[kv.first] == T{}) {
                result.data.erase(kv.first);
            }
        }
        return result;
    }

    SparseMatrix operator*(const SparseMatrix &other) const {
        if (cols_ != other.rows_)
            throw std::runtime_error("Matrix sizes do not match for multiplication");
        SparseMatrix result(rows_, other.cols_);

        for (auto &a_elem : data) {
            size_t i = a_elem.first.row;
            size_t k = a_elem.first.col;
            T a_val = a_elem.second;

            for (auto &b_elem : other.data) {
                if (b_elem.first.row == k) {
                    size_t j = b_elem.first.col;
                    T b_val = b_elem.second;
                    MatrixIndex idx{i, j};
                    result.data[idx] += a_val * b_val;
                    if (result.data[idx] == T{}) {
                        result.data.erase(idx);
                    }
                }
            }
        }

        return result;
    }

    bool is_square() const {
        return rows_ == cols_;
    }

    SparseMatrix pow_matrix(size_t power) const {
        if (!is_square()) {
            throw std::runtime_error("Only square matrices can be exponentiated");
        }
        SparseMatrix result = identity(rows_);
        SparseMatrix base = *this;
        while (power > 0) {
            if (power & 1) {
                result = result * base;
            }
            base = base * base;
            power >>= 1;
        }
        return result;
    }

    SparseMatrix inverse_diagonal() const {
        if (!is_square()) {
            throw std::runtime_error("Inverse can only be computed for square matrices");
        }
        SparseMatrix result(rows_, cols_);
        for (const auto& [idx, val] : data) {
            if (idx.row != idx.col) {
                throw std::runtime_error("Inverse can only be computed for diagonal matrices");
            }
            if (val == T{}) {
                throw std::runtime_error("Cannot invert zero diagonal element");
            }
            result.at(idx.row, idx.col) = static_cast<T>(1) / val;
        }
        return result;
    }

    static SparseMatrix identity(size_t size) {
        SparseMatrix id(size, size);
        for (size_t i = 0; i < size; ++i) {
            id.at(i, i) = 1;
        }
        return id;
    }

    void input_matrix() {
        std::cout << "Vvedite razregenuyu matricu:\n";
        std::cout << "Vvedite kolichestvo strok i stolbcov matricy: ";
        size_t r, c;
        std::cin >> r >> c;
        resize(r, c);
        std::cout << "Vvedite elementy matricy postrochno (stroka, stolbec, znachenie, razdelenie probelom).\n";
        std::cout << "Vvedite -1 -1 0 dlya okonchaniya vvoda.\n";
        while (true) {
            size_t row, col;
            T val;
            std::cin >> row >> col >> val;
            if (row == static_cast<size_t>(-1) && col == static_cast<size_t>(-1) && val == T{}) break;
            if (row >= rows_ || col >= cols_) {
                std::cout << "Indeksy (" << row << ", " << col << ") vne diapazona. Poprobuyte snova.\n";
                continue;
            }
            if (val != T{}) {
                at(row, col) = val;
            }
        }
    }

    void print_matrix() const {
        std::cout << "SparseMatrix (" << rows_ << "x" << cols_ << "):\n";
        for (size_t r = 0; r < rows_; ++r) {
            for (size_t c = 0; c < cols_; ++c) {
                std::cout << (*this)(r, c) << " ";
            }
            std::cout << "\n";
        }
    }

    SparseMatrix inverse_diagonal_matrix() const {
        return inverse_diagonal();
    }
};
#endif //INC_4LABA_SPARSEMATRIX_H
