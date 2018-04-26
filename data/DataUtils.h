//
// Created by lirfu on 26.04.18..
//

#ifndef NNPP_DATAUTILS_H
#define NNPP_DATAUTILS_H

#include <vector>
#include "Data.h"
#include "SeparatedData.h"

namespace DataUtils {
    /**
    *
    * @param inputs Vector of inputs
    * @param outputs Vector of outputs
    * @param ratio Test to train ratio
    * @return SeparatedData object
    */
    template<typename T>
    ISeparatedData<T> *separateData(std::vector<T *> *inputs, std::vector<T *> *outputs,
                                    double trainPercentage) {
        std::vector<T *> *trainInputs = new std::vector<T *>();
        std::vector<T *> *trainOutputs = new std::vector<T *>();
        std::vector<T *> *testInputs = new std::vector<T *>();
        std::vector<T *> *testOutputs = new std::vector<T *>();

        uint trainSize = static_cast<uint>(inputs->size() * trainPercentage);
        for (uint i = 0; i < inputs->size(); i++) {
            if (i <= trainSize) {
                trainInputs->push_back(inputs->at(i));
                trainOutputs->push_back(outputs->at(i));
            } else {
                testInputs->push_back(inputs->at(i));
                testOutputs->push_back(outputs->at(i));
            }
        }

        return new ISeparatedData<T>(trainInputs, trainOutputs, testInputs, testOutputs);
    }

    template<typename T>
    IData<T> *shuffleData(IData<T> *data) {
        for (uint i = 0; i < data->trainSize(); i++) {
            ulong trainIndex = rand() % data->trainSize();
            ulong testIndex = rand() % data->testSize();

            // Swap inputs.
            T *in = data->getTrainingInputs()->at(trainIndex);
            data->getTrainingInputs()->operator[](trainIndex) = data->getValidationInputs()->at(testIndex);
            data->getValidationInputs()->operator[](testIndex) = in;

            // Swap outputs.
            T *out = data->getTrainingOutputs()->at(trainIndex);
            data->getTrainingOutputs()->operator[](trainIndex) = data->getValidationOutputs()->at(testIndex);
            data->getValidationOutputs()->operator[](testIndex) = out;
        }
        return data;
    }
};


#endif //NNPP_DATAUTILS_H
