// @file linq.hpp
// @brief
// @author
// @version
// @date

#pragma once

#include <numeric>
#include <vector>
#include <map>

#include <boost/algorithm/minmax_element.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/range.hpp>
#include <boost/range/join.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>

using namespace boost;
using namespace boost::adaptors;


namespace liter
{

template <typename F>
struct function_traits : public function_traits<decltype(&F::operator())>
{};

template <typename C, typename R, typename... TArgs>
struct function_traits<R(C::*)(TArgs...) const>
{
    using function = std::function<R(TArgs...)>;
};

template <typename F>
typename function_traits<F>::function
to_function(F& lambda){
    return static_cast<typename function_traits<F>::function>(lambda);
};

// @class linq
// @brief
template <typename R>
class linq
{
private:
    R m_linq_range;

public:
    using value_type = typename R::value_type;

public:
    linq(const R& range) : m_linq_range(range)
    {};

    // @function
    // @brief
    // @param f: 筛选函数
    // @return linq
    template <typename F>
    auto where(const F& f){
        return linq<filtered_range<F, R>>(filter(m_linq_range, f));
    };

    // @function
    // @brief
    // @param f: 筛选函数
    // @return linq
    template <typename F>
    auto select(const F& f){
        auto fn = to_function(f);
        return linq<transformed_range<typename function_traits<F>::function, R>>(
            boost::adaptors::transform(m_linq_range, fn));
    };

    // @function
    // @brief 开始位置迭代器
    // @return
    auto begin() const {
        return std::begin(m_linq_range);
    };

    // @function
    // @brief 结束位置迭代器
    // @return
    auto end() const {
        return std::end(m_linq_range);
    };

    // @function
    // @brief 是否为空
    // @return bool
    bool empty() const {
        return begin() == end();
    };

    // @function
    // @brief 反转
    // @return linq
    auto reverse() {
        return linq<boost::reversed_range<R>>(boost::adaptors::reverse(m_linq_range));
    };

    // @function
    // @brief 查找满足条件的第一个值
    // @param f: 条件函数
    // @return value_type
    template <typename F>
    auto first(const F& f){
        return std::find_if(begin(), end(), f);
    };

    // @function
    // @brief 查找满足条件的最后一个值
    // @param f: 条件函数
    // @return value_type
    template <typename F>
    auto last(const F& f){
        return reverse().first(f);
    };

    // @function
    // @brief 是否任意一个满足条件
    // @param f: 条件函数
    // @return bool
    template <typename F>
    auto any(const F& f) const {
        return std::any_of(begin(), end(), f);
    };

    // @function
    // @brief 是否所有满足条件
    // @param f: 条件函数
    // @return bool
    template <typename F>
    auto all(const F& f) const {
        return std::all_of(begin(), end(), f);
    };

    // @function
    // @brief 迭代
    // @param f: 迭代函数
    // @return
    template <typename F>
    void for_each(const F& f) const {
        std::for_each(begin(), end(), f);
    };

    // @function
    // @brief 查找满足条件的第一个值
    // @param f: 条件函数
    // @return value_type
    template <typename F>
    auto contains(const F& f) const {
        return std::find_if(begin(), end(), f);
    };

    // @function
    // @brief 去除重复值
    // @param f: 重复判定函数
    // @return linq
    template <typename F>
    auto distinct(const F& f) const{
        return linq(unique(m_linq_range, f));
    };

    // @function
    // @brief 去除重复值
    // @return linq
    auto distinct() {
        return linq<uniqued_range<R>>(m_linq_range | uniqued);
    };

    // @function
    // @brief 聚合所有值
    // @param f: 聚合函数
    // @return value_type
    template <typename F>
    auto aggregate(const F& f) const {
        if (empty()){
            throw std::logic_error("aggregate on empty object");
        }

        auto it = begin();
        auto value = *it++;
        return std::accumulate(it, end(), std::move(value), f);
    };

    // @function
    // @brief 求和
    // @return value_type
    auto sum() const {
        return aggregate(std::plus<value_type>());
    };

