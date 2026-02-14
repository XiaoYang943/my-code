//
// Created by Administrator on 2026/2/14.
//

/*
================================================================================
C++ Core Guidelines — Enum.4

为枚举定义必要的操作，使其安全且易用

--------------------------------------------------------------------------------
原则说明：

    如果一个枚举在逻辑上具有某种“行为”，
    应为其定义相应的运算符或辅助函数，
    以避免重复代码和错误使用。

--------------------------------------------------------------------------------
原因：

    1. 提高使用便利性
       使用方式更自然（例如 ++Day）。

    2. 避免错误
       将转换逻辑集中在一个地方，避免重复 static_cast。

    3. 提高可读性
       让枚举更像一个“完整类型”，而不是整数替代品。

--------------------------------------------------------------------------------
核心思想：

    枚举不仅是一组常量，
    也可以是一个具备行为的强类型。
================================================================================
*/

#include <iostream>

enum class Day {
    Mon, Tue, Wed, Thu, Fri, Sat, Sun
};

// ✅ 为枚举定义前置 ++ 运算符
Day& operator++(Day& d)
{
    d = (d == Day::Sun)
        ? Day::Mon
        : static_cast<Day>(static_cast<int>(d) + 1);
    return d;
}

// 辅助输出函数
const char* to_string(Day d)
{
    switch (d) {
        case Day::Mon: return "Mon";
        case Day::Tue: return "Tue";
        case Day::Wed: return "Wed";
        case Day::Thu: return "Thu";
        case Day::Fri: return "Fri";
        case Day::Sat: return "Sat";
        case Day::Sun: return "Sun";
    }
    return "";
}

int main()
{
    std::cout << "===== 没有自定义操作的情况 =====\n";

    Day d1 = Day::Sat;

    // ❌ 每次都要写转换逻辑
    d1 = static_cast<Day>(static_cast<int>(d1) + 1);
    std::cout << "Next day: " << to_string(d1) << "\n";
    // 输出: Sun

    std::cout << "\n===== 定义 ++ 运算符后 =====\n";

    Day today = Day::Sat;
    Day tomorrow = ++today;

    std::cout << "Today: " << to_string(today) << "\n";
    std::cout << "Tomorrow: " << to_string(tomorrow) << "\n";
    // 输出:
    // Today: Sun
    // Tomorrow: Sun

    return 0;
}
