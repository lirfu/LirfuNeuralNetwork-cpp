//
// Created by lirfu on 09.02.18..
//

#ifndef NNPP_SEPARATEDDATA_H
#define NNPP_SEPARATEDDATA_H

#include "Data.h"

template <typename T>
class ISeparatedData : public IData<T> {
private:
    std::vector<T *> *trainingInputs_;
    std::vector<T *> *trainingOutputs_;
    std::vector<T *> *testInputs_;
    std::vector<T *> *testOutputs_;

public:
    ISeparatedData(std::vector<T *> *trainingInputs, std::vector<T *> *trainingOutputs,
                  std::vector<T *> *testInputs, std::vector<T *> *testOutputs) {
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

    ~ISeparatedData() {
        for (T *m : *trainingInputs_)
            delete m;
        delete trainingInputs_;

        for (T *m : *trainingOutputs_)
            delete m;
        delete trainingOutputs_;

        for (T *m : *testInputs_)
            delete m;
        delete testInputs_;

        for (T *m : *testOutputs_)
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
//                T tI = trainingInputs[randIndexTr];
//                trainingInputs[randIndexTr] = testInputs[randIndexTs];
//                testInputs[randIndexTs] = tI;
//
//                T tO = trainingOutputs[randIndexTr];
//                trainingOutputs[randIndexTr] = testOutputs[randIndexTs];
//                testOutputs[randIndexTs] = tO;
//
//            }
//    }

    std::vector<T *> *getTrainingInputs() override {
        return trainingInputs_;
    }

    std::vector<T *> *getTrainingOutputs() override {
        return trainingOutputs_;
    }

    std::vector<T *> *getValidationInputs() override {
        return testInputs_;
    }

    std::vector<T *> *getValidationOutputs() override {
        return testOutputs_;
    }
};


#endif //NNPP_SEPARATEDDATA_H
