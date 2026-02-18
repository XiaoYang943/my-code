//
// Created by Administrator on 2026/2/18.
//

#include <iostream>
#include <memory>
#include <string>

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render() const = 0;
};

class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer() {
        std::cout << "OpenGLRenderer created\n";
    }

    ~OpenGLRenderer() override {
        std::cout << "OpenGLRenderer destroyed\n";
    }

    void render() const override {
        std::cout << "Rendering with OpenGL\n";
    }
};

class VulkanRenderer : public Renderer {
public:
    VulkanRenderer() {
        std::cout << "VulkanRenderer created\n";
    }

    ~VulkanRenderer() override {
        std::cout << "VulkanRenderer destroyed\n";
    }

    void render() const override {
        std::cout << "Rendering with Vulkan\n";
    }
};

std::unique_ptr<Renderer> createRenderer(const std::string& type) {
    if (type == "opengl") {
        return std::make_unique<OpenGLRenderer>();
    }
    else if (type == "vulkan") {
        return std::make_unique<VulkanRenderer>();
    }

    return nullptr;
}

/*
 * 【工厂函数】场景分析：
 *
 * 1. 工厂函数只负责“创建”，不负责“持有”
 *    -> 对象一旦返回，所有权立即转移给调用者
 *
 * 2. 调用方成为该对象的唯一所有者
 *    -> 不存在多个模块同时拥有该对象
 *
 * 3. 对象的生命周期由调用者控制
 *    -> 调用者作用域结束，对象自动析构
 *
 * 4. 不需要共享语义
 *    -> 不存在多个长期持有者
 */

int main() {

    std::cout << "---- create renderer ----\n";

    auto renderer = createRenderer("opengl");

    if (renderer) {
        renderer->render();
    }

    std::cout << "---- end of main ----\n";

} // renderer 离开作用域，自动析构
