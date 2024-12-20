#include <iostream>
#include <unordered_map>
#include <cmath>
#include "SparseVector.h"
#include "SparseMatrix.h"

// ======================= Основная Функция ============================
int main() {
    // Ввод и работа с разреженным вектором
    std::cout << "=== Vvedenie razrezenogo vektora ===\n";
    SparseVector<double> sv;
    sv.input_vector();
    sv.print_vector();

    // Ввод второго вектора для скалярного произведения
    std::cout << "\n=== Vvedenie vtorogo razrezenogo vektora ===\n";
    SparseVector<double> sv2;
    sv2.input_vector();
    sv2.print_vector();

    // Проверка размера и скалярное произведение
    if (sv.size() == sv2.size()) {
        std::cout << "Skaljarnoe proizvedenie: " << sv.dot(sv2) << "\n";
    } else {
        std::cout << "Vektory raznogo razmera. Nijevozmozhno vychislit' skaljarnoe proizvedenie.\n";
    }

    // Умножение вектора на матрицу
    std::cout << "\n=== Umnozhenie vektora na matricu ===\n";
    SparseMatrix<double> sm;
    sm.input_matrix();
    sm.print_matrix();

    try {
        SparseVector<double> vm = sv.multiply(sm);
        std::cout << "Rezultat umnozheniya vektora na matricu:\n";
        vm.print_vector();
    } catch (const std::exception &e) {
        std::cout << "Oshibka pri umnozhenii vektora na matricu: " << e.what() << "\n";
    }

    // Транспонирование матрицы
    auto smT = sm.transpose();
    std::cout << "\n=== Transponirovannaya matrica ===\n";
    smT.print_matrix();

    // Ввод второй матрицы для сложения
    std::cout << "\n=== Vvedenie vtoroj razrezenoj matricy ===\n";
    SparseMatrix<double> sm2;
    sm2.input_matrix();
    sm2.print_matrix();

    // Сложение матриц
    try {
        auto sum = sm + sm2;
        std::cout << "\n=== Summa matric ===\n";
        sum.print_matrix();
    } catch (const std::exception &e) {
        std::cout << "Oshibka pri slozhenii matric: " << e.what() << "\n";
    }

    // Проверка возведения матрицы в степень
    if (sm.is_square()) {
        std::cout << "\nVvedite stepen' dlya vozvedeniya matricy v stepen': ";
        size_t power;
        std::cin >> power;
        try {
            auto smPow = sm.pow_matrix(power);
            std::cout << "Matrica v stepeni " << power << ":\n";
            smPow.print_matrix();
        } catch (const std::exception &e) {
            std::cout << "Oshibka pri vozvedenii matricy v stepen': " << e.what() << "\n";
        }

        // Обращение матрицы (только для диагональных)
        std::cout << "\n=== Obrashenie matricy (tol'ko dlia diagonal'nykh) ===\n";
        try {
            auto smInv = sm.inverse_diagonal_matrix();
            std::cout << "Obratnaya matrica:\n";
            smInv.print_matrix();
        } catch (const std::exception &e) {
            std::cout << "Oshibka pri obraschenii matricy: " << e.what() << "\n";
        }
    } else {
        std::cout << "Matrica ne kvadratnaya. Nijevozmozhno vozvedit' v stepen' i obratit'.\n";
    }

    return 0;
}