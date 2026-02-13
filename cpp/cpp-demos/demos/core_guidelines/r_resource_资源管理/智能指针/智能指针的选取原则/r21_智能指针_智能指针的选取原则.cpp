//
// Created by Administrator on 2026/2/9.
//

#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>


void takeUniquePtr(std::unique_ptr<int> uniqPtr) {
    std::cout << "*uniqPtr: " << *uniqPtr << '\n';
}


/**
 * 当需要用到智能指针时，首先分析"所有权语义":
 * 1. 这个对象的所有者是谁(谁负责释放)？
 * 2. 是否需要多个所有者？
 * 3. 这个对象对外暴露时，是在传递所有权，还是只是传递可用性（借用）？
 *
 * 选取智能指针的最佳实践：
 * 一. 默认首选 unique_ptr
 * 1. 什么时候用
 * 唯一拥有者：对象有且只有一个 owner
 * 需要动态生命周期（跨作用域、跨函数返回）
 * 需要多态删除（基类指针指向派生类对象）
 * 需要把对象放容器里并且容器拥有它
 * 2. 为什么首选
 * 零开销（和裸指针几乎一样大小/成本）
 * 强制所有权清晰：不能拷贝，只能移动
 * 更容易推理生命周期，减少悬垂/重复释放
 * 3. 接口怎么传
 * 只借用，不转移所有权：用 T* 或 T&（通常 const T&）
 * 要转移所有权：按值接 unique_ptr<T>（你的 takeUniquePtr 就是）
 * 可能要“拿走”也可能不拿走（条件转移）：按 unique_ptr<T>& 传引用，然后在需要时 std::move(p)
 *
 * 二. 当需要共享所有权时 才去选择 shared_ptr
 * 1. 什么时候用
 * 多个模块/对象都需要延长对象生命周期，且难以明确单一 owner
 * 生命周期横跨：回调/异步任务/事件系统/观察者
 * 图结构/资源缓存：多个地方引用同一资源，最后一个释放
 * 2. 开销成本
 * 引用计数控制块（通常一次动态分配；make_shared 可合并分配）
 * 每次拷贝/析构要改计数（多线程下通常是原子操作）
 * 容易产生隐式的“谁都能活着”：生命周期变得不透明
 * 3. 接口怎么传
 * 共享所有权（函数需要持有一份）：按值 shared_ptr<T>
 * 只借用，不增加共享所有权：T& / T* 最直观 或 const shared_ptr<T>&（但注意：这仍暴露“shared”语义，容易让人以为会共享）(如果你只是用对象，不需要拥有它，直接用 T&/T*，别把 shared_ptr 带进接口。)
 *

 * 三. 不要为了做拷贝而贪图方便地使用 shared_ptr。unique_ptr 不可以被拷贝，但仍可以被移动
 * 如果你只是用对象，不需要拥有它，直接用 T&/T*，别把 shared_ptr 带进接口。
 */
int main() {
    std::cout << '\n';

    auto uniqPtr1 = std::make_unique<int>(2011);

    takeUniquePtr(std::move(uniqPtr1));

    auto uniqPtr2 = std::make_unique<int>(2014);
    auto uniqPtr3 = std::make_unique<int>(2017);

    std::vector<std::unique_ptr<int>> vecUniqPtr {};
    vecUniqPtr.push_back(std::move(uniqPtr2));
    vecUniqPtr.push_back(std::move(uniqPtr3));
    vecUniqPtr.push_back(std::make_unique<int>(2020));

    std::cout << '\n';

    std::for_each(vecUniqPtr.begin(), vecUniqPtr.end(),
                  [](std::unique_ptr<int>& uniqPtr){ std::cout <<  *uniqPtr << '\n'; } );

    std::cout << '\n';
}
