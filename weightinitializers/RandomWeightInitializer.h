//
// Created by lirfu on 09.02.18..
//
#ifndef NNPP_RANDOMWEIGHTINITIALIZER_H
#define NNPP_RANDOMWEIGHTINITIALIZER_H

#include <random>
#include "../Matrix.h"
#include "WeightInitializer.h"

class RandomWeightInitializer : public WeightInitializer {
private:
    std::mt19937 generator_;
    std::uniform_real_distribution<> random_;

public:
    RandomWeightInitializer(double min, double max) {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_real_distribution<> random(min, max);

        generator_ = generator;
        random_ = random;
    }

    void initialize(Matrix &weights) override {
        for (uint c = 0; c < weights.cols(); c++)
            for (uint r = 0; r < weights.rows(); r++)
                weights.set(r, c, random_(generator_));
    }
};

#endif //NNPP_RANDOMWEIGHTINITIALIZER_H
