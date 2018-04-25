//
// Created by lirfu on 15.02.18..
//

#ifndef NNPP_DATASET_H
#define NNPP_DATASET_H


#include "../matrix/Matrix.h"

template <typename T>
class IData {
public:
    virtual std::vector<T *> *getTrainingInputs()=0;

    virtual std::vector<T *> *getTrainingOutputs()=0;

    virtual std::vector<T *> *getValidationInputs()=0;

    virtual std::vector<Matrix *> *getValidationOutputs()=0;

    virtual ulong trainSize()=0;

    virtual ulong testSize()=0;
};

typedef IData<Matrix> Data;

#endif //NNPP_DATASET_H
