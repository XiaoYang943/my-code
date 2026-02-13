//
// Created by Administrator on 2026/1/18.
//
#include <iostream>
#include <string>

struct Record {
    explicit Record(std::string na): name(na) {}
    std::string name;
};

/*
 * 通用规则：避免malloc和free
 *
 * C函数 malloc 分配内存，但是并不调用构造函数；free 释放内存，但是不调用析构函数
 */

int main() {
    // bad
    // Record* p1 = static_cast<Record*>(malloc(sizeof(Record)));  // 只为 Record 对象分配了内存,没有调用构造函数，没有初始化name
    // std::cout << p1->name << '\n';  // UB，p1指向的内存不包含一个已经构造的 Record ,从而 name 也没有构造

    // good
    auto p2 = new Record("Record"); // 调用构造函数
    std::cout << p2->name << '\n';

    // best
    Record p3("Record-");   // 大部分情况下的最佳实践
    std::cout << p3.name << '\n';
}
