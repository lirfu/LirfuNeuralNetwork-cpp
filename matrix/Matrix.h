//
// Created by lirfu on 08.02.18..
//

#ifndef NN_MATRIX_H
#define NN_MATRIX_H

//
// Created by lirfu on 15.10.17..
//

#pragma once

#include <algorithm>
#include <string>
#include <sys/param.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <iomanip>
#include <cstring>
#include <cassert>
#include "MatrixDimension.h"
#include "../functions/IFunction.h"

using namespace std;

class Matrix {
private:
    uint rows_, cols_;
    double *data_;

    inline void checkIndex(uint row, uint col) const {
        if (row >= rows_ || col >= cols_) {
            std::stringstream text;
            text << "Matrix index out of bounds: ("
                 << row << "," << col << "), size is: (" << rows_ << "," << cols_ << ")";
            throw text.str();
        }
    }

    void checkSizes(const Matrix &matrix) const {
        if (rows_ == matrix.rows_ && cols_ == matrix.cols_) return;

        std::stringstream text;
        text << "Matrix sizes don't match: ("
             << rows_ << "," << cols_ << ") != (" << matrix.rows_ << "," << matrix.cols_ << ")";
        throw text.str();
    }

    bool compareData(bool (*f)(double, double), const Matrix &matrix) {
        uint max = rows_ * cols_;
        for (uint i = 0; i < max; i++)
            if (!f(data_[i], matrix.data_[i]))
                return false;
        return true;
    }

public:

    static uint constructorCalls;

    /* CONSTRUCTORS */
    Matrix() {
        constructorCalls++;
        rows_ = 0;
        cols_ = 0;
        data_ = nullptr;
    }

    Matrix(uint rows, uint cols) : rows_(rows), cols_(cols) {
        constructorCalls++;
        data_ = new double[rows * cols];
    }

    Matrix(const Matrix &matrix) : rows_(matrix.rows_), cols_(matrix.cols_), data_(new double[rows_ * cols_]) {
        constructorCalls++;
        memcpy(data_, matrix.data_, rows_ * cols_ * sizeof(double));
    }

    explicit Matrix(MatrixDimension &dimension) : Matrix(dimension.rows(), dimension.cols()) {
    }

    Matrix(vector<vector<double>> &data) : Matrix(data.size(), data[0].size()) {
        for (uint r = 0; r < rows_; r++)
            for (uint c = 0; c < cols_; c++)
                set(r, c, data[r][c]);
    }

    Matrix(uint rows, uint cols, double *data) {
        rows_ = rows;
        cols_ = cols;
        data_ = data;
    }

    Matrix(uint rows, uint cols, initializer_list<double> data) : Matrix(rows, cols) {
        if (data.size() != rows * cols) {
            std::stringstream string;
            string << "Number of values doesn't match the given matrix size: "
                   << data.size() << " != " << (rows * cols);
            throw string.str();
        }

        uint r = 0;
        uint c = 0;
        for (double val : data) {
            set(r, c, val);
            c++;
            if (c == cols_) {
                r++;
                c = 0;
            }
        }
    }

    ~Matrix() {
        delete[] data_;
    }

    /* Access methods. */

    inline uint rows() const {
        return rows_;
    }

    inline uint cols() const {
        return cols_;
    }

    MatrixDimension getDimension() const {
        return {rows_, cols_};
    }

    inline double get(uint row, uint col) const {
        assert(row < rows_ && col < cols_);
        return data_[row * cols_ + col];
    }

    inline void set(uint row, uint col, double value) {
        assert(row < rows_ && col < cols_);
        data_[row * cols_ + col] = value;
    }

    /** Generates a tab separated string (TSV) representation of matrix. */
    std::string toString() {
        return toString(3);
    }

    std::string toString(int precision) {
        std::stringstream string;
        string.precision(precision);

        for (uint r = 0; r < rows_; r++) {
            // Left bracket.
            if (rows_ <= 1) string << '<';
            else if (r == 0) string << '/';
            else if (r == rows_ - 1) string << '\\';
            else string << '|';

            // Row values.
            for (uint c = 0; c < cols_; c++) {
                if (c > 0) string << " ";
                string << setw(precision + 6) << get(r, c);
            }

            // Right bracket.
            if (rows_ <= 1) string << '>';
            else if (r == 0) string << '\\';
            else if (r == rows_ - 1) string << '/';
            else string << '|';

            string << endl;
        }
        string.flush();

        return string.str();
    }

    /* OPERATORS */

    /** Copy assignment. Deletes current data and copies data from given matrix. */
    Matrix &operator=(const Matrix &matrix) {
        if (this != &matrix) {
            rows_ = matrix.rows_;
            cols_ = matrix.cols_;

            delete[] data_;
            data_ = new double[rows_ * cols_];
            memcpy(data_, matrix.data_, rows_ * cols_ * sizeof(double));
        }
        return *this;
    }

