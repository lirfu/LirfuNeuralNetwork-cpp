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
        cout << "Matrix operators tests..." << endl;

        {
            Matrix X(2, 2, {1, 2, 3, 4});
        }

        try {
            Matrix A(2, 2, {1, 2, 3, 4}), B(2, 2, {1, 1, 1, 1}), C;

            try {
                if (!(A == A))
                    throw "==";

                if (A != A)
                    throw "!=";

                C = A;
                C.set(1, 1, 1e-3);
                if (C.get(1, 1) != 1e-3)
                    throw "get-set1";

                if (A == C)
                    throw "copyAssign";

                C.set(0, 1, -100);
                if (C.get(0, 1) != -100)
                    throw "get-set2";

                C = A;
                C += B;
                if (C != A + B || C.get(1, 0) != 4)
                    throw "+";

                C = A;
                C -= B;
                if (C != A - B || C.get(1, 0) != 2)
                    throw "-";

                C = A;
                C *= 2;
                if (C != A * 2 || C.get(1, 0) != 6)
                    throw "* (scalar)";

                C = A;
                C = ~C;
                C = ~C;
                if (C != A)
                    throw "~";
                C = ~A;
                if (C.get(0, 1) != A.get(1, 0) || C.get(1, 0) != 2)
                    throw "~()";

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
            cerr << "Constructor error: " << e << endl;
        }
    }
};
