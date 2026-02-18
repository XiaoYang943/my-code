//
// Created by Administrator on 2026/2/15.
//

/*
================================================================================
C++ Core Guidelines — Con.2

默认将成员函数声明为 const

--------------------------------------------------------------------------------
原则说明：

    如果成员函数不会改变对象的“可观察状态”，
    就应当声明为 const。

--------------------------------------------------------------------------------
原因：

    1. 更精确表达设计意图
    2. 提高可读性
    3. 编译器可以捕获更多错误
    4. 有时带来更好的优化机会

--------------------------------------------------------------------------------
核心思想：

    只读函数必须标记为 const。
================================================================================
*/

#include <iostream>
#include <string>

//
// ❌ 错误示例：未声明为 const
//
class BadPoint {
    int x, y;
public:
    BadPoint(int a, int b) : x(a), y(b) {}

    int getx() { return x; }   // ❌ 应该是 const
};

//
// ✅ 正确示例
//
class Point {
    int x, y;
public:
    Point(int a, int b) : x(a), y(b) {}

    int getx() const { return x; }   // ✅ const
    int gety() const { return y; }
};

//
// ✅ mutable 示例（缓存机制）
//
class Date {
public:
    Date(int y, int m, int d)
        : year(y), month(m), day(d) {}

    const std::string& string_ref() const
    {
        if (cached_string.empty())
            compute_string();   // 在 const 中修改 mutable 成员

        return cached_string;
    }

private:
    void compute_string() const
    {
        cached_string =
            std::to_string(year) + "-" +
            std::to_string(month) + "-" +
            std::to_string(day);
    }

    int year, month, day;
    mutable std::string cached_string;  // 可在 const 中修改
};


int main()
{
    std::cout << "===== const 成员函数示例 =====\n";

    const Point pt(10, 20);

    std::cout << pt.getx() << "\n";  // OK
    std::cout << pt.gety() << "\n";

    // 如果 getx() 没有 const，这里将编译失败


    std::cout << "\n===== mutable 缓存示例 =====\n";

    const Date d(2026, 2, 14);

    std::cout << d.string_ref() << "\n";
    std::cout << d.string_ref() << "\n";  // 第二次走缓存

    return 0;
}