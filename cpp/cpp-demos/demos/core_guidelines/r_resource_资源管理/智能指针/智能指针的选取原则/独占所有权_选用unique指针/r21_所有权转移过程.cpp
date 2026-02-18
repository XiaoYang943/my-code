//
// Created by Administrator on 2026/2/9.
//

#include <iostream>
#include <memory>

struct Logger {

    Logger() {
        std::cout << "Logger created\n";
    }

    ~Logger() {
        std::cout << "Logger destroyed\n";
    }

    void info(const std::string& msg) {
        std::cout << msg << '\n';
    }
};

std::unique_ptr<Logger> createLogger() {
    /*
    * 1. 在堆上分配内存（相当于 new Logger）
    * 2. 构造 Logger 对象
    * 3. 用 unique_ptr 把这块内存“接管”（成为唯一 owner）
    *
    * 堆上：有一个 Logger 对象
    * 栈上：p（unique_ptr）指向它
     */
    auto logger = std::make_unique<Logger>();

    /*
    * 返回时并不会拷贝 unique_ptr（它不能拷贝），而是 移动（move）：
    * p 的所有权转移给调用者
    * p 变成空（nullptr）
    * 所以即使 createLogger() 结束了，堆上的 Logger 仍然存活
     */
    return logger;
}


/*
 * 什么时候用 unique_ptr :
 * 动态生命周期
 *
 * unique_ptr的本质：
 * 本质不是“自动 delete”
 * 而是把 ownership 编码进类型系统。
 */

/*
 * 对象的生命周期：这个对象从“创建完成”到“析构结束”之间的时间段
 * 1. 静态生命周期（栈上对象）
 * 对象“存活多久”由 作用域 {} 决定，离开作用域时自动析构
 *
* 2. 动态生命周期：
* 对象不随创建它的作用域结束而销毁，
* 而是由 ownership（所有权）决定。
* 通常通过堆分配实现。
 */
int main() {

    auto my_logger = createLogger();
    // 此时 my_logger 这个 unique_ptr 成为新 owner。

    my_logger->info("hello");

} // Logger 进行析构、unique_ptr 自动 delete 其管理的 Logger
