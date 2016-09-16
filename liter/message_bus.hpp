#pragma once

#include <map>
#include <string>
#include <functional>

#include <liter/any.hpp>
#include <liter/uncopyable.h>
#include <liter/function_traits.hpp>


namespace liter
{

class MessageBus : uncopyable
{
private:
    std::multimap<std::string, any> m_map;
    using Iterator = typename std::multimap<std::string, any>::iterator;

public:
    template <typename F>
    void attach(F&& f, const std::string& topic = ""){
        auto fn = to_function(std::forward<F>(f));
        add(topic, fn);
    };

    template <typename R>
    void send(const std::string& topic = ""){
        using function_type = std::function<R()>;

        std::string msg_type = topic + typeid(function_type).name();
        auto range = m_map.equal_range(msg_type);
        for (Iterator itor = range.first; itor != range.second; ++itor){
            auto f = itor->second.cast<function_type>();
            f();
        }
    };

    template <typename R, typename... TArgs>
    void send(TArgs&&... args, const std::string& topic = ""){
        using function_type = std::function<R(TArgs...)>;

        std::string msg_type = topic + typeid(function_type).name();
        auto range = m_map.equal_range(msg_type);
        for (Iterator itor = range.first; itor != range.second; ++itor){
            auto f = itor->second.cast<function_type>();
            f(std::forward<TArgs>(args)...);
        }
    };

    template <typename R, typename... TArgs>
    void remove(const std::string& topic = ""){
        using function_type = std::function<R(TArgs...)>;

        std::string msg_type = topic + typeid(function_type).name();
        auto range = m_map.equal_range(msg_type);
        m_map.erase(range.first, range.second);
    };

private:
    template <typename F>
    void add(const std::string& topic, F&& f){
        std::string msg_type = topic + typeid(F).name();
        m_map.emplace(std::move(msg_type), std::forward<F>(f));
    };
};

}
