
#ifndef INC_4LABA_SPARSEVECTOR_H
#define INC_4LABA_SPARSEVECTOR_H
#include <iostream>
#include <unordered_map>
#include <cmath>
#include <stdexcept>
#include "SparseMatrix.h"

template<typename T>
class SparseMatrix;

template<typename T>
class SparseVector {
private:
    std::unordered_map<size_t, T> data;
    size_t size_;

public:
    using iterator = typename std::unordered_map<size_t, T>::iterator;
    using const_iterator = typename std::unordered_map<size_t, T>::const_iterator;

    SparseVector(size_t size = 0) : size_(size) {}

    void resize(size_t newSize) {
        size_ = newSize;
        for (auto it = data.begin(); it != data.end();) {
            if (it->first >= newSize) {
                it = data.erase(it);
            } else {
                ++it;
            }
        }
    }

    size_t size() const { return size_; }

    T operator[](size_t idx) const {
        if (idx >= size_) throw std::out_of_range("Index out of range");
        auto it = data.find(idx);
        return (it != data.end()) ? it->second : T{};
    }

    T& at(size_t idx) {
        if (idx >= size_) throw std::out_of_range("Index out of range");
        return data[idx];
    }

    iterator begin() { return data.begin(); }
    iterator end()   { return data.end(); }
    const_iterator begin() const { return data.begin(); }
    const_iterator end() const { return data.end(); }

    SparseVector& operator+=(const T& scalar) {
        for (auto it = data.begin(); it != data.end(); ) {
            it->second += scalar;
            if (it->second == T{}) {
                it = data.erase(it);
            } else {
                ++it;
            }
        }
        return *this;
    }

    SparseVector& operator*=(const T& scalar) {
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

    SparseVector& pow_elements(const T& p) {
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

    SparseVector operator+(const SparseVector &other) const {
        if (size_ != other.size_) throw std::runtime_error("Vector sizes do not match");
        SparseVector result(size_);
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

    T dot(const SparseVector &other) const {
        if (size_ != other.size_) throw std::runtime_error("Vector sizes do not match");
        T res = T{};
        const SparseVector* a = this;
        const SparseVector* b = &other;
        if (other.data.size() < data.size()) {
            a = &other;
            b = this;
        }

        for (auto &kv : a->data) {
            auto it = b->data.find(kv.first);
            if (it != b->data.end()) {
                res += kv.second * it->second;
            }
        }
        return res;
    }

    SparseVector<T> multiply(const SparseMatrix<T>& matrix) const {
        if (size_ != matrix.rows()) {
            throw std::runtime_error("Vector size and matrix row count do not match for multiplication");
        }
        SparseVector<T> result(matrix.cols());
        for (const auto& [idx, val] : data) {
            for (const auto& [m_idx, m_val] : matrix) {
                if (m_idx.row == idx) {
                    result.at(m_idx.col) += val * m_val;
                }
            }
        }
        return result;
    }

    void input_vector() {
        std::cout << "Vvedite razregeniy vector:\n";
        std::cout << "Vvedite kolichestvo elementov vektora: ";
        size_t n;
        std::cin >> n;
        resize(n);
        std::cout << "Vvedite elementy vektora postrochno (index i znachenie, razdelenie probelom).\n";
        std::cout << "Vvedite -1 dlya okonchaniya vvoda.\n";
        while (true) {
            size_t idx;
            T val;
            std::cin >> idx;
            if (idx == static_cast<size_t>(-1)) break;
            std::cin >> val;
            if (idx >= size_) {
                std::cout << "Index " << idx << " vne diapazona. Poprobuyte snova.\n";
                continue;
            }
            if (val != T{}) {
                at(idx) = val;
            }
        }
    }

    void print_vector() const {
        std::cout << "SparseVector: ";
        for (size_t i = 0; i < size_; ++i) {
            std::cout << (*this)[i] << " ";
        }
        std::cout << "\n";
    }
};
#endif //INC_4LABA_SPARSEVECTOR_H
