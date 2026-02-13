//
// Created by Administrator on 2026/1/11.
//

/*
 * cast 是什么：
 * 在 C++ 里，cast 并不只是 static_cast / dynamic_cast / reinterpret_cast / const_cast 等等..
 * 凡是“显式告诉编译器把一个表达式当成另一个类型”的行为，都是 cast
 *
 * 为什么要减少 cast 的使用？
 * cast 会让“编译期就该失败的错误，变成运行期才炸，甚至直接出现 UB (Undefined Behavior，未定义行为)”
 *
 * 最佳实践：
 * template
*/

/*
* 对比	        cast	template
* 保留类型信息	❌	    ✅
* 编译期检查	    ❌	    ✅
* 错误发现时机	运行期	编译期
* 可读性	        差	    好
* 可维护性	    低	    高
 */
#include <iostream>

// 不用cast时，如果类型不兼容，则错误在编译期就被抛出
// struct A {};
// struct B {};
// int main() {
//     A a;
//     B* b = &a;
// }

/*
 * 为什么发生了 UB ：
 * A和B完全不同，把一个 A*，强行转换成 B*，跳过了编译器的安全保护
 */
struct A { int x = 1; };
struct B { int y = 2; };
// int main() {
//     A a;
//     B* b = (B*)&a;     // c风格 cast
//     std::cout << b->y << std::endl;  // UB：1
// }

int main() {
    A a;
    B* b = reinterpret_cast<B*>(&a);
    std::cout << b->y << std::endl;  // UB：1
}

// 这三个转换都是"在规则内"才能转换，目前无法满足“规则“
// int main() {
//     A a;
//     B* b = static_cast<B*>(&a); // 编译不通过
//     B* b = dynamic_cast<B*>(&a); // 编译不通过
//     B* b = const_cast<B*>(&a);  // 编译不通过
// }
