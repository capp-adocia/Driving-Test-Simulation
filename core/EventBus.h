/*****************************************************************//**
 * \file   EventBus.h
 * \brief  事件总线，用于事件发布和订阅
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef EVENTBUS_H
#define EVENTBUS_H
#include "../util/common/standard.h"
#include <entt/entt.hpp>

struct EventType {
    glm::uint32_t id;
};

// 事件优先级
class EventPriority
{
public:
    static constexpr int LOWEST = 0;
    static constexpr int LOW = 1;
    static constexpr int NORMAL = 2;
    static constexpr int HIGH = 3;
    static constexpr int MONITOR = 4;
};

class EventBus {
public:
    static EventBus& GetInstance() {
        static EventBus instance;
        return instance;
    }
    EventBus(const EventBus&) = delete;
    EventBus(EventBus&&) = delete;
    void operator=(const EventBus&) = delete;
    void operator=(EventBus&&) = delete;
    ~EventBus() = default;

    // 订阅事件 指定优先级
    template <typename Event>
    void Subscribe(std::function<void(Event)> listener, int priority = EventPriority::NORMAL) {
        auto& listeners = m_listeners[typeid(Event)];
        listeners.push_back({ priority, [listener](void* event) {
            listener(*static_cast<Event*>(event));
        } });
        // 按优先级对监听器排序
        std::sort(listeners.begin(), listeners.end(), [](const ListenerInfo& a, const ListenerInfo& b) {
            return a.priority > b.priority;
            });
    }
    // 发布事件 优先级顺序发布
    template <typename Event>
    void Publish(const Event& event) {
        auto it = m_listeners.find(typeid(Event));
        if (it != m_listeners.end()) {
            for (auto& listener : it->second) {
                listener.listener(const_cast<void*>(static_cast<const void*>(&event)));
            }
        }
    }

private:
    struct ListenerInfo {
        int priority;
        std::function<void(void*)> listener;
    };

    EventBus() = default;
    // 存储每种事件类型对应的监听器和优先级
private:
    std::unordered_map<std::type_index, std::vector<ListenerInfo>> m_listeners;
};
#define EB_INS EventBus::GetInstance()

#endif // !EVENTBUS_H