    // @function
    // @brief 返回元素个数
    // @return size_t
    auto count() const {
        return std::distance(begin(), end());
    };

    // @function
    // @brief 返回满足条件的元素个数
    // @param f: 条件函数
    // @return size_t
    template <typename F>
    auto count(const F& f) const {
        return std::count_if(begin(), end(), f);
    };

    // @function
    // @brief 平均值
    // @return value_type
   auto average() {
        return sum() / count();
    };

    // @function
    // @brief 最小值
    // @param f: 比较函数
    // @return value_type
    template <typename F>
    auto min(const F& f) const {
        if (empty()){
            throw std::logic_error("min on empty object");
        }

        return *std::min_element(begin(), end(), f);
    };

    // @function
    // @brief 最小值
    // @return value_type
    auto min() const {
        if (empty()){
            throw std::logic_error("min on empty object");
        }

        return *std::min_element(begin(), end());
    };

    // @function
    // @brief 最大值
    // @param f: 比较函数
    // @return value_type
    template <typename F>
    auto max(const F& f) const {
        if (empty()){
            throw std::logic_error("max on empty object");
        }

        return *std::max_element(begin(), end(), f);
    };

    // @function
    // @brief 最大值
    // @return value_type
    auto max() const {
        if (empty()){
            throw std::logic_error("max on empty object");
        }

        return *std::max_element(begin(), end());
    };

    // @function
    // @brief 最大最小值
    // @param f: 比较函数
    // @return std::pair
    template <typename F>
    auto minmax(const F& f) const {
        if (empty()){
            throw std::logic_error("minmax on empty object");
        }

        auto v = std::minmax_element(begin(), end(), f);
        return std::make_pair(*(v.first), *(v.second));
    };

    // @function
    // @brief 最大最小值
    // @return std::pair
    auto minmax() const {
        if (empty()){
            throw std::logic_error("minmax on empty object");
        }

        auto v = std::minmax_element(begin(), end());
        return std::make_pair(*(v.first), *(v.second));
    };

    // @function
    // @brief 获取索引的值
    // @param index: 索引
    // @return value_type
    template <typename T>
    auto element_at(T index) const {
        return std::next(begin(), index);
    };

    // @function
    // @brief 获取所有的key
    // @return linq
    auto keys() const {
        return linq<boost::select_first_range<R>>(boost::adaptors::keys(m_linq_range));
    };

    // @function
    // @brief 获取所有的value
    // @return linq
    auto values() const {
        return linq<boost::select_second_const_range<R>>(boost::adaptors::values(m_linq_range));
    };

    // @function
    // @brief 获取前n个值
    // @param n: 索引
    // @return linq
    template <typename T>
    auto take(T n) const {
        return linq(slice(m_linq_range, 0, n));
    };

    // @function
    // @brief 获取一部分值
    // @param start: 开始索引
    // @param end: 结束索引
    // @return linq
    template <typename T>
    auto take(T start, T end) const {
        return linq(slice(m_linq_range, start, end));
    };

    // @function
    // @brief 转换为vector
    // @return vector<value_type>
    std::vector<value_type> to_vector() {
        return std::vector<value_type>(begin(), end());
    };

    // @function
    // @brief 获取一部分值
    // @param f: 结束索引判断函数
    // @return linq
    template <typename F>
    auto take_while(const F& f) const {
        return linq(boost::make_iterator_range(begin(),
                                               std::find_if(begin(), end(), f)));
    };

    // @function
    // @brief 跳过前n个值
    // @param n: 索引
    // @return linq
    template <typename T>
    auto skip(T n) const {
        return linq(boost::make_iterator_range(begin() + n, end()));
    };

    // @function
    // @brief 跳过一部分值
    // @param f: 结束索引判断函数
    // @return linq
    template <typename F>
    auto skip_while(const F& f) const {
        return linq(boost::make_iterator_range(std::find_if_not(begin(), end(), f),
                                               end()));
    };

