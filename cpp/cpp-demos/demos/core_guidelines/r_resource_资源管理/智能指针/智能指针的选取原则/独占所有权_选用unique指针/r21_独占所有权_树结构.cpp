//
// Created by Administrator on 2026/2/9.
//

#include <iostream>
#include <memory>
#include <vector>

struct Node {
    int value;
    std::vector<std::unique_ptr<Node>> children;

    explicit Node(int v) : value(v) {
        std::cout << "Node " << value << " created\n";
    }

    ~Node() {
        std::cout << "Node " << value << " destroyed\n";
    }

    void addChild(std::unique_ptr<Node> child) {
        children.push_back(std::move(child));
    }

    void print(int depth = 0) const {
        for (int i = 0; i < depth; ++i)
            std::cout << "  ";

        std::cout << value << "\n";

        for (const auto& child : children) {
            child->print(depth + 1);
        }
    }
};

/**
 * 【树结构】场景分析：
 *
 * 结构特征：
 * 1. 每个节点只能有一个父节点
 * 2. 子节点不能被多个父节点共享
 * 3. 子节点的生命周期严格隶属于父节点
 *
 * 所有权模型：
 * 父节点拥有子节点
 * 子节点的释放由父节点控制
 */
int main() {

    std::cout << "---- create tree ----\n";

    auto root = std::make_unique<Node>(1);

    auto child2 = std::make_unique<Node>(2);
    child2->addChild(std::make_unique<Node>(4));
    child2->addChild(std::make_unique<Node>(5));

    root->addChild(std::move(child2));
    root->addChild(std::make_unique<Node>(3));

    std::cout << "\n---- print tree ----\n";
    root->print();

    std::cout << "\n---- end of main ----\n";

} // root 离开作用域，整棵树自动递归析构
