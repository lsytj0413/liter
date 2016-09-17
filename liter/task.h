#pragma once

#include <future>
#include <functional>
using std::function;


namespace liter
{

template <typename... R>
class task;

template <typename R, typename... TArgs>
class task<R(TArgs...)>
{
private:
    function<R(TArgs...)> m_fn;

public:
    using return_type = R;

public:
    task(function<R(TArgs...)>&& f): m_fn(std::move(f)){};
    task(function<R(TArgs...)>& f): m_fn(f){};

    ~task(){};

    void wait(){
        std::async(m_fn).wait();
    };

    template <typename... TArgs0>
    R get(TArgs0&&... args){
        return std::async(m_fn, std::forward<TArgs0>(args)...);
    };

    std::shared_future<R> run(){
        return std::async(m_fn);
    };

    template <typename F>
    auto then(F&& f){
        using result_type = typename std::result_of<F(R)>::type;

        auto func = std::move(m_fn);
        return task<result_type(TArgs...)>([func, &f](TArgs&&... args){
                std::future<R> last_fn = std::async(func, std::forward<TArgs>(args)...);
                return std::async(f, last_fn.get()).get();
            });
    };
};

}
