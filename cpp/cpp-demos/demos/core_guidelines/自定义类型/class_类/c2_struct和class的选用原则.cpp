//
// Created by Administrator on 2026/2/14.
//

/*
================================================================================
C++ Core Guidelines — C.2

struct和class的选用原则：
有不变量使用 class；成员可独立变化使用 struct

--------------------------------------------------------------------------------
原则说明：

    如果一个类型存在“不变量”（成员之间存在逻辑约束），
    应使用 class 并通过构造函数建立不变量。

    如果数据成员彼此独立、没有逻辑约束，
    可以使用 struct。

--------------------------------------------------------------------------------
原因：

    1. 提高可读性
       使用 class 暗示该类型具有内部约束。

    2. 强化设计意图
       struct 表示简单数据聚合，
       class 表示具有规则与约束的类型。

    3. 明确不变量责任
       class 的构造函数负责建立不变量，
       之后成员函数可以假设不变量成立。

--------------------------------------------------------------------------------
什么是不变量？

    不变量是对象成员之间必须始终满足的逻辑条件。

    一旦构造函数建立不变量，
    所有公开成员函数都可以依赖该不变量。

--------------------------------------------------------------------------------
核心思想：

    是否存在成员之间的逻辑约束，
    决定使用 class 还是 struct。
================================================================================
*/

#include <iostream>
#include <string>
#include <stdexcept>

//
// ✅ 示例1：成员可独立变化 —— 使用 struct
//
struct Pair {
    std::string name;
    int volume;
    // name 与 volume 之间没有逻辑约束
};


//
// ✅ 示例2：存在不变量 —— 使用 class
// 不变量：日期必须合法
//
enum class Month {
    Jan = 1, Feb, Mar, Apr, May, Jun,
    Jul, Aug, Sep, Oct, Nov, Dec
};

class Date {
public:
    Date(int yy, Month mm, int dd)
        : y_(yy), m_(mm), d_(dd) {
        validate();  // 构造时建立不变量
    }

    int year() const { return y_; }
    Month month() const { return m_; }
    int day() const { return d_; }

private:
    int y_;
    Month m_;
    int d_;

    void validate() const {
        if (d_ <= 0 || d_ > 31) {
            throw std::invalid_argument("Invalid day");
        }
    }
};

int main() {

    std::cout << "===== struct 示例 =====\n";

    Pair p;
    p.name = "Box";
    p.volume = -100;   // 允许（无不变量约束）

    std::cout << p.name << ", volume=" << p.volume << "\n";


    std::cout << "\n===== class 示例 =====\n";

    Date d(2026, Month::Feb, 14);
    std::cout << "Year: " << d.year() << "\n";

    // ❌ 无法破坏内部不变量
    // d.d_ = 100;   // 编译错误（private）

    // ❌ 构造非法对象会抛异常
    // Date bad(2026, Month::Feb, 100);

    return 0;
}