    template <typename T>
    auto step(T n) {
        return stride(m_linq_range, n);
    };

    auto indirect() {
        return linq<boost::indirected_range<R>>(boost::adaptors::indirect(m_linq_range));
    };

    // @function
    // @brief 连接
    // @param other: 另一个序列
    // @return linq
    template <typename R2>
    auto concat(const R2& other) {
        return linq<joined_range<R, const R2>>(boost::join(m_linq_range, other));
    };

    // @function
    // @brief 获取并集
    // @param other: 另一个序列
    // @param r: 结果vector
    // @return
    template <typename R2>
    void except(const R2& other, std::vector<value_type>& r) {
        std::set_difference(begin(), end(), std::begin(other), std::end(other), back_inserter(r));
    };

    // @function
    // @brief 是否包含
    // @param other: 另一个序列
    // @return bool
    template <typename R2>
    bool includes(const R2& other) const {
        return std::includes(begin(), end(), std::begin(other), std::end(other));
    };

    // @function
    // @brief 分组
    // @param f: 分组函数
    // @return multimap
    template <typename Fn>
    std::multimap<typename std::result_of<Fn(value_type)>::type, value_type>
    group_by(const Fn& f) {
        using key_type = decltype(std::declval<Fn>()(std::declval<value_type>()));

        std::multimap<key_type, value_type> r;
        std::for_each(begin(), end(), [&r, &f](value_type item) {
                r.insert(std::make_pair(f(item), item));
            });
        return r;
    };

    // @function
    // @brief 分组
    // @param fnk: key分组函数
    // @param fnv: value分组函数
    // @return multimap
    template <typename KeyFn, typename ValueFn>
    auto group_by(const KeyFn& fnk, const ValueFn& fnv) {
        using key_type = decltype(std::declval<KeyFn>()(std::declval<value_type>()));
        using val_type = decltype(std::declval<ValueFn>()(std::declval<value_type>()));

        std::multimap<key_type, val_type> r;
        std::for_each(begin(), end(), [&r, &fnk, &fnv](value_type item) {
                r.insert(std::make_pair(fnk(item), fnv(item)));
            });
        return r;
    };

    // @function
    // @brief 转换值
    // @return linq
    template <typename T>
    auto cast() {
        std::function<T(value_type)> f = [](value_type item){
            return static_cast<T>(item);
        };

        return linq<transformed_range<std::function<T(value_type)>, R>>(select(f));
    };

    // @function
    // @brief 是否相等
    // @param other: 另一个序列
    // @return bool
    template <typename R2>
    bool equals(const linq<R2>& other) const {
        return count() == other.count() &&
                std::equal(begin(), end(), other.begin());
    };

    // @function
    // @brief 是否相等
    // @param other: 另一个序列
    // @param f: 比较函数
    // @return bool
    template <typename R2, typename F>
    bool equals(const linq<R2>& other, const F& f) const {
        return count() == other.count() &&
                std::equal(begin(), end(), other.begin(), f);
    };

    template <typename R2>
    bool operator==(const linq<R2>& other) const {
        return equals(other);
    };

    template <typename R2>
    bool operator!=(const linq<R2>& other) const {
        return !(*this == other);
    };
};

template <template <typename T> class IteratorRange, typename R>
using Range = IteratorRange<decltype(std::begin(std::declval<R>()))>;

template <typename R>
using iterator_range = Range<boost::iterator_range, R>;

// @function
// @brief linq创建辅助函数
// @param range: 序列
// @return linq
template <typename R>
linq<iterator_range<R>> from(const R& range){
    return linq<iterator_range<R>>(iterator_range<R>(range));
};

// @function
// @brief 压缩
// @param cont: 可变参数列表
// @return
template <typename... T>
auto zip(const T&... cont) {
    auto zip_begin = boost::make_zip_iterator(boost::make_tuple(std::begin(cont)...));
    auto zip_end = boost::make_zip_iterator(boost::make_tuple(std::end(cont)...));

    return boost::make_iterator_range(zip_begin, zip_end);
};

}
