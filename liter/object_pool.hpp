// @file object_pool.hpp
// @brief 对象池
// @author
// @version
// @date

#pragma once

#include <string>
#include <map>
#include <memory>
#include <functional>
#include <cassert>

#include <liter/uncopyable.h>


namespace liter
{

// @class ObjectPool
// @brief 对象池
template <typename T, int MaxSize>
class ObjectPool : uncopyable
{
private:
    std::multimap<std::string, std::shared_ptr<T>> m_objects;

    template <typename... TArgs>
    using Constructor = std::function<std::shared_ptr<T>(TArgs...)>;

public:
    // @function
    // @brief 初始化对象池
    // @param num: 个数
    // @param args: 可变参数列表
    // @return
    template <typename... TArgs>
    void init(size_t num, TArgs&&... args){
        assert(num > 0 && num <= MaxSize);

        auto consName = typeid(Constructor<TArgs...>).name();
        for (size_t i = 0; i < num; ++i){
            m_objects.emplace(consName,
                              std::shared_ptr<T>(new T(std::forward<TArgs>(args)...), [this, consName](T* p){
                                      m_objects.emplace(std::move(consName), std::shared_ptr<T>(p));
                                  })
                              );
        }
    };

    // @function
    // @brief 获取一个对象
    // @return shared_ptr<T>
    template <typename... TArgs>
    std::shared_ptr<T> get(){
        auto consName = typeid(Constructor<TArgs...>).name();

        auto range = m_objects.equal_range(consName);
        for (auto it = range.first; it != range.second; ++it){
            auto ptr = it->second;
            m_objects.erase(it);
            return ptr;
        }

        return nullptr;
    };
};

}
