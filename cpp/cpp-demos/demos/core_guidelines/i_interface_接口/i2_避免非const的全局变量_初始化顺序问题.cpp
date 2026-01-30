//
// Created by Administrator on 2026/1/13.
//

#include <iostream>

int base = 0;

// 结果依赖 base
int result = base + 10;

// 稍后才给 base 正确赋值
int init_base() {
    return 100;
}

// 运行期初始化
int real_base = init_base();

/*
* 在“单文件”里，标准保证按“声明顺序”初始化
* 初始化顺序一旦不符合逻辑期望，就会出错
* 在多文件里，这个顺序甚至是不确定的，问题只会更严重
 */
int main() {
    // main 里再修正 base
    base = real_base;

    std::cout << "base   = " << base << std::endl;  // 100
    std::cout << "result = " << result << std::endl;    // UB: 10 ,理论上期望 result 等于110
}

