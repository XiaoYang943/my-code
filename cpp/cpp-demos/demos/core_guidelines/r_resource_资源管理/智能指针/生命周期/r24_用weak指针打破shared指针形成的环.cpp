//
// Created by Administrator on 2026/2/18.
//

#include <iostream>
#include <memory>

/*
 * 核心问题：
 * shared_ptr 使用引用计数，如果对象之间形成循环引用，引用计数永远不会归零，对象就不会被销毁，导致【内存泄漏】。
 *
 *  总结：
 *  任何可能形成环的关系，必须至少有一侧使用 weak_ptr
 */


// ===============================
// ❌ 错误版本：双向都用 shared_ptr
// ===============================
struct BadNode {
    int value;

    // 双向都拥有对方
    std::shared_ptr<BadNode> next;
    std::shared_ptr<BadNode> prev;

    BadNode(int v) : value(v) {
        std::cout << "BadNode " << value << " constructed\n";
    }

    ~BadNode() {
        std::cout << "BadNode " << value << " destructed\n";
    }
};


// ===============================
// ✅ 正确版本：使用 weak_ptr 打破环
// ===============================
struct GoodNode {
    int value;

    std::shared_ptr<GoodNode> next;  // 拥有后继
    std::weak_ptr<GoodNode> prev;    // 仅观察前驱（不拥有）

    GoodNode(int v) : value(v) {
        std::cout << "GoodNode " << value << " constructed\n";
    }

    ~GoodNode() {
        std::cout << "GoodNode " << value << " destructed\n";
    }
};


int main() {

    std::cout << "============================\n";
    std::cout << " BAD: shared_ptr cycle\n";
    std::cout << "============================\n";

    {
        auto n1 = std::make_shared<BadNode>(1);
        auto n2 = std::make_shared<BadNode>(2);

        // 建立双向关系
        n1->next = n2;
        n2->prev = n1;

        /*
            此时引用计数情况：

            n1:
                - main 里的 n1
                - n2->prev
                => use_count = 2

            n2:
                - main 里的 n2
                - n1->next
                => use_count = 2
        */

        std::cout << "n1 use_count: " << n1.use_count() << "\n";
        std::cout << "n2 use_count: " << n2.use_count() << "\n";
    }

    /*
        离开作用域后：

        main 中的 n1 和 n2 被销毁，
        但内部仍然互相持有：

            n1 <---shared--- n2
            n2 <---shared--- n1

        此时：

            n1.use_count = 1
            n2.use_count = 1

        永远不会变成 0，
        所以析构函数不会调用，
        发生内存泄漏，没有打印 destructed。
    */

    std::cout << "Scope exited (BAD)\n\n";


    std::cout << "============================\n";
    std::cout << " GOOD: weak_ptr breaks cycle\n";
    std::cout << "============================\n";

    {
        auto n1 = std::make_shared<GoodNode>(1);
        auto n2 = std::make_shared<GoodNode>(2);

        n1->next = n2;
        n2->prev = n1;  // weak_ptr，不增加引用计数

        /*
            此时引用计数：

            n1:
                - main 中 n1
                => use_count = 1

            n2:
                - main 中 n2
                - n1->next
                => use_count = 2

            注意：
            n2->prev 是 weak_ptr，
            不会增加 n1 的引用计数。
        */

        std::cout << "n1 use_count: " << n1.use_count() << "\n";
        std::cout << "n2 use_count: " << n2.use_count() << "\n";
    }

    /*
        离开作用域后：

        1. main 中 n1 被销毁
           n1.use_count -> 0
           n1 被 delete
           n1->next (shared_ptr) 释放对 n2 的拥有

        2. n2.use_count -> 1 (只剩 main)
           main 的 n2 被销毁
           n2.use_count -> 0
           n2 被 delete

        weak_ptr 不影响引用计数，
        所以循环被打破，
        所有对象正确析构。
    */

    std::cout << "Scope exited (GOOD)\n";
}
