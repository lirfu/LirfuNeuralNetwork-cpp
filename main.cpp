#include <memory>
#include "Matrix.h"
#include "SeparatedData.h"
#include "weightinitializers/WeightInitializer.h"
#include "NeuralNetwork.h"
#include "weightinitializers/RandomWeightInitializer.h"
#include "layers/FullyConnectedLayer.h"
#include "functions/Sigmoid.h"
#include "descentmethods/VanillaGradientDescend.h"
#include "functions/Linear.h"

int main() {

//    Matrix *a = new Matrix(1, 1, {2});
//
//    vector<SeparatedData> data;
//
////    SeparatedData se(new vector<Matrix*>({a}),
////                     new vector<Matrix*>({a}),
////                     new vector<Matrix*>({a}),
////                     new vector<Matrix*>({a}));
////    data.push_back(se);
//
//    data.emplace_back(new vector<Matrix*>({a}),
//                      new vector<Matrix*>({a}),
//                      new vector<Matrix*>({a}),
//                      new vector<Matrix*>({a}));
//
//    data.emplace_back(new vector<Matrix*>({a}),
//                      new vector<Matrix*>({a}),
//                      new vector<Matrix*>({a}),
//                      new vector<Matrix*>({a}));
//
//    cout << Matrix::constructorCalls << endl;
//
//
//    for (SeparatedData s: data) {
//        Matrix &a = *(*s.getTestInputs())[0];
//        a*=a;
//    }
//
//    cout<<data[0].getTestInputs()->operator[](0)->toString()<<endl;
//
//    cout << Matrix::constructorCalls << endl;


    try {
        int numberOfPoints = 50;
        vector<Matrix *> inputs;
        vector<Matrix *> outputs;

        double input;
        for (int i = 0; i < numberOfPoints; i++) {
            input = i * 2. * M_PI / numberOfPoints;
            inputs.push_back(new Matrix(1, 1, {input}));
            outputs.push_back(new Matrix(1, 1, {4 * sin(2 * input) + 5}));
        }

//        Matrix *i1 = new Matrix(1, 2, {1, 5});
//        Matrix *i2 = new Matrix(1, 2, {2, 4});
//        Matrix *i3 = new Matrix(1, 2, {3, 6});
//        Matrix *o1 = new Matrix(1, 1, {1});
//        Matrix *o2 = new Matrix(1, 1, {2});
//        Matrix *o3 = new Matrix(1, 1, {3});

        vector<SeparatedData> batches;
        batches.emplace_back(&inputs, &outputs, &inputs, &outputs);

        /* STOCHASTIC */
//        batches.emplace_back(new vector<Matrix *>({i1}),
//                             new vector<Matrix *>({o1}),
//                             new vector<Matrix *>({i1}),
//                             new vector<Matrix *>({o1}));
//        batches.emplace_back(new vector<Matrix *>({i2}),
//                             new vector<Matrix *>({o2}),
//                             new vector<Matrix *>({i2}),
//                             new vector<Matrix *>({o2}));
//        batches.emplace_back(new vector<Matrix *>({i3}),
//                             new vector<Matrix *>({o3}),
//                             new vector<Matrix *>({i3}),
//                             new vector<Matrix *>({o3}));

        /* BATCH */
//        batches.emplace_back(new vector<Matrix *>({i1,i2,i3}),
//                             new vector<Matrix *>({o1,o2,o3}),
//                             new vector<Matrix *>({i1,i2,i3}),
//                             new vector<Matrix *>({o1,o2,o3}));

        WeightInitializer *initializer = new RandomWeightInitializer(-1, 1);
        DescendMethod *descendMethod = new VanillaGradientDescend();
        NeuralNetwork network(new InputLayer(1), {
                new FullyConnectedLayer(1, 6, new Sigmoid(), descendMethod, initializer),
                new FullyConnectedLayer(6, 1, new Linear(), descendMethod, initializer)
        });
        delete initializer;

        uint iteration = 0;
        double error = 1000;
        while (error > 1e-4) {
            error = network.backpropagate(1e-3, batches);
            if (iteration++ % 1000 == 0)
                std::cout << iteration << ".  " << setprecision(16) << error << std::endl;
//            cout << "Constructor calls: " << Matrix::constructorCalls << endl;
        }
        std::cout << ++iteration << ".  " << setprecision(16) << error << std::endl;

        for (int i = 0; i < numberOfPoints; i++) {
            input = i * 2. * M_PI / numberOfPoints;
            std::cout << input << " " << network.getOutput(*new Matrix(1, 1, {input}))[0][0] << std::endl;
        }

    } catch (string s) {
        std::cout << s << endl;
    }

    return 0;
}