//
// Created by Administrator on 2026/2/14.
//

/*
================================================================================
C++ Core Guidelines — C.181

避免使用“裸 union”

--------------------------------------------------------------------------------
原则说明：

    不要直接使用没有类型标识的 union。

    裸 union 没有记录当前存储的是哪种类型，
    程序员必须手动记住当前成员类型，
    极易导致类型错误。

--------------------------------------------------------------------------------
原因：

    1. union 不记录当前激活的成员
    2. 错误读取成员会产生未定义行为
    3. 类型错误可能“悄悄发生”，难以察觉
    4. 编译器无法提供保护

--------------------------------------------------------------------------------
核心思想：

    如果一个类型可能存储多种值，
    必须显式记录当前类型。
================================================================================
*/

#include <iostream>
#include <variant>

//
// ❌ 错误示例：裸 union
//
union Value {
    int x;
    double d;
};


//
// ✅ 正确做法1：使用 class 包装 + 类型标识
//
class SafeValue {
public:
    enum class Type { Int, Double };

    SafeValue(int v) : type_(Type::Int) { data_.x = v; }
    SafeValue(double v) : type_(Type::Double) { data_.d = v; }

    Type type() const { return type_; }

    int as_int() const {
        if (type_ != Type::Int)
            throw std::runtime_error("Not an int");
        return data_.x;
    }

    double as_double() const {
        if (type_ != Type::Double)
            throw std::runtime_error("Not a double");
        return data_.d;
    }

private:
    Type type_;

    union {
        int x;
        double d;
    } data_;
};


//
// ✅ 正确做法2：使用 C++17 std::variant
//


int main()
{
    std::cout << "===== 裸 union 示例 =====\n";

    Value v;
    v.d = 987.654;

    // ❌ 未定义行为：读取错误成员
    std::cout << "Read as int: " << v.x << "\n";
    // 输出是不可预测的（位模式解释错误）

    v.x = 123;
    std::cout << "Read as double: " << v.d << "\n";
    // 未定义行为


    std::cout << "\n===== 包装后的安全 union =====\n";

    SafeValue s1(42);
    std::cout << "Safe int: " << s1.as_int() << "\n";

    SafeValue s2(3.14);
    std::cout << "Safe double: " << s2.as_double() << "\n";


    // std::cout << "\n===== 使用 std::variant =====\n";
    //
    // std::variant<int, double> sv;
    //
    // sv = 123;
    // std::cout << "variant int: "
    //           << std::get<int>(sv) << "\n";
    //
    // sv = 123.456;
    // std::cout << "variant double: "
    //           << std::get<double>(sv) << "\n";

    return 0;
}
