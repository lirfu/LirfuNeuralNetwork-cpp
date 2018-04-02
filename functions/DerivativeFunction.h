//
// Created by lirfu on 09.02.18..
//

#ifndef NNPP_DERIVATIVEFUNCTION_H
#define NNPP_DERIVATIVEFUNCTION_H


#include "IFunction.h"

class DerivativeFunction {
public:
    virtual IFunction &getFunction()=0;

    virtual IFunction &getDerivative()=0;
};


#endif //NNPP_DERIVATIVEFUNCTION_H
