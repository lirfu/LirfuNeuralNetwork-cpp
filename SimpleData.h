//
// Created by lirfu on 09.02.18..
//

#ifndef NNPP_SEPARATEDDATA_H
#define NNPP_SEPARATEDDATA_H


#include "Matrix.h"
#include "Data.h"

/**Does not perform data splitting, uses same sets for training and validation.*/
class SeparatedData : public Data {
private:
    std::vector<Matrix *> *inputsSet_;
    std::vector<Matrix *> *outputsSet_;

public:
    SeparatedData(std::vector<Matrix *> *inputsSet, std::vector<Matrix *> *outputsSet) {
        if (inputsSet->size() != outputsSet->size()) {
            std::stringstream str;
            str << "Data array lengths don't match: " << inputsSet->size()
                << " != " << outputsSet->size();
            throw str.str();
        }

        inputsSet_ = inputsSet;
        outputsSet_ = outputsSet;
    }

    ~SeparatedData() {
        for (Matrix *m:*inputsSet_)
            delete m;
        delete inputsSet_;

        for (Matrix *m:*outputsSet_)
            delete m;
        delete outputsSet_;
    }

    std::vector<Matrix *> *getTrainingInputs() override {
        return inputsSet_;
    }

    std::vector<Matrix *> *getTrainingOutputs() override {
        return outputsSet_;
    }

    std::vector<Matrix *> *getValidationInputs() override {
        return inputsSet_;
    }

    std::vector<Matrix *> *getValidationOutputs() override {
        return outputsSet_;
    }
};


#endif //NNPP_SEPARATEDDATA_H
