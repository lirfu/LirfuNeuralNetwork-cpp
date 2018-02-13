//
// Created by lirfu on 09.02.18..
//

#ifndef NNPP_VANILLAGRADIENTDESCEND_H
#define NNPP_VANILLAGRADIENTDESCEND_H

#include "../Matrix.h"
#include "DescendMethod.h"

class VanillaGradientDescend : public DescendMethod {
    void performDescend(Matrix &previousWeights, Matrix &gradient) override {
        previousWeights -= gradient;
    }
};

#endif //NNPP_VANILLAGRADIENTDESCEND_H
