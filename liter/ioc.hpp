// @file ioc.hpp
// @brief 控制反转实现模板
// @author
// @version
// @date

#pragma once

#include <functional>
#include <unordered_map>
#include <string>
#include <memory>

#include <liter/any.hpp>
#include <liter/uncopyable.h>


namespace liter
{

// @class Ioc
// @brief 控制反转实现类
class Ioc : public uncopyable
{
private:
    std::unordered_map<std::string, any> m_creator_map;

public:
    Ioc(void){};
    ~Ioc(void){};

    // @function
    // @brief 注册类型的创建函数
    // @param key: 类型标志字符串
    // @return
    template <typename T, typename Depend, typename... TArgs>
    typename std::enable_if<!std::is_base_of<T, Depend>::value>::type
    register_type(const std::string& key){
        std::function<T*(TArgs...)> fn = [](TArgs... args){
            return new T(new Depend(args...));
        };

        register_type(key, std::move(fn));
    };

    // @function
    // @brief 注册类型的创建函数
    // @param key: 类型名称
    // @return
    template <typename T, typename Depend, typename... TArgs>
    typename std::enable_if<std::is_base_of<T, Depend>::value>::type
    register_type(const std::string& key){
        std::function<T*(TArgs...)> fn = [](TArgs... args){
            return new Depend(args...);
        };

        register_type(key, std::move(fn));
    };

    // @function
    // @brief 创建类型的变量
    // @param key: 类型标志字符串
    // @param args: 可变参数列表
    // @return T*
    template <typename T, typename... TArgs>
    T* resolve(const std::string& key, TArgs... args){
        auto itor = m_creator_map.find(key);
        if (itor == m_creator_map.end()){
            return nullptr;
        }

        auto fn = itor->second.cast<std::function<T*(TArgs...)>>();
        return fn(args...);
    };

    // @function
    // @brief 创建类型的变量
    // @param key: 类型标志字符串
    // @param args: 可变参数列表
    // @return shared_ptr<T>
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
