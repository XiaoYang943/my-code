//
// Created by Administrator on 2026/1/18.
//

#include <iostream>
#include <stdexcept>

struct ResourceGuard {
    int* data;

    explicit ResourceGuard(int n) : data(new int[n]) {
        std::cout << "ResourceGuard acquired resource\n";
    }

    ~ResourceGuard() {
        std::cout << "ResourceGuard released resource\n";
        delete[] data;
    }
};

void bad(int n) {
    /*
    *  没必要在堆上创建 ResourceGuard，即费时又易错
    *  可能会忘记释放这部分内存，而在 delete 调用之前也可能会发生异常，最终会造成内存泄露
     */
    ResourceGuard* p = new ResourceGuard(n);    // 构造

    // 模拟中途发生异常
    throw std::runtime_error("something went wrong");

    delete p; // 永远执行不到，这里才触发析构
}

void good(int n) {
    /*
     * 作用域对象(栈对象)，它在设计上就是安全的(RAII)
     */
    ResourceGuard g(n); // 构造，离开作用域后自动析构

    // 同样抛异常
    throw std::runtime_error("something went wrong");
}


/*
 * 通用规则：优先使用有作用域的对象(c++运行时会管理好有作用域的对象)
 */
int main() {
    try {
        bad(10);    // 不安全
    } catch (...) {
        std::cout << "exception caught\n";
    }

    try {
        good(10);   // 安全
    } catch (...) {
        std::cout << "exception caught\n";
    }
}
