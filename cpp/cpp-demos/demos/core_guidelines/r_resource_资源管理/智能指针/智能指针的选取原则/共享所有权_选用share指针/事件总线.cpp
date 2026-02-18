//
// Created by Administrator on 2026/2/18.
//
#include <iostream>
#include <memory>
#include <vector>
#include <functional>

/*
 * =========================
 *  通用事件总线（EventBus）
 * =========================
 *
 * 背景：
 * EventBus 的实现是【发布-订阅（Publish–Subscribe）】机制。
 *
 * 在该机制中：
 * - 一个 Event 被发布
 * - 多个 Subscriber 接收并处理该 Event
 * - Subscriber 可能同步处理，也可能异步处理
 *
 * 关键设计问题：
 * Event 的生命周期应该由谁控制？
 *
 * 结论：
 * Event 的生命周期不能由：
 * - 发布者控制
 * - EventBus 控制
 * - 任意单个 Subscriber 控制
 *
 * 而必须由：
 * - “仍在使用该 Event 的最后一个 Subscriber”控制
 *
 * 这是一种【共享所有权 + 引用计数】语义，
 * 在 C++ 中，唯一与之精确匹配的工具是 std::shared_ptr。
 */

class Event {
public:
    virtual ~Event() = default;
};

class DataEvent : public Event {
public:
    explicit DataEvent(int value) : value_(value) {
        std::cout << "DataEvent created\n";
    }

    ~DataEvent() override {
        std::cout << "DataEvent destroyed\n";
    }

    int value() const { return value_; }

private:
    int value_;
};

class EventBus {
public:
    /*
    * Subscriber 表示一个“事件消费者”。
    *
    * 设计约束：
    * 1. Subscriber 的数量是动态的（0..N）
    * 2. Subscriber 的处理方式是不确定的：
    *    - 可能同步处理并立即返回
    *    - 可能异步处理（保存 Event，稍后处理）
    *
    * 直接后果：
    * - EventBus 无法知道某个 Event 何时“真正不再被使用”
    * - 发布者在 publish() 返回时也无法安全销毁 Event
    *
    * 因此：
    * Event 的生命周期必须独立于 publish() 调用栈，
    * 并且能够被 Subscriber 显式延长。
    *
    * std::shared_ptr 正是用于表达：“对象存活到最后一个使用者释放为止”。
    */
    using Subscriber = std::function<void(std::shared_ptr<Event>)>;

    void subscribe(Subscriber subscriber) {
        subscribers_.push_back(std::move(subscriber));
    }

    void publish(std::shared_ptr<Event> event) {

        /*
         * publish 的职责：
         * - 分发 Event
         * - 但不是管理 Event 的最终生命周期
         *
         * 广播语义意味着：
         * - 同一个 Event 会被交给多个 Subscriber
         * - 每个 Subscriber 都可能独立决定：
         *     是否立即处理
         *     是否延后处理
         *     是否将 Event 传递给其他执行单元
         *
         * 因此：
         * publish() 只能“传播所有权”，而不能“集中管理销毁时机”。
         */
        for (auto& subscriber : subscribers_) {
            subscriber(event);
        }

        /*
           * publish() 返回时：
           * - EventBus 无法假设 Event 已经不再被使用
           * - Event 是否销毁，只能由 shared_ptr 的引用计数决定
           *
           * 如果某个 Subscriber 仍然持有 shared_ptr：
           * - Event 必须继续存活
           *
           * 如果所有 Subscriber 都已释放：
           * - Event 在引用计数归零时自动析构
           */
    }

private:
    std::vector<Subscriber> subscribers_;
};

int main() {

    EventBus bus;

    /*
     * 订阅者 A
     */
    bus.subscribe([](std::shared_ptr<Event> e) {
        auto data = std::dynamic_pointer_cast<DataEvent>(e);
        if (data) {
            std::cout << "Subscriber A received: "
                      << data->value() << "\n";
        }
    });

    /*
     * 订阅者 B
     */
    bus.subscribe([](std::shared_ptr<Event> e) {
        auto data = std::dynamic_pointer_cast<DataEvent>(e);
        if (data) {
            std::cout << "Subscriber B received: "
                      << data->value() << "\n";
        }
    });

    /*
     * 发布事件
     */
    auto event = std::make_shared<DataEvent>(42);

    bus.publish(event);

    std::cout << "End of publish\n";

    /*
     * main 结束时：
     * 如果没有订阅者保存 event，
     * shared_ptr 引用计数归零，
     * DataEvent 自动析构。
     */
}