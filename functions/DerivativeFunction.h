//
// Created by lirfu on 09.02.18..
//

#ifndef NNPP_DERIVATIVEFUNCTION_H
#define NNPP_DERIVATIVEFUNCTION_H


class DerivativeFunction {
public:
    virtual double calculate(double argument)=0;

    virtual double calculateDerivative(double argument)=0;
};


#endif //NNPP_DERIVATIVEFUNCTION_H
