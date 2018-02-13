//
// Created by lirfu on 09.02.18..
//

#ifndef NNPP_DESCENDMETHOD_H
#define NNPP_DESCENDMETHOD_H

#include "../Matrix.h"

class DescendMethod {
public:
    virtual void performDescend(Matrix &previousWeights, Matrix &gradient)=0;
};

#endif //NNPP_DESCENDMETHOD_H
