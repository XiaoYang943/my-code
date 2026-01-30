//
// Created by Administrator on 2026/1/11.
//

#include <iostream>
/*
*

结构体 Wrapper 的内存布局是“连续的”
a 占用 10 * sizeof(int) 字节
guard 紧跟在 a 后面
没有任何“保护边界”

低地址
+---------+
| a[0]    |
| a[1]    |
| a[2]    |
| ...     |
| a[9]    |
+---------+
| guard   |
+---------+
高地址

 */
struct Wrapper {
    int a[10];
    int guard;
};

/*
 * 数组作为参数传递时，退化成指针，长度信息丢失（array decay）
 * 传给函数时：void f(int arr[])
 * 等价于void f(int* arr)
 * arr 只是一个 int*，它的值是 &w.a[0]，但是不知道长度信息
 */
void f(int arr[]) {
    arr[10] = 520; // 编译器不知道合法索引是0-9，arr[10]实际上是把520写到了 guard
}


// arr = 指针 + 长度（10）
// void f_good(std::span<int> arr) {
//     arr.at(10) = 520;       // 直接抛异常，BUG 立刻暴露
// }

/*
 * array_decay(数组退化)问题
 * 用c++20的std::span避免(或gsl::span)
 *
 * 注：range error(数组读写越界)问题也可以用span避免
 */
int main() {
    Wrapper w;
    w.guard = 999;  // 初始化为 999

    f(w.a);

    std::cout << "guard = " << w.guard << std::endl;    // 520
}