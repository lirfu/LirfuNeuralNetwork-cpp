//
// Created by lirfu on 15.02.18..
//

#ifndef NNPP_DATASET_H
#define NNPP_DATASET_H


#include "../matrix/Matrix.h"

class Data {
public:
    virtual std::vector<Matrix *> *getTrainingInputs()=0;

    virtual std::vector<Matrix *> *getTrainingOutputs()=0;

    virtual std::vector<Matrix *> *getValidationInputs()=0;

    virtual std::vector<Matrix *> *getValidationOutputs()=0;
};


#endif //NNPP_DATASET_H
