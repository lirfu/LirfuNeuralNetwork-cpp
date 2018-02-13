//
// Created by lirfu on 09.02.18..
//

#ifndef NNPP_WEIGHTINITIALIZER_H
#define NNPP_WEIGHTINITIALIZER_H


#include "../Matrix.h"

class WeightInitializer {
public:
    virtual void initialize(Matrix &weights)=0;
};


#endif //NNPP_WEIGHTINITIALIZER_H
