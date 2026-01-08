//
// Created by Administrator on 2026/1/8.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

// STL优先理念：尽量复用 STL 中的方法，基础方法不要重复造轮子
int main()
{
    std::vector<int> v = {10, 20, 30, 40, 50};
    int val = 30;

    auto p = std::find(std::begin(v), std::end(v), val);

    if (p != std::end(v))
    {
        std::cout << "找到元素: " << *p << std::endl;
        std::cout << "索引位置: " << std::distance(std::begin(v), p) << std::endl;
    }
    else
    {
        std::cout << "未找到元素" << std::endl;
    }

    return 0;
}