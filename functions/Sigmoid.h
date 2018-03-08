//
// Created by lirfu on 09.02.18..
//

#ifndef NNPP_SIGMOID_H
#define NNPP_SIGMOID_H


#include <cmath>
#include "DerivativeFunction.h"

class Sigmoid : public DerivativeFunction {
public:
    double calculate(double argument) override {
        return 1. / (1 + exp(-argument));
    }

    double calculateDerivative(double argument) override {
        double value = calculate(argument);
        return value * (1 - value);
    }
};


#endif //NNPP_SIGMOID_H
