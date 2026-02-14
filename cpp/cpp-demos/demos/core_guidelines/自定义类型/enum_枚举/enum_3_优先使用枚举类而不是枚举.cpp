//
// Created by Administrator on 2026/2/14.
//

/*
================================================================================
C++ Core Guidelines — Enum.3

优先使用 enum class，而不是传统 enum

--------------------------------------------------------------------------------
原则说明：

    使用 enum class（强类型枚举），
    避免传统 enum 的隐式整数转换问题。

--------------------------------------------------------------------------------
原因：

    1. 传统 enum 会自动转换为 int
       容易产生意外的类型混用。

    2. 不同枚举类型之间可能发生错误传参，
       编译器无法阻止。

    3. enum class 具有更强的类型安全，
       不会隐式转换为整数。

--------------------------------------------------------------------------------
核心思想：

    枚举本质上是一个“独立类型”，
    不应被当作普通整数使用。
================================================================================
*/

#include <iostream>

//
// ❌ 传统 enum 示例
//
enum WebColor {
    Red   = 0xFF0000,
    Green = 0x00FF00,
    Blue  = 0x0000FF
};

enum ProductInfo {
    RedProduct = 0,
    PurpleProduct = 1,
    BlueProduct = 2
};

void Print_color(int color) {
    std::cout << "color value: " << color << "\n";
}

//
// ✅ enum class 示例
//
enum class SafeWebColor {
    Red   = 0xFF0000,
    Green = 0x00FF00,
    Blue  = 0x0000FF
};

enum class SafeProductInfo {
    Red,
    Purple,
    Blue
};


int main() {

    std::cout << "===== 传统 enum 问题 =====\n";

    WebColor webby = Blue;

    // 问题1：自动转换为 int
    Print_color(webby);
    // 输出: color value: 255

    // 问题2：错误类型也能传入
    Print_color(BlueProduct);
    // 输出: color value: 2
    // 逻辑上明显错误，但编译通过


    std::cout << "\n===== enum class 安全性 =====\n";

    SafeWebColor safeWeb = SafeWebColor::Blue;

    // Print_color(safeWeb);
    // ❌ 编译错误：不能自动转换为 int

    // Print_color(SafeProductInfo::Red);
    // ❌ 编译错误：类型不匹配

    // 如需转换，必须显式转换
    Print_color(static_cast<int>(safeWeb));
    // 输出: color value: 255

    return 0;
}
