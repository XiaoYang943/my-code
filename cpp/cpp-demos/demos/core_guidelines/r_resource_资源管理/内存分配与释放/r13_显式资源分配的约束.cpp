//
// Created by Administrator on 2026/1/18.
//

#include <iostream>
#include <memory>
#include <stdexcept>

struct Widget {
    int id;

    Widget(int i, bool shouldThrow = false) : id(i) {
        std::cout << "Widget " << id << " constructed\n";
        if (shouldThrow) {
            std::cout << "Widget " << id << " throws!\n";
            throw std::runtime_error("constructor failed");
        }
    }

    ~Widget() {
        std::cout << "Widget " << id << " destructed\n";
    }
};

void fun(std::shared_ptr<Widget> a,
         std::shared_ptr<Widget> b)
{
    std::cout << "fun() called\n";
}

int main() {
    std::cout << "=== BAD: multiple explicit allocations in one expression ===\n";

    try {
        // ❌ R.13 违规：一个表达式里有两个 new
        fun(
            std::shared_ptr<Widget>(new Widget(1, false)),
            std::shared_ptr<Widget>(new Widget(2, true))   // 构造函数抛异常
        );
    }
    catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }

    std::cout << "\n=== GOOD: separate statements ===\n";

    try {
        // ✅ 每条语句最多一个显式资源分配
        auto sp1 = std::shared_ptr<Widget>(new Widget(3, false));
        auto sp2 = std::shared_ptr<Widget>(new Widget(4, true));

        fun(sp1, sp2);
    }
    catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }

    std::cout << "\n=== BEST: factory functions (make_shared) ===\n";

    try {
        // ✅✅ 最佳方案：无显式 new，异常安全
        fun(
            std::make_shared<Widget>(5, false),
            std::make_shared<Widget>(6, true)
        );
    }
    catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }
}
