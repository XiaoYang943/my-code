//
// Created by Administrator on 2026/2/9.
//

#include <iostream>

struct Base {
    ~Base() {
        std::cout << "Base destroyed\n";
    }
};

struct Derived : Base {

    static int alive;

    int* buffer;

    Derived() {
        buffer = new int[100];
        alive++;
        std::cout << "Derived created, alive = "
                  << alive << '\n';
    }

    ~Derived() {
        delete[] buffer;
        alive--;
        std::cout << "Derived destroyed, alive = "
                  << alive << '\n';
    }
};

int Derived::alive = 0;

/*
 *
 */
int main() {

    Base* b = new Derived();

    delete b;

    std::cout << "alive after delete = "
              << Derived::alive << '\n';
}
