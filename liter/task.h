#pragma once

#include <future>
#include <functional>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
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

template <typename Iterator, typename F>
void parallel_foreach(Iterator&& begin, Iterator&& end, F&& f){
    auto partNum = std::thread::hardware_concurrency();
    auto blockSize = std::distance(begin, end) / partNum;
    Iterator last = begin;

    if (blockSize > 0){
        std::advance(last, (partNum-1)*blockSize);
    }
    else {
        last = end;
        blockSize = 1;
    }

    std::vector<std::future<void>> futures;
    for(; begin != last; std::advance(begin, blockSize)){
        futures.emplace_back(std::async([begin, blockSize, &f](){
                    std::for_each(begin, begin + blockSize, f);
                }));
    }

    futures.emplace_back(std::async([&begin, &end, &f](){
                std::for_each(begin, end, f);
            }));

    std::for_each(futures.begin(), futures.end(), [](std::future<void>& f){
            f.get();
        });
};

template <typename... Fns>
void parallel_invoke(Fns&&... fns){
    task_group g;
    g.run(std::forward<Fns>(fns)...);
    g.wait();
};

template <typename Range, typename ReduceF>
auto parallel_reduce(Range&& range, typename Range::value_type& init, ReduceF&& f){
    return parallel_reduce<Range, ReduceF>(range, init, f, f);
};

template <typename Range, typename RangeF, typename ReduceF>
auto parallel_reduce(Range&& range, typename Range::value_type& init, RangeF&& f0, ReduceF&& f1){
    auto partNum = std::thread::hardware_concurrency();
    auto begin = std::begin(range);
    auto end = std::end(range);
    auto blockSize = std::distance(begin, end) / partNum;
    auto last = begin;

    if (blockSize > 0){
        std::advance(last, (partNum-1)*blockSize);
    }
    else {
        last = end;
        blockSize = 1;
    }

    using value_type = typename Range::value_type;
    std::vector<std::future<value_type>> futures;

    for(; begin != last; std::advance(begin, blockSize)){
        futures.emplace_back(std::async([begin, &init, blockSize, &f0](){
                    f0(begin, begin + blockSize, init);
                }));
    }

    futures.emplace_back(std::async([&begin, &end, &init, &f0](){
                f0(begin, end, init);
            }));

    std::vector<value_type> r;
    std::for_each(futures.begin(), futures.end(), [&r](auto f){
            r.emplace_back(f.get());
        });

    return f1(r.begin(), r.end(), init);
};

}
