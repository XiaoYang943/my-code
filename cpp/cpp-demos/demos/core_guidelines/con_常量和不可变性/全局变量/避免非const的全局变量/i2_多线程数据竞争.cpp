//
// Created by Administrator on 2026/1/13.
//

#include <iostream>
#include <thread>

int g_counter = 0;

void worker() {
    for (int i = 0; i < 100000; ++i) {
        ++g_counter;   // 数据竞争
    }
}

/*
 * 多线程同时修改全局变量，导致数据竞争(数据竞争的必要条件：共享且可变的状态)，出现UB
 */
int main() {
    std::thread t1(worker);
    std::thread t2(worker);

    t1.join();
    t2.join();

    std::cout << g_counter << std::endl; // UB: g_counter < 200000，而理论上应该 == 200000
}
