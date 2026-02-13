//
// Created by Administrator on 2026/1/11.
//

#include <iostream>

#include <cstdint>
/*
 * narrowing conversions(窄化转换是对算数值有精度损失的隐式转换)
 *
 * 尽量减少使用，并在必要时使用 gsl::narrow 或 gsl::narrow_cast
 */
int main() {
     // double  -> int        // 小数被截断,但编译器不报错
     double h = 1234.56;
     int i = h;

     std::cout << i << std::endl; // 1234


     // int64_t -> int32_t / uint8_t    // 范围缩小 + 取模
     int big = 300;
     uint8_t v = big;

     std::cout << (int)v << std::endl; // 44（300 % 256） 溢出但你毫不知情


     // int -> uint8_t        // 范围缩小
     int x = 1024;
     uint8_t u8 = x;

     std::cout << (int)u8 << std::endl; // 0（1024 % 256）


     // signed -> unsigned    // 负数变成巨大正数
     int neg = -1;
     unsigned int u = neg;

     std::cout << u << std::endl; // 4294967295（32 位 unsigned）
}