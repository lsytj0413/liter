// @file events.hpp
// @brief 观察者模式实现模板
// @author
// @version
// @date

#pragma once

#include <map>
#include <liter/uncopyable.h>


namespace liter
{

// @class events
// @brief 观察者模式包装类
template <typename F>
class events : public uncopyable
{
private:
    int m_observer_id = 0;
    std::map<int, F> m_connections;

    template <typename Fn>
    int assign(Fn&& f){
        int id = ++m_observer_id;
        m_connections.emplace(id, std::forward<Fn>(f));
        return id;
    };

public:
    events(){};
    ~events(){};

    // @function
    // @brief 注册观察者
    // @param f: 观察函数
    // @return int, 观察者id
    int connect(F&& f){
        return assign(f);
    };

    // @function
    // @brief 注册观察者
    // @param f: 观察函数
    // @return int, 观察者id
    int connect(const F& f){
        return assign(f);
    };

    // @function
    // @brief 取消观察者
    // @param id: 观察者id
    // @return
    void disconnect(int id){
        m_connections.erase(id);
    };

    // @function
    // @brief 通知函数
    // @param args: 可变参数列表
    // @return
    template <typename... TArgs>
    void notify(TArgs&&... args){
        for (auto&& it : m_connections){
            it.second(std::forward<TArgs>(args)...);
        }
    };
};

}
