//
// Created by Administrator on 2026/2/18.
//
//
// R.33 示例：使用 unique_ptr<T>& 表达“重新绑定（reseat）”语义
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
 * 【如果函数可能改变 unique_ptr 指向的对象，那么参数就是 unique_ptr 的引用 &】
 *
    R.33：参数是 std::unique_ptr<Task>&

    为什么是引用 (&)？

    因为我们要“修改调用者持有的 unique_ptr”。

    也就是说：
        - 所有权仍然属于调用者
        - 但函数可能改变它指向的对象
        - 这叫 reseat（重新指向）

    如果这里不是引用：

        void retry(std::unique_ptr<Task> task)

    那就变成 R.32（接管所有权），
    调用者会失去所有权 —— 语义完全不同。

    如果写成：

        void retry(Task& task)

    那就不能替换对象，
    只能修改对象内容。

    所以必须是：

        std::unique_ptr<Task>&
*/

void retry(std::unique_ptr<Task>& task) {

    std::cout << "retry() checking task...\n";

    if (task->id < 100) {

        std::cout << "Task failed. Replacing with new task.\n";

        /*
            关键:发生 reseat
            旧对象被销毁
            unique_ptr 现在指向新对象
            所有权仍然在调用者
        */
        task = std::make_unique<Task>(task->id + 1000);
    }
}


int main() {

    auto task = std::make_unique<Task>(42);

    task->run();

    std::cout << "Calling retry...\n";

    /*
        注意：

        这里不需要 std::move，
        因为我们没有转移所有权。

        我们只是允许 retry()
        修改 task 指向的对象。
    */
    retry(task);

    std::cout << "After retry, task id = "<< task->id << "\n";

    std::cout << "End of main\n";
}
