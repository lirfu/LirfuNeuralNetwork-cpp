//
// Created by lirfu on 08.02.18..
//

#ifndef NNPP_INNERLAYER_H
#define NNPP_INNERLAYER_H


#include "Layer.h"
#include "../weightinitializers/WeightInitializer.h"

class InnerLayer : public Layer {
protected:
    InnerLayer(Matrix &output) : Layer(output) {}

    InnerLayer(Layer &layer) : Layer(layer) {}

    InnerLayer() {}

public:
    ~InnerLayer() = default;

    virtual void initialize(WeightInitializer *initializer)=0;

    virtual void forwardPass(Layer &leftLayer)=0;

    virtual Matrix &backwardPass(Matrix &outDiff, Matrix &leftOutputs, double learningRate)=0;

    virtual void updateWeights(ulong batchSize)=0;
};


#endif //NNPP_INNERLAYER_H
