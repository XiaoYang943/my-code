//
// Created by Administrator on 2026/2/15.
//


/*
================================================================================
C++ Core Guidelines — Con.3

默认通过指针或引用传递 const

--------------------------------------------------------------------------------
原则说明：

    当函数不需要修改传入对象时，
    应使用 const T* 或 const T&。

--------------------------------------------------------------------------------
原因：

    1. 防止被调用函数意外修改数据
    2. 提高代码可推理性
    3. 明确表达设计意图
    4. 编译器可帮助检查错误

--------------------------------------------------------------------------------
核心思想：

    只有在确实需要修改时，
    才传递非 const 指针或引用。
================================================================================
*/

#include <iostream>
#include <string>

//
// ❌ 错误示例：不修改却使用非 const 引用
//
void print_bad(std::string& s)   // ❌ 暗示可能修改
{
    std::cout << s << "\n";
}

//
// ✅ 正确示例：只读引用
//
void print_good(const std::string& s)
{
    std::cout << s << "\n";
}

//
// ✅ 需要修改时，才使用非 const
//
void append_exclamation(std::string& s)
{
    s += "!";
}

//
// ❌ 不要移除 const
//
void dangerous(const std::string& s)
{
    // std::string& ref = const_cast<std::string&>(s);  // ❌ 不要这样做
    // ref += "hack";
}

int main()
{
    std::cout << "===== 只读示例 =====\n";

    std::string msg = "hello";

    print_good(msg);   // OK

    print_bad(msg);    // 编译通过，但语义不清晰


    std::cout << "\n===== 修改示例 =====\n";

    append_exclamation(msg);
    std::cout << msg << "\n";   // hello!


    std::cout << "\n===== const 对象示例 =====\n";

    const std::string const_msg = "world";

    print_good(const_msg);  // OK

    // print_bad(const_msg);  // ❌ 编译错误

    return 0;
}