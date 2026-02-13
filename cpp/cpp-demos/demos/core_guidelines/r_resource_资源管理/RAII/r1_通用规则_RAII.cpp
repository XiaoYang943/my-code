//
// Created by Administrator on 2026/1/18.
//

#include <iostream>
#include <new>
#include <string>

class ResourceGuard {
public:
    explicit ResourceGuard(const std::string& res):resource(res){
        std::cout << "Acquire the " << resource << "." <<  '\n';
    }
    ~ResourceGuard(){
        std::cout << "Release the "<< resource << "." << '\n';
    }
private:
    std::string resource;
};

/*
 * 通用规则：使用资源句柄和 RAII 自动管理资源
 * RAII(Resource Acquisition Is Initialization 资源获取即初始化)
 *
 * RAII的理念：
 * 为资源创建一种代理对象(局部对象，其所有者是c++运行时，所以它代理的资源也归c++运行时所有)，代理的构造函数获取资源，代理的析构函数释放资源
 *
 * RAII的例子：
 * STL的容器、智能指针、锁
 */
int main() {

    std::cout << '\n';

    /*
     * resGuard1 是一个局部对象
     *
     * resGuard1 的生命周期：
     *   - 从定义开始
     *   - 到 main 函数结束为止
     *
     * 构造时获取资源
     */
    ResourceGuard resGuard1{"memoryBlock1"};

    std::cout << "\nBefore local scope" << '\n';

    {
        /*
         * 这是一个新的局部作用域
         *
         * resGuard2 的生命周期：
         *   - 从定义开始
         *   - 到这个大括号结束
         */
        ResourceGuard resGuard2{"memoryBlock2"};

        // 在这个作用域内，memoryBlock2 是“被持有”的
    }   // ← 离开作用域，resGuard2 被销毁，析构函数自动调用

    std::cout << "After local scope" << '\n';

    std::cout << '\n';

    std::cout << "\nBefore try-catch block" << '\n';

    try {
        /*
         * resGuard3 同样是一个局部对象
         *
         * 关键点：
         *   即使发生异常，它的析构函数也一定会被调用
         *   这就是 RAII 提供的“异常安全”
         */
        ResourceGuard resGuard3{"memoryBlock3"};

        // 抛出异常，模拟内存分配失败等错误
        throw std::bad_alloc();

        // 下面的代码永远不会执行
    }
    catch (const std::bad_alloc& e) {
        // 异常被捕获
        std::cout << e.what();
    }

    /*
     * try 块结束前：
     *   - resGuard3 已经在“异常栈展开”过程中被正确析构
     *   - memoryBlock3 已经被释放
     */
    std::cout << "\nAfter try-catch block" << '\n';

    std::cout << '\n';
}
