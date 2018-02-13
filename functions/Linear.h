//
// Created by lirfu on 13.02.18..
//

#ifndef NNPP_LINEAR_H
#define NNPP_LINEAR_H


#include "DerivativeFunction.h"

class Linear : public DerivativeFunction {
public:
    inline double calculate(double argument) override {
        return argument;
    }

    inline double calculateDerivative(double argument) override {
        return 1;
    }
};


#endif //NNPP_LINEAR_H
