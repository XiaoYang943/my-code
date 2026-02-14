//
// Created by Administrator on 2026/2/14.
//

/*
================================================================================
C++ Core Guidelines — P.10

优先使用不可变数据（Prefer immutable data to mutable data）

--------------------------------------------------------------------------------
原则说明：

    尽量让数据在创建后不再改变。
    使用 const 表达“不可变性”。

--------------------------------------------------------------------------------
原因：

    1. 常量比变量更容易推理
    2. 不可变对象不会被意外修改
    3. 不可变数据天然线程安全（不会发生数据竞争）
    4. 编译器更容易优化常量

--------------------------------------------------------------------------------
核心思想：

    能不变，就不要变。
================================================================================
*/

#include <iostream>
#include <thread>

// ❌ 示例1：可变数据导致逻辑难以推理
void mutable_example() {
    std::cout << "\n===== 可变数据示例 =====\n";

    int value = 10;

    value += 5;
    value *= 2;
    value -= 3;

    std::cout << "final value: " << value << "\n";
    // 输出: 27

    // 问题：
    // 读代码时必须逐步跟踪所有修改
}


// ✅ 示例2：不可变数据，更容易推理
void immutable_example() {
    std::cout << "\n===== 不可变数据示例 =====\n";

    const int base = 10;
    const int added = base + 5;
    const int multiplied = added * 2;
    const int finalValue = multiplied - 3;

    std::cout << "final value: " << finalValue << "\n";
    // 输出: 27

    // 优点：
    // 每一步结果都固定
    // 不需要追踪状态变化
}


// ❌ 示例3：多线程修改共享变量（数据竞争）
void race_condition_example() {
    std::cout << "\n===== 数据竞争示例 =====\n";

    int counter = 0;

    auto task = [&]() {
        for (int i = 0; i < 100000; ++i) {
            counter++;   // 数据竞争
        }
    };

    std::thread t1(task);
    std::thread t2(task);

    t1.join();
    t2.join();

    std::cout << "counter: " << counter << "\n";
    // 结果不可预测（可能不是 200000）
}


// ✅ 示例4：不可变数据天然线程安全
void immutable_thread_safe_example() {
    std::cout << "\n===== 不可变数据线程安全示例 =====\n";

    const int sharedValue = 42;

    auto task = [&]() {
        // 只能读取，不能修改
        std::cout << sharedValue << " ";
    };

    std::thread t1(task);
    std::thread t2(task);

    t1.join();
    t2.join();

    // 输出: 42 42
    // 不会发生数据竞争
}


int main() {

    mutable_example();

    immutable_example();

    race_condition_example();

    immutable_thread_safe_example();

    return 0;
}
