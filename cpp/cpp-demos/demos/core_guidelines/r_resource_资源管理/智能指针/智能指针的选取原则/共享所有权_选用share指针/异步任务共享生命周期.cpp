//
// Created by Administrator on 2026/2/18.
//
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

/*
 * 场景说明：
 *
 * 一个 Worker 发起异步任务。
 * 异步任务会在未来某个时间执行。
 *
 * 关键问题：
 * 主线程和异步线程谁先结束无法确定。
 *
 * 如果主线程结束时对象被销毁，
 * 而异步线程仍在访问它，就会发生悬垂引用。
 *
 * 因此：
 * 必须让“多个执行流”共同拥有该对象,只能使用 shared_ptr。
 *
 */

class Worker : public std::enable_shared_from_this<Worker> {
public:
    Worker() {
        std::cout << "Worker created\n";
    }

    ~Worker() {
        std::cout << "Worker destroyed\n";
    }

    void doAsyncWork() {

        /*
         * shared_from_this()：
         *
         * 当前对象已经被 shared_ptr 管理，
         * 这里获取一个新的 shared_ptr，
         * 使引用计数 +1。
         *
         * 这意味着：
         * 异步线程也成为“owner”。
         */
        auto self = shared_from_this();

        /*
         * lambda 捕获 self（shared_ptr）:延长对象生命周期
         *
         * 只要异步线程未结束，
         * self 就不会释放，
         * Worker 就不会析构。
         */
        std::thread([self]() {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "Async work finished\n";
        }).detach();
    }
};

int main() {

    {
        /*
         * 使用 make_shared 创建 Worker
         *
         * 此时：
         * 引用计数 = 1
         */
        auto worker = std::make_shared<Worker>();

        worker->doAsyncWork();

        /*
         * 主线程 1 秒后离开作用域。
         * 此时 worker 局部变量销毁，
         * 引用计数 -1。
         *
         * 但异步线程仍然持有一个 shared_ptr。
         */
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Main scope ending\n";
    }

    /*
     * 等待足够时间让异步线程结束。
     */
    std::this_thread::sleep_for(std::chrono::seconds(3));
}
