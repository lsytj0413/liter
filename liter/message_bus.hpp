// @file message_bus.hpp
// @brief 消息总线
// @author
// @version
// @date

#pragma once

#include <map>
#include <string>
#include <functional>

#include <liter/any.hpp>
#include <liter/uncopyable.h>
#include <liter/function_traits.hpp>


namespace liter
{

// @class MessageBus
// @brief 消息总线类
class MessageBus : uncopyable
{
private:
    std::multimap<std::string, any> m_map;
    using Iterator = typename std::multimap<std::string, any>::iterator;

public:
    // @function
    // @brief 监听消息
    // @param f: 消息处理函数
    // @param topic: 消息主题
    // @return
    template <typename F>
    void attach(F&& f, const std::string& topic = ""){
        auto fn = to_function(std::forward<F>(f));
        add(topic, fn);
    };

    // @function
    // @brief 发送消息
    // @param args: 可变参数列表
    // @param topic: 消息主题
    // @return
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

    // @function
    // @brief 移除监听
    // @param topic: 消息主题
    // @return
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
