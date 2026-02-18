//
// Created by Administrator on 2026/2/18.
//

#include <iostream>
#include <memory>

using std::cout;


/*
 * 1️⃣ 可修改引用（T&）
 *
 * 语义：
 * - 调用者必须提供一个存在的对象
 * - 函数可以修改该对象
 * - 函数不参与生命周期管理
 */
void modifyValue(int& value) {

    cout << "[modifyValue] modifying value\n";

    value += 19;   // 修改对象
}


/*
 * 2️⃣ 只读引用（const T&）
 *
 * 语义：
 * - 调用者必须提供一个存在的对象
 * - 函数不能修改该对象
 * - 函数不参与生命周期管理
 */
void readOnlyValue(const int& value) {

    cout << "[readOnlyValue] value = "
         << value << '\n';
}


/*
 * 3️⃣ 指针形式（T*）
 *
 * 语义：
 * - 对象可以不存在（nullptr）
 * - 函数需要自行检查是否为空
 * - 函数不参与生命周期管理
 */
void maybePrint(int* value) {

    cout << "[maybePrint] ";

    if (value)
        cout << "value = " << *value << '\n';
    else
        cout << "value is nullptr\n";
}


/*
 * 三种“非拥有（non-owning）”参数形式
 *
 * 核心原则：
 * 如果函数不表达所有权语义（不接管、不共享、不重置），
 * 则参数应为 T&、const T& 或 T*，而不是智能指针。
 *
 * 参数类型本质上是在表达“是否参与生命周期管理”。
 *
 * 三种非拥有形式：
 *
 * 1. T&
 *    - 必须有对象
 *    - 可修改对象
 *    - 不参与生命周期
 *
 * 2. const T&
 *    - 必须有对象
 *    - 只读
 *    - 不参与生命周期
 *
 * 3. T*
 *    - 对象可为空
 *    - 可修改对象
 *    - 不参与生命周期
 */
int main() {

    cout << "\n=== Create shared object ===\n\n";

    auto sharedPtr = std::make_shared<int>(1998);

    cout << "Initial state:\n";
    cout << "  *sharedPtr = " << *sharedPtr << '\n';
    cout << "  use_count  = " << sharedPtr.use_count() << '\n';

    cout << "\n=== Non-owning calls ===\n\n";

    /*
     * 这里显式解引用 shared_ptr，
     * 说明函数只关心 int 本身，
     * 不关心 shared_ptr 的所有权语义。
     */
    modifyValue(*sharedPtr);
    readOnlyValue(*sharedPtr);

    /*
     * T* 形式允许传入 nullptr。
     */
    maybePrint(sharedPtr.get());
    maybePrint(nullptr);

    cout << "\n=== After modify ===\n\n";

    cout << "  *sharedPtr = " << *sharedPtr << '\n';
    cout << "  use_count  = " << sharedPtr.use_count() << '\n';

    cout << '\n';
}

