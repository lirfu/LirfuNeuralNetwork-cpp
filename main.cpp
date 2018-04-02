#include "demos/Demos.h"
#include "matrix/MatrixTests.h"

//#include<iostream>
//
//using namespace std;
//int i = 0;
//
//class A {
//public:
//    int t;
//
//    virtual ~A() {
//        cout << "~A" << endl;
//    }
//
//    virtual A &operator+(double a) {
//        t = i++;
//        cout << "ERROR+" << endl;
//    }
//
//    virtual A &operator=(const A &a) =0;
//
//    virtual A &operator=(A &a)=0;
//};
//
//class B : public A {
//public:
//    ~B() {
//        cout << "~B" << endl;
//    }
//
//    B &operator+(double a) override {
//        t = i++;
//        cout << "op+" << a << endl;
//        return *this;
//    }
//
//    virtual A &operator=(const A &a) override {
//        cout << "B=const" << endl;
//    }
//
//    virtual A &operator=(A &a) override {
//        cout << "B=" << endl;
//    }
//};
//
//void dot(A &a) {
//    A lol = a + 2;
//    lol + 3;
//}

int main() {
//    B b;
//    dot(b);

    Demos::demoRegression();

//    MatrixTests::testOperators();
//    MatrixTests::testMethods();
    return 0;
}