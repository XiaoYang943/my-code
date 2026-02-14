//
// Created by Administrator on 2026/2/14.
//

#include <iostream>
#include <cstring>

struct MyStruct {
    int a;
    double b;
};

union MyUnion {
    int a;
    double b;
};

int main() {

    std::cout << "===== 内存大小对比 =====\n";

    std::cout << "sizeof(MyStruct): " << sizeof(MyStruct) << "\n";
    std::cout << "sizeof(MyUnion):  " << sizeof(MyUnion) << "\n";

    /*
        struct：
            每个成员独立占空间
            总大小 ≈ 所有成员大小之和（加对齐）

        union：
            所有成员共享同一块内存
            总大小 = 最大成员大小
    */

    std::cout << "\n===== struct 行为 =====\n";

    MyStruct s;
    s.a = 10;
    s.b = 3.14;

    std::cout << "s.a = " << s.a << "\n";
    std::cout << "s.b = " << s.b << "\n";
    // 两个成员互不影响


    std::cout << "\n===== union 行为 =====\n";

    MyUnion u;
    u.a = 10;
    std::cout << "u.a = " << u.a << "\n";

    u.b = 3.14;   // 覆盖同一块内存
    std::cout << "u.b = " << u.b << "\n";

    std::cout << "u.a (错误读取) = " << u.a << "\n";
    // 未定义行为：现在内存存的是 double，
    // 但你用 int 解释它

    return 0;
}
