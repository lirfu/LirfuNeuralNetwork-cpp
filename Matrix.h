//
// Created by lirfu on 08.02.18..
//

#ifndef NN_MATRIX_H
#define NN_MATRIX_H

//
// Created by lirfu on 15.10.17..
//

#pragma once

#include <string>
#include <sys/param.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <iomanip>
#include "MatrixDimension.h"

using namespace std;

class Matrix {
private:
    uint rows_, cols_;
    vector<vector<double>> data_;

    void checkSizes(const Matrix &matrix) {
        if (rows_ == matrix.rows_ && cols_ == matrix.cols_) return;

        std::stringstream text;
        text << "Matrix sizes don't match: ("
             << rows_ << "," << cols_ << ") != (" << matrix.rows_ << "," << matrix.cols_ << ")";
        throw text.str();
    }

    bool compareData(bool (*f)(double, double), const Matrix &matrix) {
        checkSizes(matrix);

        for (int r = 0; r < rows_; r++)
            for (int c = 0; c < cols_; c++)
                if (!f(data_[r][c], matrix.data_[r][c]))
                    return false;

        return true;
    }

    void multiply(const Matrix &matrix, vector<vector<double>> &data) {
        if (cols_ != matrix.rows_) {
            std::stringstream string;
            string << "Incompatible sizes: ("
                   << rows_ << "," << cols_ << ") * (" << matrix.rows_ << "," << matrix.cols_ << ").";
            throw string.str();
        }

        for (int r = 0; r < rows_; r++) {
            vector<double> row;
            for (int c = 0; c < matrix.cols_; c++) {
                double sum = 0;
                for (int i = 0; i < cols_; i++) {
                    sum += data_[r][i] * matrix.data_[i][c];
                }
                row.push_back(sum);
            }
            data.push_back(row);
        }
    }

public:
    static ulong constructorCalls;

    /* CONSTRUCTORS */
    Matrix() {
        constructorCalls++;
        rows_ = 0;
        cols_ = 0;
    }

    Matrix(uint rows, uint cols) : rows_(rows), cols_(cols) {
        constructorCalls++;
        vector<double> row(cols, 0);
        vector<vector<double>> data(rows, row);
        data_ = data;
    }

    explicit Matrix(MatrixDimension &dimension) {
        constructorCalls++;
        Matrix(dimension.rows(), dimension.cols());
    }

    Matrix(vector<vector<double>> &data) : data_(data) {
        constructorCalls++;
        rows_ = data_.size();
        cols_ = data_[0].size();
    }

    Matrix(uint rows, uint cols, initializer_list<double> data) : rows_(rows), cols_(cols) {
        constructorCalls++;
        if (data.size() != rows * cols) {
            std::stringstream string;
            string << "Number of values doesn't match the given matrix size: "
                   << data.size() << " != " << (rows * cols);
            throw string.str();
        }

        vector<double> row;
        vector<vector<double>> data_rows(rows, row);
        uint r = 0, c = 0;
        for (double val : data) {
            if (c == cols) {
                c = 0;
                data_rows[++r].push_back(val);
            } else
                data_rows[r].push_back(val);

            c++;
        }
        data_ = data_rows;
    }

    Matrix(const Matrix &matrix) {
        constructorCalls++;
        rows_ = matrix.rows_;
        cols_ = matrix.cols_;
        data_.assign(matrix.data_.begin(), matrix.data_.end());
    }

    ~Matrix() = default;

    /* Access methods. */

    uint rows() const {
        return rows_;
    }

    uint cols() const {
        return cols_;
    }

    Matrix getRowMatrix(uint index) {
        if (index < 0 || index > rows_ - 1) {
            std::stringstream string;
            string << "Row index out of bounds: " << index;
            throw string.str();
        }

        vector<double> row(data_[index]);
        vector<vector<double>> data;
        data.push_back(row);
        return {data};
    }

    Matrix getColMatrix(uint index) {
        if (index < 0 || index > cols_ - 1) {
            std::stringstream string;
            string << "Column index out of bounds: " << index;
            throw string.str();
        }
        vector<double> row;
        vector<vector<double>> data(rows_, row);
        for (int r = 0; r < rows_; r++) {
            data[r].push_back(data_[r][index]);
        }
        return {data};
    }

    MatrixDimension getDimension() const {
        return {rows_, cols_};
    }

    double get(uint row, uint col) const {
        if (row > rows_ - 1 || col > cols_ - 1) {
            std::stringstream text;
            text << "Matrix index out of bounds: ("
                 << row << "," << col << "), size is: (" << rows_ << "," << cols_ << ")";
            throw text.str();
        }

        return data_[row][col];
    }

    void set(uint row, uint col, double value) {
        if (row > rows_ - 1 || col > cols_ - 1) {
            std::stringstream text;
            text << "Matrix index out of bounds: ("
                 << row << "," << col << "), size is: (" << rows_ << "," << cols_ << ")";
            throw text.str();
        }

        data_[row][col] = value;
    }

