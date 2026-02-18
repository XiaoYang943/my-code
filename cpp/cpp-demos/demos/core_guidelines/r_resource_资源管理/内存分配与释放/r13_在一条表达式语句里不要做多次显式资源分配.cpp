//
// Created by Administrator on 2026/1/18.
//
// 演示 C++ Core Guidelines R.13
// Perform at most one explicit resource allocation in a single expression statement.
//
// 核心思想：
// 在一条表达式语句中不要进行多次显式资源分配（如 new）。
// 否则在异常发生时，可能因为求值顺序未指定而导致资源泄漏。
//

#include <iostream>
#include <memory>
#include <stdexcept>

struct Widget {
    int id;

    // 构造函数可以选择是否抛异常，用于模拟“资源创建过程中失败”
    Widget(int i, bool shouldThrow = false) : id(i) {
        std::cout << "Widget " << id << " constructed\n";

        if (shouldThrow) {
            std::cout << "Widget " << id << " throws!\n";
            // 模拟构造阶段失败（例如打开文件失败、数据库连接失败等）
            throw std::runtime_error("constructor failed");
        }
    }

    // 析构函数用于观察对象是否被正确释放
    ~Widget() {
        std::cout << "Widget " << id << " destructed\n";
    }
};

void fun(std::shared_ptr<Widget> a,
         std::shared_ptr<Widget> b)
{
    // 只有当两个 shared_ptr 都成功构造后，才会进入这里
    std::cout << "fun() called\n";
}

int main() {

    std::cout << "=== BAD: multiple explicit allocations in one expression ===\n";

    try {
        // ❌ R.13 违规：一个表达式里有两个 new
        //
        // 问题不在 shared_ptr，
        // 而在于：两个显式资源分配出现在同一条表达式里。
        //
        // 编译器可能这样执行（顺序未指定）：
        //
        // 1. operator new for Widget(1)
        // 2. operator new for Widget(2)
        // 3. 调用 Widget(1) 构造函数
        // 4. 调用 Widget(2) 构造函数 -> 抛异常
        //
        // 如果异常发生在 shared_ptr 完全构造之前，
        // 第一个 new 出来的内存可能还未被 RAII 接管，
        // 就可能产生资源泄漏。
        //
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
        //
        // 现在资源创建顺序是：
        // 1. 构造 sp1（成功后立即由 shared_ptr 接管）
        // 2. 构造 sp2（这里抛异常）
        //
        // 如果 sp2 构造失败，
        // sp1 已经是一个完整的 shared_ptr 对象，
        // 会在栈展开时自动析构，释放资源。
        //
        auto sp1 = std::shared_ptr<Widget>(new Widget(3, false));
        auto sp2 = std::shared_ptr<Widget>(new Widget(4, true));

        fun(sp1, sp2);
    }
    catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }

    std::cout << "\n=== BEST: factory functions (make_shared) ===\n";

    try {
        // ✅✅ 最佳方案：使用 make_shared
        //
        // 优点：
        // 1. 没有显式 new（避免裸资源暴露）
        // 2. 对象和控制块一次性分配
        // 3. 强异常安全保证
        // 4. 更高性能（一次内存分配）
        //
        // 即使构造函数抛异常，也不会泄漏内存，
        // 因为资源创建与控制块构造是一个整体操作。
        //
        fun(
            std::make_shared<Widget>(5, false),
            std::make_shared<Widget>(6, true)
        );
    }
    catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }
}
