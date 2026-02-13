//
// Created by Administrator on 2026/1/18.
//
#include <iostream>
#include <memory>

struct MyInt{
    explicit MyInt(int myInt):i(myInt) {}
    ~MyInt() {
        std::cout << "Goodbye from " << i << '\n';
    }
    int i;
};

void bad() {
    std::cout << '\n';

    MyInt* myInt = new MyInt(2011);

    // 两个所有者，发生两次内存释放
    std::unique_ptr<MyInt> uniq1 = std::unique_ptr<MyInt>(myInt);
    // std::unique_ptr<MyInt> uniq2 = std::unique_ptr<MyInt>(myInt);   // 重复释放，UB

    std::cout << '\n';
}

void good() {
    std::cout << '\n';

    auto unique_ptr = std::make_unique<MyInt>(2012);

    std::cout << '\n';
}

/*
 * NNN(No Naked New，不要裸着的 New)：立即将显式资源分配的结果交给一个管理者对象(unique_ptr或shared_ptr)
 */
int main() {

    bad();

    good();
}

