//
// Created by Administrator on 2026/2/14.
//

#include <iostream>
#include <limits>

/*
窄化类型转换:导致信息丢失
================================================================================
工程级原则：窄化类型转换防御规范
================================================================================

【原则 1】禁止隐式窄化转换
    - 所有“缩小类型”的转换必须使用 static_cast 显式表达
    - 表达“我知道我在丢数据”

【原则 2】整数缩小必须做范围检查
    - 使用 std::numeric_limits<T>::max()/min()
    - 任何 int -> short / long long -> int 都必须检查

【原则 3】有符号 -> 无符号必须逻辑判断
    - 必须先判断 >= 0
    - 否则可能产生极大值（死循环 / 越界 / 条件判断失效）

【原则 4】优先使用 {} 初始化
    - {} 会在编译期阻止窄化
    - 让错误尽可能在编译阶段暴露

【原则 5】保持类型一致性
    - 如果变量本质需要 double，就不要降级成 float
    - 如果数据可能超过 int，就直接使用 long long

【原则 6】开启高等级编译警告
    - -Wall
    - -Wextra
    - -Wconversion

核心思想：
    类型系统是用来表达约束的。
    任何信息丢失都必须是显式且可控的。
================================================================================
*/

int main() {

    std::cout << "===== 1. double -> int 精度丢失 =====\n";
    {
        double price = 19.99;

        int wrong = price;   // ❌ 违反原则1：隐式窄化
        std::cout << "wrong: " << wrong << "\n";
        // 输出: 19

        int correct = static_cast<int>(std::round(price));  // ✅ 显式转换
        std::cout << "correct: " << correct << "\n";
        // 输出: 20
    }


    std::cout << "\n===== 2. int -> short 溢出 =====\n";
    {
        int big = 40000;

        short wrong = big;   // ❌ 违反原则2：未做范围检查
        std::cout << "wrong: " << wrong << "\n";
        // 可能输出: -25536（平台相关）

        if (big <= std::numeric_limits<short>::max()) {  // ✅ 范围检查
            short correct = static_cast<short>(big);
            std::cout << "correct: " << correct << "\n";
        } else {
            std::cout << "correct: overflow detected\n";
            // 输出: overflow detected
        }
    }


    std::cout << "\n===== 3. int -> unsigned 符号灾难 =====\n";
    {
        int value = -1;

        unsigned int wrong = value;  // ❌ 违反原则3
        std::cout << "wrong: " << wrong << "\n";
        // 输出: 4294967295 (32位平台)

        if (value >= 0) {  // ✅ 必须先判断
            unsigned int correct = static_cast<unsigned int>(value);
            std::cout << "correct: " << correct << "\n";
        } else {
            std::cout << "correct: negative value not allowed\n";
            // 输出: negative value not allowed
        }
    }


    std::cout << "\n===== 4. long long -> int 截断 =====\n";
    {
        long long big = 5000000000LL;

        int wrong = big;  // ❌ 违反原则2
        std::cout << "wrong: " << wrong << "\n";
        // 可能输出: 705032704（平台相关）

        if (big <= std::numeric_limits<int>::max()) {
            int correct = static_cast<int>(big);
            std::cout << "correct: " << correct << "\n";
        } else {
            std::cout << "correct: int overflow\n";
            // 输出: int overflow
        }
    }


    std::cout << "\n===== 5. double -> float 精度损失 =====\n";
    {
        double precise = 123456789.123456789;

        float wrong = precise;  // ❌ 违反原则5：降低精度
        std::cout << "wrong: " << wrong << "\n";
        // 输出类似: 1.23457e+08

        double correct = precise;  // ✅ 保持高精度
        std::cout << "correct: " << correct << "\n";
        // 输出: 123456789.123457
    }


    std::cout << "\n===== 6. C++11 {} 编译期防御 =====\n";
    {
        // int x{10.9};  // ❌ 编译期报错（原则4生效）

        int x{10};  // ✅ 安全初始化
        std::cout << "x: " << x << "\n";
        // 输出: 10
    }

    return 0;
}