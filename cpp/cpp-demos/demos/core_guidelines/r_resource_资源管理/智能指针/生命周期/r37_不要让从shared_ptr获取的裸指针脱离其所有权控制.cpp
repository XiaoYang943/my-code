//
// Created by Administrator on 2026/2/18.
//

#include <iostream>
#include <memory>
#include <functional>
#include <thread>
#include <chrono>

using std::cout;


struct Connection {
    ~Connection() {
        cout << "Connection destroyed\n";
    }

    void send() {
        cout << "Sending data...\n";
    }
};


/*
 * 一个简单的“事件系统”，
 * 延迟执行回调
 */
void postDelayed(std::function<void()> cb) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    cb();
}



/* ============================================================
 * ❌ 错误示例：把 shared_ptr.get() 传进回调
 * ============================================================
 *
 * 问题：
 * - 回调只保存了裸指针
 * - shared_ptr 在回调执行前可能被 reset
 * - 对象可能已被销毁
 * - 回调中访问悬空指针 → 未定义行为
 */
void badExample() {

    cout << "\n===== BAD EXAMPLE =====\n";

    auto conn = std::make_shared<Connection>();

    Connection* raw = conn.get();   // ❌ 从 shared_ptr 中拿出裸指针

    /*
     * 注册一个延迟回调
     * 回调里使用 raw 指针
     */
    std::thread t([raw]() {
        postDelayed([raw]() {
            cout << "Callback (bad) running...\n";
            raw->send();   // 💥 可能访问已销毁对象
        });
    });

    /*
     * 主线程提前 reset
     * 此时引用计数变 0，对象被销毁
     */
    conn.reset();

    t.join();
}



/* ============================================================
 * ✅ 正确示例：让回调持有 shared_ptr
 * ============================================================
 *
 * 关键点：
 * - 回调捕获 shared_ptr
 * - 引用计数增加
 * - 对象在回调执行期间一定存活
 */
void goodExample() {

    cout << "\n===== GOOD EXAMPLE =====\n";

    auto conn = std::make_shared<Connection>();

    /*
     * 回调直接捕获 shared_ptr
     * 这样回调持有一份所有权
     */
    std::thread t([conn]() {

        postDelayed([conn]() {
            cout << "Callback (good) running...\n";
            conn->send();  // ✅ 安全
        });
    });

    /*
     * 即使这里 reset
     * 回调线程仍然持有一份 shared_ptr
     * 对象不会被销毁
     */
    conn.reset();

    t.join();
}



int main() {

    badExample();

    goodExample();

    return 0;
}
