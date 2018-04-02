//
// Created by lirfu on 08.02.18..
//

#ifndef NNPP_INNERLAYER_H
#define NNPP_INNERLAYER_H


#include "Layer.h"

template<typename T>
class InnerLayer : public Layer<T> {
protected:
    InnerLayer(T &output) : Layer<T>(output) {}

    InnerLayer(Layer<T> &layer) : Layer<T>(layer) {}

public:
    virtual void forwardPass(Layer<T> &leftLayer)=0;

    virtual T &backwardPass(T &outDiff, T &leftOutputs, double learningRate)=0;

    virtual void updateWeights()=0;
};


#endif //NNPP_INNERLAYER_H
