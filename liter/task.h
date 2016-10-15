// @file task.h
// @brief 并行任务
// @author
// @version
// @date

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

// @class task
// @brief 任务类
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

    // @function
    // @brief 等待任务完成
    // @return
    void wait(){
        std::async(m_fn).wait();
    };

    // @function
    // @brief 获取任务返回值
    // @param args: 可变参数列表
    // @return R
    template <typename... TArgs0>
    R get(TArgs0&&... args){
        return std::async(m_fn, std::forward<TArgs0>(args)...).get();
    };

    // @function
    // @brief 异步执行任务
    // @return shared_future<R>
    std::shared_future<R> run(){
        return std::async(m_fn);
    };

    // @function
    // @brief 链式调用
    // @param f: 下一个函数
    // @return task
    template <typename F>
    auto then(F&& f){
        using result_type = typename std::result_of<F(R)>::type;

        auto func = std::move(m_fn);
        return task<result_type(TArgs...)>([func, f](TArgs&&... args){
                std::future<R> last_fn = std::async(func, std::forward<TArgs>(args)...);
                return std::async(f, last_fn.get()).get();
            });
    };
};

// @class task_group
// @brief 任务组
class task_group : public uncopyable
{
private:
    using return_variant = liter::variant<int, string, double, short, unsigned int>;

    std::multimap<return_variant, any> m_group;
    std::vector<std::shared_future<void>> m_void_group;

public:
    task_group(){};
    ~task_group(){};

    // @function
    // @brief 异步执行任务
    // @param task: 任务
    // @return
    template <typename R,
              typename = typename std::enable_if<!std::is_same<R, void>::value>::type>
    void run(task<R()>&& task){
        m_group.emplace(R(), task.run());
    };

    // @function
    // @brief 异步执行任务
    // @param task: 任务
    // @return
    void run(task<void()>&& task){
        m_void_group.push_back(task.run());
    };

    // @function
    // @brief 异步执行任务
    // @param task: 任务
    // @return
    template <typename F>
    void run(F&& f){
        run(task<typename std::result_of<F()>::type>(std::forward<F>(f)));
    };

    // @function
    // @brief 异步执行多个任务
    // @param first: 任务之一
    // @param rest: 余下的任务
    // @return
    template <typename F, typename... Funs>
    void run(F&& first, Funs&&... rest){
        run(std::forward<F>(first));
        run(std::forward<Funs>(rest)...);
    };

    // @function
    // @brief 等待所有任务完成
    // @return
    void wait(){
        for (auto itor = m_group.begin(); itor != m_group.end(); ++itor){
            auto& v = const_cast<return_variant&>(itor->first);
            v.visit([&](int a){
                    future_get<int>(itor->second);
                },[&](double b){
                    future_get<double>(itor->second);
                },[&](std::string v){
                    future_get<std::string>(itor->second);
                },[&](short v){
                    future_get<short>(itor->second);
                },[&](unsigned int v){
                    future_get<unsigned int>(itor->second);
                });
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

// @function
// @brief 等待所有任务完成
// @param range: 任务列表
// @return vecotr, 任务执行结果
template <typename Range>
auto when_all(Range&& range){
    using return_type = typename std::remove_reference<Range>::type::value_type::return_type;

    auto task = [&range](){
        std::vector<std::shared_future<return_type>> fv;
        for (auto task : range){
            fv.emplace_back(task.run());
        }

        std::vector<return_type> v;
        for (auto item : fv){
            v.emplace_back(item.get());
        }

        return v;
    };

    return task();
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

// @function
// @brief 等待其中一个任务完成
// @param range: 任务列表
// @return std::pair
template <typename Range>
auto when_any(Range&& range){
    auto task = [&range](){
        return detail::get_any_result_pair(detail::transform(range));
    };

    return task;
};

// @function
// @brief 并行序列循环
// @param begin: 起始迭代器
// @param end: 结束迭代器
// @param f: 迭代函数
// @return
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

// @function
// @brief 异步调用函数
// @param fns: 可变函数列表
// @return
template <typename... Fns>
void parallel_invoke(Fns&&... fns){
    task_group g;
    g.run(std::forward<Fns>(fns)...);
    g.wait();
};

// @function
// @brief 并行reduce
// @param range: 值序列
// @param init: 初始值
// @param f0: 迭代函数
// @param f1: reduce函数
// @return
template <typename Range, typename RangeF, typename ReduceF>
auto parallel_reduce(Range&& range,
                     typename std::remove_reference<Range>::type::value_type init,
                     RangeF&& f0,
                     ReduceF&& f1)
{
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

    using value_type = typename std::remove_reference<Range>::type::value_type;
    std::vector<std::future<value_type>> futures;

    for(; begin != last; std::advance(begin, blockSize)){
        futures.emplace_back(std::async([begin, &init, blockSize, &f0]() -> value_type{
                    return f0(begin, begin + blockSize, init);
                }));
    }

    futures.emplace_back(std::async([&begin, &end, &init, &f0]() -> value_type{
                return f0(begin, end, init);
            }));

    std::vector<value_type> r;
    std::for_each(futures.begin(), futures.end(), [&r](std::future<value_type>& f){
            r.emplace_back(f.get());
        });

    return f1(r.begin(), r.end(), init);
};

// @function
// @brief 并行reduce
// @param range: 值序列
// @param init: 初始值
// @param f: 迭代函数与reduce函数
// @return
template <typename Range, typename ReduceF>
auto parallel_reduce(Range&& range,
                     typename std::remove_reference<Range>::type::value_type init,
                     ReduceF&& f){
    return parallel_reduce(std::forward<Range>(range),
                           init,
                           std::forward<ReduceF>(f),
                           std::forward<ReduceF>(f));
};

}
