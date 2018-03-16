//
// Created by lirfu on 08.02.18..
//

#ifndef NN_MATRIXDIMENSION_H
#define NN_MATRIXDIMENSION_H

#include <zconf.h>
#include <string>

class MatrixDimension {
private:
    uint rows_, cols_;
public:
    MatrixDimension(uint rows, uint cols) : rows_(rows), cols_(cols) {}

    uint rows() { return rows_; }

    uint cols() { return cols_; }

    bool equals(const MatrixDimension &d) { return rows_ == d.rows_ && cols_ == d.cols_; }

    std::string toString();
};


#endif //NN_MATRIXDIMENSION_H
