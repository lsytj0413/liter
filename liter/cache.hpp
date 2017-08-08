// @file cache.hpp
// @brief 函数返回值缓存
// @author
// @version
// @date

#pragma once

#include <functional>
#include <tuple>
#include <memory>
#include <map>
#include <unordered_map>

using std::function;
using std::tuple;
using std::make_shared;
using std::map;


namespace liter
{

// 以函数中不同的参数为 key cache 返回值
template <typename R, typename... Args>
function<R(Args...)> cache(R(*func) (Args...)) {
    auto result_map = make_shared<map<tuple<Args...>, R>>();

    return ([=](Args... args){
            tuple<Args...> t(args...);
            if (result_map->find(t) == result_map->end()) {
                (*result_map)[t] = func(args...);
            }

            return (*result_map)[t];
        });
}

// 函数类型为 key cache 返回值
template<typename R, typename... Args>
function<R(Args...)> sugar(R(*func)(Args...), bool needClear = false) {
    using function_type = function<R(Args...)>;
    static std::unordered_map<decltype(func), function_type> functor_map;

    if (needClear) {
        return functor_map[func] = cache(func);
    }

    if (functor_map.find(func) == functor_map.end()) {
        functor_map[func] = cache(func);
    }

    return functor_map[func];
}

}
