//
// Created by Administrator on 2026/2/18.
//

#include <iostream>
#include <memory>

struct Task {
    explicit Task(int id) : id(id) {
        std::cout << "Task " << id << " created\n";
    }

    ~Task() {
        std::cout << "Task " << id << " destroyed\n";
    }

    void run() const {
        std::cout << "Task " << id << " is running...\n";
    }

    int id;
};


/*
 * 原则：
 * 如果函数可能改变 unique_ptr 指向的对象，那么参数类型应为 std::unique_ptr<T>&。
 *
 * 语义表达：
 * - 所有权仍然属于调用者
 * - 但函数可能“重新绑定”这个指针
 * - 这叫 reseat（重新指向）
 *
 * 注意：
 * 这不同于 R.32（接管所有权）。
 *
 * void f(std::unique_ptr<T>)      → 接管所有权
 * void f(std::unique_ptr<T>&)     → 可能重新绑定
 */
void retry(std::unique_ptr<Task>& task) {

    std::cout << "retry() checking task...\n";

    if (task->id < 100) {

        std::cout << "Task failed. Replacing with new task.\n";

        /*
         * 这里发生 reseat：
         *
         * 1. 创建新的 Task
         * 2. 销毁旧的 Task
         * 3. task 现在指向新对象
         * 4. 所有权仍然在 main
         */
        task = std::make_unique<Task>(task->id + 1000);
    }
}


int main() {

    auto task = std::make_unique<Task>(42);

    task->run();

    std::cout << "Calling retry...\n";

    /*
     * 注意：这里不需要 std::move
     * 没有转移所有权，只是允许 retry() 修改 task 的指向。
     */
    retry(task);

    std::cout << "After retry, task id = "<< task->id << "\n";

    std::cout << "End of main\n";
}
