#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include "component.h"

class Entity
{
public:
    Entity(int ID, std::string name = "Empty");
    virtual ~Entity() = default;

    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        component->owner = this;
        auto* ptr = component.get();
        components[typeid(T)] = std::move(component);
        return ptr;
    }

    template<typename T>
    T* GetComponent()
    {
        auto it = components.find(typeid(T));
        return it != components.end() ? static_cast<T*>(it->second.get()) : nullptr;
    }

    // virtual void OnLogicUpdate(float deltaTime) = 0;
    // virtual void OnRenderUpdate(float deltaTime) = 0;
private:
    int m_ID; //暂时使用自增ID
    std::string m_name;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
};