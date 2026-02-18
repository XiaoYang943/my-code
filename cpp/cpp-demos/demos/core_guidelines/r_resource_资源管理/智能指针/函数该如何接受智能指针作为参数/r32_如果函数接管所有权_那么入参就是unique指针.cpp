//
// Created by Administrator on 2026/2/18.
//

#include <iostream>
#include <memory>

struct Task {
    Task(int id) : id(id) {
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
 * 【如果函数接管所有权_参数就是unique指针】
 * 函数接管所有权:函数成为对象的最后拥有者
 */
void process(std::unique_ptr<Task> task) {

    std::cout << "process() now owns the task\n";

    task->run();

    std::cout << "process() finished\n";

    // 函数结束时 task 被销毁
    // Task 生命周期在这里终止
}

int main() {

    auto task = std::make_unique<Task>(42);

    std::cout << "Main hands task to process()\n";

    process(std::move(task));   // 所有权转移

    if (!task) {
        std::cout << "Main no longer owns the task\n";
    }

    std::cout << "End of main\n";
}