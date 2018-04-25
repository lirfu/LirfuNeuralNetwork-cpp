//
// Created by lirfu on 09.02.18..
//

#ifndef NNPP_SEPARATEDDATA_H
#define NNPP_SEPARATEDDATA_H


#include "../matrix/Matrix.h"
#include "Data.h"

class SeparatedData : public Data {
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

    ~SeparatedData() {
        for (Matrix *m : *trainingInputs_)
            delete m;
        delete trainingInputs_;

        for (Matrix *m : *trainingOutputs_)
            delete m;
        delete trainingOutputs_;

        for (Matrix *m : *testInputs_)
            delete m;
        delete testInputs_;

        for (Matrix *m : *testOutputs_)
            delete m;
        delete testOutputs_;
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

    std::vector<Matrix *> *getTrainingInputs() override {
        return trainingInputs_;
    }

    std::vector<Matrix *> *getTrainingOutputs() override {
        return trainingOutputs_;
    }

    std::vector<Matrix *> *getValidationInputs() override {
        return testInputs_;
    }

    std::vector<Matrix *> *getValidationOutputs() override {
        return testOutputs_;
    }

    ulong trainSize() override {
        return trainingInputs_->size();
    }

    ulong testSize() override {
        return testInputs_->size();
    }

    /**
    *
    * @param inputs Vector of inputs
    * @param outputs Vector of outputs
    * @param ratio Test to train ratio
    * @return SeparatedData object
    */
    static SeparatedData separateData(std::vector<Matrix *> *inputs, std::vector<Matrix *> *outputs, double ratio) {
        return {inputs, outputs, inputs, outputs};
    }
};


#endif //NNPP_SEPARATEDDATA_H
