#pragma once

#include <map>
#include <liter/uncopyable.h>


namespace liter
{

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

    int connect(F&& f){
        return assign(f);
    };

    int connect(const F& f){
        return assign(f);
    };

    void disconnect(int id){
        m_connections.erase(id);
    };

    template <typename... TArgs>
    void notify(TArgs&&... args){
        for (auto&& it : m_connections){
            it.second(std::forward<TArgs>(args)...);
        }
    };
};

}
