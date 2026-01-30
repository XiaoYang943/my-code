//
// Created by Administrator on 2026/1/13.
//

#include <iostream>

int g_value = 0;

void a() {
    g_value = 42;
}

void b() {
    g_value = -1;
}

/*
 * 全局变量(或指针、引用)任何地方都能改，没有“所有者”，不好维护
 * 都不安全
 */
int main() {
    a();
    std::cout << g_value << std::endl; // 42

    b();
    std::cout << g_value << std::endl; // -1
}
