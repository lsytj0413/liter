#pragma once

#include <future>
#include <functional>
#include <string>
#include <map>
#include <vector>
using std::string;
using std::function;

#include <liter/any.hpp>
#include <liter/uncopyable.h>
#include <liter/variant.hpp>
#include <liter/function_traits.hpp>


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


class task_group : public uncopyable
{
private:
    using return_variant = variant<int, string, double, short, unsigned int>;

    std::multimap<return_variant, any> m_group;
    std::vector<std::shared_future<void>> m_void_group;

public:
    task_group(){};
    ~task_group(){};

    template <typename R, typename = typename std::enable_if<!std::is_same<R, void>::value>::type>
    void run(task<R()>&& task){
        m_group.emplace(R(), task.run());
    };

    void run(task<void()>&& task){
        m_void_group.push_back(task.run());
    };

    template <typename F>
    void run(F&& f){
        run(task<typename std::result_of<F()>::type>(std::forward<F>(f)));
    };

    template <typename F, typename... Funs>
    void run(F&& first, Funs&&... rest){
        run(std::forward<F>(first));
        run(std::forward<Funs>(rest)...);
    };

    void wait(){
        for (auto itor = m_group.begin(); itor != m_group.end(); ++itor){
            // itor->first.visit([&](int a){
            //         future_get<int>(itor->second);
            //     },[&](double b){
            //         future_get<double>(itor->second);
            //     },[&](std::string v){
            //         future_get<std::string>(itor->second);
            //     },[&](short v){
            //         future_get<short>(itor->second);
            //     },[&](unsigned int v){
            //         future_get<unsigned int>(itor->second);
            //     });
        }

        for (auto itor = m_void_group.begin(); itor != m_void_group.end(); ++itor){
            itor->get();
        }
    };

private:
    template <typename T>
    void future_get(any& f){
        f.cast<std::shared_future<T>>().get();
    };
};

template <typename Range>
auto when_all(Range&& range){
    using return_type = typename Range::value_type::return_type;

    auto task = [&range](){
        std::vector<std::shared_future<return_type>> fv;
        for (auto&& task : range){
            fv.emplace_back(task.run());
        }

        std::vector<return_type> v;
        for (auto&& item : fv){
            v.emplace_back(item.get());
        }

        return v;
    };

    return task;
};

namespace detail
{

template <typename R>
struct RangeTrait
{
    using type = R;
};

template <typename R>
struct RangeTrait<std::shared_future<R>>
{
    using type = R;
};

template <typename Range>
auto transform(Range&& range){
    using return_type = typename Range::value_type::return_type;

    std::vector<std::shared_future<return_type>> fv;
    for (auto&& task : range){
        fv.emplace_back(task.run());
    }

    return fv;
};

template <typename Range>
auto get_any_result_pair(Range&& fv){
    size_t size = fv.size();

    for(;;){
        for(size_t i = 0; i < size; ++i){
            if(fv[i].wait_for(std::chrono::milliseconds(1)) == std::future_status::ready){
                return std::make_pair(i, fv[i].get());
            }
        }
    }
};

}

template <typename Range>
auto when_any(Range&& range){
    auto task = [&range](){
        return detail::get_any_result_pair(detail::transform(range));
    };

    return task;
};



}
