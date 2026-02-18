//
// Created by Administrator on 2026/2/18.
//

/*
 * 原则：
 * 接口必须准确表达生命周期语义，不要滥用智能指针作为参数
 *
 * 总结：
 * 如果函数会操作对象的所有权或生命周期，可以使用智能指针。
 * 如果函数只是“使用对象”，应该使用 T& 或 T*，不要把 shared_ptr 带进接口。
 */


#include <iostream>
#include <memory>

using std::cout;


// ===============================
// ✅ 正确用法：函数修改所有权
// ===============================
void asSmartPointerGood(std::shared_ptr<int>& shr) {

    cout << "asSmartPointerGood \n";

    // 当前引用计数
    cout << "    shr.use_count(): " << shr.use_count() << '\n';

    /*
        reset 会：

        1. 释放当前所管理对象的一份引用
        2. 指向一个新的对象
        3. 修改原有的所有权结构

        也就是说，这个函数确实在“操作生命周期”。

        因此，把 shared_ptr 作为参数是合理的。
    */
    shr.reset(new int(2011));

    cout << "    shr.use_count(): " << shr.use_count() << '\n';

    cout << "asSmartPointerGood \n";
}


// ===============================
// ❌ 错误用法：函数只是使用对象
// ===============================
void asSmartPointerBad(std::shared_ptr<int>& shr) {

    cout << "asSmartPointerBad(sharedPtr2) \n";

    /*
        这里只是修改被指向对象的值：

            *shr += 19;

        它没有：

        - 拷贝 shared_ptr
        - 重置 shared_ptr
        - 改变引用计数
        - 存储 shared_ptr
        - 延长对象生命周期

        只是“使用对象”。

        因此，参数类型不应该是 shared_ptr<int>&，
        应该是：void asSmartPointerBad(int& value)

        调用时：asSmartPointerBad(*sharedPtr2);

        这样接口语义才准确。
    */
    *shr += 19;
}


/*
   - asSmartPointerGood 使用 shared_ptr 是合理的，因为它显式修改了所有权。
   - asSmartPointerBad 使用 shared_ptr 是不合理的，因为它只是“使用对象”。
*/
int main() {

    cout << '\n';

    auto sharedPtr1 = std::make_shared<int>(1998);

    // 拷贝，形成共享所有权
    auto sharedPtr2 = sharedPtr1;

    /*
        此时：sharedPtr1 和 sharedPtr2 指向同一个 int(1998)，引用计数 = 2
    */
    cout << "sharedPtr1.use_count(): " << sharedPtr1.use_count() << '\n';

    cout << '\n';

    /*
        调用正确接口设计的函数。
    */
    asSmartPointerGood(sharedPtr1);

    cout << '\n';

    /*
        现在发生了什么？

        sharedPtr1.reset(new int(2011));

        结果：

        sharedPtr1 -> 指向新对象 2011
        sharedPtr2 -> 仍然指向旧对象 1998

        原来那个 1998 的引用计数从 2 变成 1
        因为 sharedPtr1 不再共享它
    */

    cout << "*sharedPtr1: " << *sharedPtr1 << '\n';
    cout << "sharedPtr1.use_count(): " << sharedPtr1.use_count() << '\n';

    cout << '\n';

    cout << "*sharedPtr2: " << *sharedPtr2 << '\n';
    cout << "sharedPtr2.use_count(): " << sharedPtr2.use_count() << '\n';

    cout << '\n';

    /*
        调用错误接口设计的函数。
    */
    asSmartPointerBad(sharedPtr2);

    cout << "*sharedPtr2: " << *sharedPtr2 << '\n';

    cout << '\n';
}
