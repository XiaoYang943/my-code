//
// Created by Administrator on 2026/2/14.
//

/*
================================================================================
C++ Core Guidelines — Enum.2

使用枚举表示一组相关的具名常量

--------------------------------------------------------------------------------
原则说明：

    当一组常量在概念上属于同一类别时，
    应使用 enum 或 enum class 表达这种关系，
    而不是使用多个独立的整数常量。

--------------------------------------------------------------------------------
原因：

    1. 明确表达“这些常量彼此相关”
    2. 枚举本身就是一个具名类型
    3. 提高类型安全，避免与其他整数混用
    4. switch 语句可以获得编译器的检查支持

--------------------------------------------------------------------------------
核心思想：

    用类型表达概念集合，
    而不是用裸整数表达语义。
================================================================================
*/

#include <iostream>

// ❌ 错误示例：使用裸整数
void print_color_bad(int color) {
    if (color == 0)
        std::cout << "red\n";
    else if (color == 1)
        std::cout << "green\n";
    else if (color == 2)
        std::cout << "blue\n";
}

// ✅ 正确示例：使用强类型枚举
enum class WebColor {
    Red   = 0xFF0000,
    Green = 0x00FF00,
    Blue  = 0x0000FF
};

void print_color_good(WebColor color) {
    switch (color) {
        case WebColor::Red:
            std::cout << "red\n";
            break;
        case WebColor::Green:
            std::cout << "green\n";
            break;
        case WebColor::Blue:
            std::cout << "blue\n";
            break;
    }
}


// ❌ 漏写 case 的风险示例
enum class ProductInfo {
    Red,
    Purple,
    Blue
};

void print_product(ProductInfo info) {
    switch (info) {
        case ProductInfo::Red:
            std::cout << "Red product\n";
            break;
        case ProductInfo::Purple:
            std::cout << "Purple product\n";
            break;
        // ❗ 漏掉 Blue
    }
    // 如果开启高等级警告，编译器可能会提示枚举未完全覆盖
}


int main() {

    std::cout << "===== 错误示例 =====\n";
    print_color_bad(1);  // 可读性差


    std::cout << "\n===== 正确示例 =====\n";
    print_color_good(WebColor::Green);


    std::cout << "\n===== switch 漏写示例 =====\n";
    print_product(ProductInfo::Blue);  // 无输出（逻辑缺陷）

    return 0;
}
