//
// Created by lirfu on 09.02.18..
//

#ifndef NNPP_SEPARATEDDATA_H
#define NNPP_SEPARATEDDATA_H


#include "Matrix.h"

class SeparatedData {
private:
    std::vector<Matrix *> *trainingInputs_;
    std::vector<Matrix *> *trainingOutputs_;
    std::vector<Matrix *> *testInputs_;
    std::vector<Matrix *> *testOutputs_;

public:
    SeparatedData(std::vector<Matrix *> *trainingInputs, std::vector<Matrix *> *trainingOutputs,
                  std::vector<Matrix *> *testInputs, std::vector<Matrix *> *testOutputs) {
        if (trainingInputs->size() != trainingOutputs->size()) {
            std::stringstream str;
            str << "Training data array lengths don't match: " << trainingInputs->size()
                << " != " << trainingOutputs->size();
            throw str.str();
        }
        if (testInputs->size() != testOutputs->size()) {
            std::stringstream str;
            str << "Test data array lengths don't match: " << trainingInputs->size()
                << " != " << trainingOutputs->size();
            throw str.str();
        }

        trainingInputs_ = trainingInputs;
        trainingOutputs_ = trainingOutputs;
        testInputs_ = testInputs;
        testOutputs_ = testOutputs;
    }

//    void shuffleData() {
//        Random rand = new Random();
//        for (uint i = 0; i < trainingInputs.size() && i < testInputs.size(); i++)
//            if (rand.nextBoolean()) {
//                uint randIndexTr = rand.nextInt(trainingInputs.size());
//                uint randIndexTs = rand.nextInt(testInputs.size());
//
//                Matrix tI = trainingInputs[randIndexTr];
//                trainingInputs[randIndexTr] = testInputs[randIndexTs];
//                testInputs[randIndexTs] = tI;
//
//                Matrix tO = trainingOutputs[randIndexTr];
//                trainingOutputs[randIndexTr] = testOutputs[randIndexTs];
//                testOutputs[randIndexTs] = tO;
//
//            }
//    }

    std::vector<Matrix *> *getTrainingInputs() {
        return trainingInputs_;
    }

    std::vector<Matrix *> *getTrainingOutputs() {
        return trainingOutputs_;
    }

    std::vector<Matrix *> *getTestInputs() {
        return testInputs_;
    }

    std::vector<Matrix *> *getTestOutputs() {
        return testOutputs_;
    }
};


#endif //NNPP_SEPARATEDDATA_H
