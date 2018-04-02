//
// Created by lirfu on 13.02.18..
//

#ifndef NNPP_LINEAR_H
#define NNPP_LINEAR_H


#include "DerivativeFunction.h"

class Linear : public DerivativeFunction {
private:
    class Function : public IFunction {
    public:
        double calculate(double value) override {
            return value;
        }
    } function;

    class Derivation : public IFunction {
    public:
        double calculate(double d) override {
            return 1;
        }
    } derivation;

public:
    IFunction &getFunction() override {
        return function;
    }

    IFunction &getDerivative() override {
        return derivation;
    }
};


#endif //NNPP_LINEAR_H