    /** Move assignment. Moves data from given matrix and removes its data. */
    Matrix &operator=(Matrix &&matrix) noexcept {
        if (this != &matrix) {
            rows_ = matrix.rows_;
            cols_ = matrix.cols_;

            delete[] data_;
            data_ = matrix.data_;
            matrix.data_ = nullptr;
        }
        return *this;
    }

    /** Creates a new matrix with transposed data of this. */
    Matrix operator~() {
        double *data = new double[rows_ * cols_];
        for (uint c = 0; c < cols_; c++) {
            uint c_index = c * rows_;
            for (uint r = 0; r < rows_; r++)
                data[c_index + r] = get(r, c);
        }
        return {cols_, rows_, data};
    }

    /**Checks if all data is equal (element-wise). */
    bool operator==(const Matrix &matrix) {
        if (rows_ != matrix.rows_ || cols_ != matrix.cols_) return false;
        return compareData([](double a, double b) -> bool { return a == b; }, matrix);
    }

    /**Checks if all data is different (element-wise). */
    bool operator!=(const Matrix &matrix) {
        return !(*this == matrix);
    }

    /** Add given matrix to this matrix (element-wise). */
    Matrix &operator+=(const Matrix &matrix) {
        assert(rows_ == matrix.rows_ && cols_ == matrix.cols_);
        uint max = rows_ * cols_;
        for (uint i = 0; i < max; i++) {
            data_[i] += matrix.data_[i];
        }
        return *this;
    }

    /** Construct new matrix from addition of this and given matrix (element-wise). */
    Matrix operator+(const Matrix &matrix) {
        Matrix m(*this);
        m += matrix;
        return m;
    }

    /** Subtract given matrix form this matrix (element-wise). */
    Matrix &operator-=(const Matrix &matrix) {
        assert(rows_ == matrix.rows_ && cols_ == matrix.cols_);
        uint max = rows_ * cols_;
        for (uint i = 0; i < max; i++)
            data_[i] -= matrix.data_[i];
        return *this;
    }

    /** Construct new matrix from subtraction of this and given matrix (element-wise). */
    Matrix operator-(const Matrix &matrix) {
        Matrix m(*this);
        m -= matrix;
        return m;
    }

    /** Multiply this matrix with given scalar (element-wise). */
    Matrix &operator*=(double value) {
        uint max = rows_ * cols_;
        for (uint i = 0; i < max; i++)
            data_[i] *= value;
        return *this;
    }

    /** Construct new matrix from multiplication of this and given scalar (element-wise). */
    Matrix operator*(double value) {
        Matrix m(*this);
        m *= value;
        return m;
    }

    /** Construct new matrix from multiplication of given scalar and this (element-wise). */
    friend Matrix operator*(double value, Matrix &matrix) {
        return matrix * value;
    }

    /** Multiply given matrix to this matrix (element-wise). */
    Matrix &operator%=(const Matrix &matrix) {
        assert(rows_ == matrix.rows_ && cols_ == matrix.cols_);
        uint max = rows_ * cols_;
        for (uint i = 0; i < max; i++)
            data_[i] *= matrix.data_[i];
        return *this;
    }

    /** Construct new matrix from multiplication of this and given matrix (element-wise). */
    Matrix operator%(const Matrix &matrix) {
        Matrix m(*this);
        m %= matrix;
        return m;
    }

    /** Construct new matrix from addition of this and given matrix (matrix multiplication). */
    Matrix operator*(const Matrix &matrix) {
        assert(cols_ == matrix.rows_);

        double *newData = new double[rows_ * matrix.cols_];
        for (uint r = 0; r < rows_; r++) {
            for (uint c = 0; c < matrix.cols_; c++) {
                double sum = 0;
                for (uint i = 0; i < cols_; i++)
                    sum += data_[r * cols_ + i] * matrix.get(i, c);
                newData[r * matrix.cols_ + c] = sum;
            }
        }
        return {rows_, matrix.cols_, newData};
    }

    template<typename Function>
    /** Apply given function to this matrix (element-wise). */
    Matrix &operator^=(Function &&f) {
        uint max = rows_ * cols_;
        for (uint i = 0; i < max; i++)
            data_[i] = f.calculate(data_[i]);
        return *this;
    }

    template<typename Function>
    /** Construct new matrix from applying given function to this matrix (element-wise). */
    Matrix operator^(Function &&f) {
        Matrix m(*this);
        m ^= f;
        return m;
    }
};

#endif //NN_MATRIX_H
