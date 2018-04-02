//
// Created by lirfu on 09.02.18..
//

#ifndef NNPP_SIMPLEDDATA_H
#define NNPP_SIMPLEDDATA_H

#include "Data.h"

/**Does not perform data splitting, uses same sets for training and validation.*/
template <typename T>
class ISimpleData : public IData<T> {
private:
    std::vector<T *> *inputsSet_;
    std::vector<T *> *outputsSet_;

public:
    ISimpleData(std::vector<T *> *inputsSet, std::vector<T *> *outputsSet) {
        if (inputsSet->size() != outputsSet->size()) {
            std::stringstream str;
            str << "Data array lengths don't match: " << inputsSet->size()
                << " != " << outputsSet->size();
            throw str.str();
        }

        inputsSet_ = inputsSet;
        outputsSet_ = outputsSet;
    }

    ~ISimpleData() {
        for (T *m:*inputsSet_)
            delete m;
        delete inputsSet_;

        for (T *m:*outputsSet_)
            delete m;
        delete outputsSet_;
    }

    std::vector<T *> *getTrainingInputs() override {
        return inputsSet_;
    }

    std::vector<T *> *getTrainingOutputs() override {
        return outputsSet_;
    }

    std::vector<T *> *getValidationInputs() override {
        return inputsSet_;
    }

    std::vector<T *> *getValidationOutputs() override {
        return outputsSet_;
    }
};

typedef ISimpleData<Matrix> SimpleData;

#endif //NNPP_SIMPLEDDATA_H