    Matrix resize(uint rows, uint cols) {
        vector<vector<double>> newVals;
        for (int r = 0; r < rows; r++) {
            if (r >= rows_) {
                newVals.emplace_back(cols, 0);
            } else {
                vector<double> row;
                for (int c = 0; c < cols; c++) {
                    if (c >= cols_) {
                        row.push_back(0);
                    } else {
                        row.push_back(data_[r][c]);
                    }
                }
                newVals.push_back(row);
            }
        }
        return {newVals};
    }

    Matrix copy() {
        return {*this};
    }

    /** Generates a tab separated string (TSV) representation of matrix. */
    std::string toString() {
        return toString(3);
    }

    std::string toString(int precision) {
        std::stringstream string;
        string.precision(precision);

        uint rowIndex = 0;
        for (vector<double> &row : data_) {
            for (uint i = 0; i < row.size(); i++) {
                if (i > 0) string << " ";
                string << setw(precision + 6) << row[i];
            }
            if (++rowIndex < rows_)
                string << endl;
        }
        string.flush();

        return string.str();
    }

    template<typename Lambda>
    void forEach(Lambda &&lambda) {
        for (uint r = 0; r < rows_; r++)
            for (uint c = 0; c < cols_; c++) {
                lambda(data_[r][c]);
            }
    }

    Matrix &HadamardProduct(Matrix &matrix) {
        checkSizes(matrix);
        for (uint r = 0; r < rows_; r++)
            for (uint c = 0; c < cols_; c++)
                data_[r][c] *= matrix[r][c];
        return *this;
    }

    Matrix nHadamardProduct(Matrix &matrix) {
        Matrix result(*this);
        result.HadamardProduct(matrix);
        return result;
    }

    /* OPERATORS */

//    Matrix &operator=(const Matrix &matrix) {
//        data_.clear();
//        data_ = vector<vector<double>>(matrix.data_);
//        return *this;
//    }

//    Matrix &operator=(Matrix &matrix) {
//        data_.clear();
//        data_.swap(matrix.data_);
//        return *this;
//    }

    vector<double> &operator[](uint row) {
        return data_[row];
    }

    Matrix operator~() {
        vector<vector<double>> data;
        for (int c = 0; c < cols_; c++) {
            vector<double> row;
            for (int r = 0; r < rows_; r++)
                row.push_back(data_[r][c]);
            data.push_back(row);
        }

        return {data};
    }

    bool operator==(const Matrix &matrix) {
        if (rows_ != matrix.rows_ || cols_ != matrix.cols_) return false;
        return data_ == matrix.data_;
    }

    bool operator!=(const Matrix &matrix) {
        return !(*this == matrix);
    }

    bool operator<(const Matrix &matrix) {
        return compareData([](double a, double b) -> bool { return a < b; }, matrix);
    }

    bool operator<=(const Matrix &matrix) {
        return compareData([](double a, double b) -> bool { return a <= b; }, matrix);
    }

    bool operator>(const Matrix &matrix) {
        return compareData([](double a, double b) -> bool { return a > b; }, matrix);
    }

    bool operator>=(const Matrix &matrix) {
        return compareData([](double a, double b) -> bool { return a >= b; }, matrix);
    }

    Matrix &operator+=(const Matrix &matrix) {
        checkSizes(matrix);
        for (int r = 0; r < rows_; r++)
            for (int c = 0; c < cols_; c++)
                data_[r][c] += matrix.data_[r][c];
        return *this;
    }

    Matrix operator+(const Matrix &matrix) {
        Matrix m(*this);
        m += matrix;
        return m;
    }

    Matrix &operator-=(const Matrix &matrix) {
        checkSizes(matrix);
        for (int r = 0; r < rows_; r++)
            for (int c = 0; c < cols_; c++)
                data_[r][c] -= matrix.data_[r][c];
        return *this;
    }

    Matrix operator-(const Matrix &matrix) {
        Matrix m(*this);
        m -= matrix;
        return m;
    }

    Matrix &operator*=(double value) {
        for (int r = 0; r < rows_; r++)
            for (int c = 0; c < cols_; c++)
                data_[r][c] *= value;
        return *this;
    }

    Matrix operator*(double value) {
        Matrix m(*this);
        m *= value;
        return m;
    }

    friend Matrix operator*(double value, Matrix &matrix) {
        Matrix m(matrix);
        m *= value;
        return m;
    }

    Matrix operator*(const Matrix &matrix) {
        if (cols_ != matrix.rows_) {
            stringstream str;
            str << "Product of incompatible matrices: ("
                << rows_ << "," << cols_ << ") != (" << matrix.rows_ << "," << matrix.cols_ << ")" << endl;
            throw str.str();
        }
        vector<vector<double>> data;
        multiply(matrix, data);
        return {data};
    }

    template<typename Lambda>
    Matrix &operator^=(Lambda function) {
        for (int r = 0; r < rows_; r++)
            for (int c = 0; c < cols_; c++)
                data_[r][c] = function(data_[r][c]);
        return *this;
    }

    template<typename Lambda>
    Matrix operator^(Lambda function) {
        Matrix m(*this);
        m ^= function;
        return m;
    }
};

#endif //NN_MATRIX_H
