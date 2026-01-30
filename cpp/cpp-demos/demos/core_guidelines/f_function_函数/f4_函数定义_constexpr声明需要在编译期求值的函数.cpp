//
// Created by Administrator on 2026/1/13.
//

/*
* 这个函数的实现满足常量表达式规则，
* 如果参数是编译期常量，你可以在编译期执行它
 */
constexpr auto gcd(int a, int b) {
    while (b != 0){
        auto t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/*
 * constexpr 允许这个函数在需要时被编译期执行
 *
 * 编译期执行的好处
 * 1. 因为 constexpr 的结果会在运行期作为只读存储区域中的常量提供，所以性能高
 * 3. 能在编译期求值的 constexpr 函数是纯函数，因此是线程安全的
 * 2. 编译期计算，把错误前移到编译期，消除运行期不确定性
 *
 * 注意：
 * 不要滥用 constexpr
 *
 * 使用场景：
 * 数学函数 / 算法 / 纯计算    → constexpr
 * 业务    / 配置 / IO       → 不要constexpr
 */
int main() {

    constexpr int i = gcd(11, 121); // 编译期执行，因为变量本身要求常量

    int a = 11;
    int b = 121;
    int j = gcd(a, b);  // 运行期执行，因为参数不是常量

}