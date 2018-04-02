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

        cout << "init 4" << endl; // Initialize graph with 2 curves

        // Run backpropagation.
        uint iteration = 0;
        double error = 1000;
        while (error > 1e-4) {
            error = network.backpropagate(1e-3, batches);
            iteration++;

            // Report progress.
            if (iteration % 1000 == 0) {
                std::cout << "echo Iteration " << iteration << " has error: "
                          << setprecision(16) << error << std::endl;

                cout << "clear" << endl; // Clear the graphs.
                Matrix out;
                for (uint i = 0; i < numberOfPoints; i++) {
                    out = network.getOutput(*inputs.at(i));
                    // Add the target and predicted output values.
                    std::cout << "add " << out.get(0, 0) << " " << out.get(0, 1) << " " <<
                              outputs.at(i)->get(0, 0) << " " << outputs.at(i)->get(0, 1) << std::endl;
                }
            }
        }
        std::cout << "echo Finally, iteration " << iteration << " has error: "
                  << setprecision(16) << error << std::endl;

        // Cleanup data.
        for (Data *sd : batches) {
            delete sd;
        }
    } catch (string s) {
        std::cout << s << endl;
    }
}
