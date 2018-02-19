//
// Created by lirfu on 15.10.17..
//

#pragma once

#include <iostream>
#include "Matrix.h"

using namespace std;

class MatrixTests {
public:
    static void testOperators() {
        cout << "Matrix tests..." << endl;
        try {
            Matrix A(2, 2, {1, 2, 3, 4}), B(2, 2, {1, 1, 1, 1}), C;

            try {
                C = A;
                C = C.resize(4, 4);
                if (C[3][3] != 0)
                    throw "resize up";

                C = C.resize(2, 2);
                if (C.rows() != 2 || C.cols() != 2)
                    throw "resize down";

                C.set(1, 1, 1e-3);
                if (C.get(1, 1) != 1e-3)
                    throw "get-set";

                C[0][1] = -100;
                if (C[0][1] != -100)
                    throw "[]";

                if (!(A == A))
                    throw "==";

                if (A != A)
                    throw "!=";

                C = A;
                C += B;
                if (C != A + B)
                    throw "+";

                C = A;
                C -= B;
                if (C != A - B)
                    throw "-";

                C = A;
                C *= 2;
                if (C != A * 2)
                    throw "* (scalar)";

                C = A;
                C = ~(~C);
                C = ~C;
                C = ~C;
                if (C != A)
                    throw "~";

                C = A + B * 2;
                C -= B * 2;
                if (C != A)
                    throw "equation 1";

                cout << "PASS!" << endl;
            } catch (const char *e) {
                cerr << "Test failed: " << e << endl;
                cerr << "A:" << endl << A.toString() << endl;
                cerr << "B:" << endl << B.toString() << endl;
                cerr << "C:" << endl << C.toString() << endl;
            }
        } catch (string e) {
            cerr << "Internal error: " << e << endl;
        }
    }
};
