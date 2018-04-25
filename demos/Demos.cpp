//
// Created by lirfu on 19.03.18..
//

#include <memory>
#include "Demos.h"
#include "../matrix/Matrix.h"
#include "../data/Data.h"
#include "../weightinitializers/WeightInitializer.h"
#include "../weightinitializers/RandomWeightInitializer.h"
#include "../descentmethods/DescendMethod.h"
#include "../descentmethods/VanillaGradientDescend.h"
#include "../NeuralNetwork.h"
#include "../layers/FullyConnectedLayer.h"
#include "../functions/Sigmoid.h"
#include "../functions/Linear.h"
#include "../data/SimpleData.h"
#include "../NetworkBuilder.h"

void Demos::demoRegression() {
    try {
        uint numberOfPoints = 50;

        // Create the data.
        vector<Matrix *> inputs;
        vector<Matrix *> outputs;
        for (int i = 0; i < numberOfPoints; i++) {
            double input = i * 2. * M_PI / numberOfPoints;
            inputs.push_back(new Matrix(1, 2, {input, input}));
            outputs.push_back(new Matrix(1, 2, {4 * sin(input) * input + 5, 0.2 * input + 3}));
        }
        vector<Data *> batches;
        batches.push_back(new SimpleData(&inputs, &outputs));

        // Build the network.
        WeightInitializer *initializer = new RandomWeightInitializer(-2, 2);
        shared_ptr<DescendMethod> descendMethod(new VanillaGradientDescend());
        shared_ptr<DerivativeFunction> sigmoid(new Sigmoid());
        shared_ptr<DerivativeFunction> linear(new Linear());
        NeuralNetwork network(new InputLayer<Matrix>(2), {
                new FullyConnectedLayer<Matrix>(2, 7, sigmoid, descendMethod, initializer),
                new FullyConnectedLayer<Matrix>(7, 4, sigmoid, descendMethod, initializer),
                new FullyConnectedLayer<Matrix>(4, 3, sigmoid, descendMethod, initializer),
                new FullyConnectedLayer<Matrix>(3, 2, linear, descendMethod, initializer)
        });
        delete initializer;

//        You can set weight parameters to compare performance with other implementations.
//        vector<InnerLayer<Matrix> *> layers = network.getLayers();
//        double weights[] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2};
//        layers[0]->setNeuron(0, weights);
//        layers[0]->setNeuron(1, weights);
//        layers[0]->setNeuron(2, weights);
//        layers[0]->setNeuron(3, weights);
//        layers[0]->setNeuron(4, weights);
//        layers[1]->setNeuron(0, weights);
//        layers[1]->setNeuron(1, weights);

#ifdef OUTPUT_RESULTS
        cout << "init 2" << endl; // Initialize graph with 2 curves
#endif
        // Run backpropagation.
        uint iteration = 0;
        double error = 1000;
        while (error > 1e-1) {
            error = network.backpropagate(1e-3, batches);
            iteration++;

            // Report progress.
            if (iteration % 1000 == 0) {
#ifdef OUTPUT_RESULTS
                std::cout << "echo";
#endif
                std::cout << "Iteration " << iteration << " has error: "
                          << setprecision(16) << error << std::endl;
#ifdef OUTPUT_RESULTS
                cout << "clear" << endl; // Clear the graphs.
                Matrix out;
                for (uint i = 0; i < numberOfPoints; i++) {
                    out = network.getOutput(*inputs.at(i));
                    // Add the target and predicted output values.
                    std::cout << "add " << out.get(0, 0) << " " << outputs.at(i)->get(0, 0) << std::endl;
                }
#endif
            }
        }
#ifdef OUTPUT_RESULTS
        std::cout << "echo";
#endif
        std::cout << "Finally, iteration " << iteration << " has error: "
                  << setprecision(16) << error << std::endl;

        // Cleanup data.
        for (Data *sd : batches) {
            delete sd;
        }
    } catch (string s) {
        std::cout << s << endl;
    }
}

void ::Demos::demoGPBuilder() {
    try {
        uint numberOfPoints = 200;

        // Create the data.
        std::vector<Matrix *> inputs;
        std::vector<Matrix *> outputs;
        for (int i = 0; i < numberOfPoints; i++) {
            double input = i * 2. * M_PI / numberOfPoints;
            inputs.push_back(new Matrix(1, 1, {input}));
            outputs.push_back(new Matrix(1, 1, {0.2 * sin(input) + 0.2 * sin(4 * input + M_PI / 7) + 0.5}));
        }
        std::vector<Data *> batches;
        batches.push_back(new SimpleData(&inputs, &outputs));

        // Build the network.
        WeightInitializer* initializer = new RandomWeightInitializer(-1,1);
        InputLayer *inputLayer = new InputLayer(1);
        NeuralNetwork network = NetworkBuilder(inputLayer)
                .addLayer(20)
                .addLayer(16)
                .addLayer(7)
                .addLayer(1, NetworkBuilder::FULLY_CONNECTED, NetworkBuilder::LINEAR)
                .build(initializer);
        delete initializer;

#ifdef OUTPUT_RESULTS
        cout << "init 2" << endl; // Initialize graph with 2 curves
#endif
        // Run backpropagation.
        uint iteration = 0;
        double error = 1000;
        long iterator = 100000L;
        while (iterator-- > 0 /*error > 1e-3*/) {
            error = network.backpropagate(1, batches);
            iteration++;

            // Report progress.
            if (iteration % 1000 == 0) {
#ifdef OUTPUT_RESULTS
                std::cout << "echo ";
#endif
                std::cout << "Iteration " << iteration << " has error: "
                          << setprecision(16) << error << std::endl;
#ifdef OUTPUT_RESULTS
                cout << "clear" << endl; // Clear the graphs.
                Matrix out;
                for (uint i = 0; i < numberOfPoints; i++) {
                    out = network.getOutput(*inputs.at(i));
                    // Add the target and predicted output values.
                    std::cout << "add " << out.get(0, 0) << " " << out.get(0, 1) << " " <<
                              outputs.at(i)->get(0, 0) << " " << outputs.at(i)->get(0, 1) << std::endl;
                }
#endif
            }
        }
#ifdef OUTPUT_RESULTS
        std::cout << "echo ";
#endif
        std::cout << "Finally, iteration " << iteration << " has error: "
                  << setprecision(16) << error << std::endl;

        // Cleanup data.
        for (Data *sd : batches) {
            delete sd;
        }
    } catch (string s) {
        std::cout << s << endl;
    }
}

void ::Demos::demoXORProblem() {
    std::vector<Matrix *> inputs;
    inputs.push_back(new Matrix(1, 2, {0, 0}));
    inputs.push_back(new Matrix(1, 2, {0, 1}));
    inputs.push_back(new Matrix(1, 2, {1, 0}));
    inputs.push_back(new Matrix(1, 2, {1, 1}));
    std::vector<Matrix *> outputs;
    outputs.push_back(new Matrix(1, 1, {0}));
    outputs.push_back(new Matrix(1, 1, {1}));
    outputs.push_back(new Matrix(1, 1, {1}));
    outputs.push_back(new Matrix(1, 1, {0}));


    std::vector<Data *> batches;
    batches.push_back(new SimpleData(&inputs, &outputs));

    // TODO Model definition...
}
