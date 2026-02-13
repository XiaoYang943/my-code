//
// Created by Administrator on 2026/2/9.
//

#include <iostream>
#include <memory>
#include <vector>

/**
 * 树形结构
 *
 * 一个节点只能有一个父节点 -> 子节点必须只有一个所有者
 */
struct Node {
    int value;
    std::vector<std::unique_ptr<Node>> children;

    Node(int v) : value(v) {
        std::cout << "Node " << value << " created\n";
    }

    ~Node() {
        std::cout << "Node " << value << " destroyed\n";
    }

    void addChild(std::unique_ptr<Node> child) {
        children.push_back(std::move(child));
    }
};

/*
 * 什么时候用 unique_ptr :
 * 对象在同一时间只能有一个所有者(不能被随意共享的)
 */
int main() {

    auto root = std::make_unique<Node>(1);

    root->addChild(std::make_unique<Node>(2));
    root->addChild(std::make_unique<Node>(3));

} // root 离开作用域
