//
// Created by lirfu on 09.02.18..
//

#ifndef NNPP_SIGMOID_H
#define NNPP_SIGMOID_H


#include <cmath>
#include "DerivativeFunction.h"

class Sigmoid : public DerivativeFunction {
private:
    class Function : public IFunction {
    public:
        double calculate(double value) override {
            return 1. / (1 + exp(-value));
        }
    } function;

    class Derivative : public IFunction {
    public:
        double calculate(double value) override {
            double val = 1. / (1 + exp(-value));
            return val * (1 - val);
        }
    } derivative;
public:
    IFunction &getFunction() override {
        return function;
    }

    IFunction &getDerivative() override {
        return derivative;
    }
};


#endif //NNPP_SIGMOID_H
