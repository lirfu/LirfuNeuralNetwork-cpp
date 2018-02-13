//
// Created by lirfu on 08.02.18..
//

#include "MatrixDimension.h"

std::string MatrixDimension::toString() {
    std::string str("(");
    str += std::to_string(rows_);
    str += ",";
    str += std::to_string(cols_);
    str += ")";
    return str;
}