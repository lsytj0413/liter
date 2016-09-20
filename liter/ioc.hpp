#pragma once

#include <functional>
#include <unordered_map>
#include <string>
#include <memory>

#include <liter/any.hpp>
#include <liter/uncopyable.h>


namespace liter
{

class Ioc : public uncopyable
{
private:
    std::unordered_map<std::string, any> m_creator_map;

public:
    Ioc(void){};
    ~Ioc(void){};

    template <typename T, typename Depend, typename... TArgs>
    typename std::enable_if<!std::is_base_of<T, Depend>::value>::type
    register_type(const std::string& key){
        std::function<T*(TArgs...)> fn = [](TArgs... args){
            return new T(new Depend(args...));
        };

        register_type(key, std::move(fn));
    };

    template <typename T, typename Depend, typename... TArgs>
    typename std::enable_if<std::is_base_of<T, Depend>::value>::type
    register_type(const std::string& key){
        std::function<T*(TArgs...)> fn = [](TArgs... args){
            return new Depend(args...);
        };

        register_type(key, std::move(fn));
    };

    template <typename T, typename... TArgs>
    T* resolve(const std::string& key, TArgs... args){
        auto itor = m_creator_map.find(key);
        if (itor == m_creator_map.end()){
            return nullptr;
        }

        auto fn = itor->second.cast<std::function<T*(TArgs...)>>();
        return fn(args...);
    };

    template <typename T, typename... TArgs>
    std::shared_ptr<T> resolve_shared(const std::string& key, TArgs... args){
        T* ptr = resolve<T>(key, args...);
        return std::shared_ptr<T>(ptr);
    };

private:
    void register_type(const std::string& key, any ctor){
        if (m_creator_map.find(key) != m_creator_map.end()){
            throw std::invalid_argument("duplicate key");
        }

        m_creator_map.emplace(key, ctor);
    };
};

}
