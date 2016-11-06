# lsytj0413的 liter 库

liter是一个采用C++11/14编写的 Header only 的工具库. 包含一些C++编程中常用的工具类, 可以简化C++代码的编写过程.

## 使用方式

克隆下仓库后, 讲仓库目录加入　*include-path* 即可.

具体使用方式可以参见　*test* 目录下的单元测试用例.

## 编译

liter库只包含头文件, 不需编译即可使用. 用户可以选择编译运行单元测试.

### 单元测试编译方式

1. 下载 *GTEST*　并安装

2. 进入test目录, 执行一下命令:

```
cd liter/test
mkdir build
cd build
cmake ..
make
```

3. 执行编译输出的可执行文件

##  简介

liter 包含一些　C++　工具类, 使用简便, 功能强大.

| 文件 | 内容 | 描述 |
|:--|:--|:--|
| liter/Singleton.h | Singleton | 单例模式的实现模板 |
| liter/SyncQueue.hpp | SyncQueue | 线程安全的队列容器模板  |
| liter/any.hpp | any | 可存放任意类型的any容器 |
| liter/aspect.hpp | aspect | AOP简易实现 |
| liter/binary.hpp | binary | 编译时2进制与10进制工具 |
| liter/callable.h | functor_call | 可调用对象的参数解包示例 |
| liter/command.hpp | command | 命令模式实现模板 |
| liter/curry.hpp | curry | 函数科里化实现模板 |
| liter/events.hpp | events | 观察者模式实现模板 |
| liter/function_traits.hpp | function_traits | 函数特征提取模板 |
| liter/ioc.hpp | Ioc | 控制反转实现模板 |
| liter/lazy.h | lazy | 惰性求值 |
| liter/linq.hpp | linq | 语言集成查询 |
| liter/message_bus.hpp | MessageBus | 消息总线 |
| liter/object_pool.hpp | ObjectPool | 对象池 |
| liter/optional.hpp | optional | 可选值模板 |
| liter/range.hpp | range | 生成范围序列 |
| liter/sequence.hpp | sequence | 序列 |
| liter/string_utils.h | StringUtils | 一些字符串处理相关函数 |
| liter/task.h | task | 并行任务 |
| liter/thread_pool.hpp | ThreadPool | 半同步半异步线程池 |
| liter/timer.h | timer | 计时器 |
| liter/tulpe_helper.hpp | find_index, apply等 | 一些std::tuple的帮助函数 |
| liter/uncopyable.h | uncopyable | 禁止拷贝基类 |
| liter/utils.h | variadic_sum等 | 一些工具函数 |
| liter/variant.hpp | variant | 可变值类型容器 |
| liter/visitor.hpp | visitor | 访问者模式实现模板 |
| liter/DllHelper.h | DllHelper | 动态库调用帮助类, 适用于 Windows |

## 使用示例

### liter::any

```
auto v = liter::any();
v = liter::any(int(1));
v.is<int>();                                 // true
int i = v.cast<int>();                       // 1
v = liter::any(std::string("any"));
v.is<std::string>();                         // true
std::string s = v.cast<std::string>();       // "any"
```

### liter::aspect

```
class A {
public:
    void before(int i){ cout << "A::before" << endl; };
};

class B {
public:
    void after(int i){ cout << "B::after" << endl; };
};

class C {
public:
    void before(int i){ cout << "C::before" << endl; };
    void after(int i){ cout << "C::after" << endl; };
};
void test(int i){ cout << "test" << endl;};

liter::invoke<A, B, C>(test, 10);
A::before
C::before
test
B::after
C::after
```

### liter::command

```
static int test3(int i, int j){
    return i + j;
};

auto f1 = liter::command<int>();
f1.wrap(test3, 10, 30);

auto v1 = f1.execute();      // v1 = 40
```

### liter::curry

```
int test_f2(int i, int j){
    return i + j;
};

auto v = liter::curry(test_f2);
int i = v(1)(10)；       // i = 11
```

### liter::function_traits

```
double h0(int i, double j, char c){
    return j;
};

using R0 = liter::function_traits<decltype(h0)>::return_type;
auto v0 = std::is_same<double, R0>::value;     // v0 = true
auto num0 = liter::function_traits<decltype(h0)>::arity;   // num0 = 3
using arg00 = typename liter::function_traits<decltype(h0)>::template arg_type<0>;
auto v00 = std::is_same<int, arg00>::value;   // v00 = true
using arg01 = typename liter::function_traits<decltype(h0)>::template args<1>::type;
auto v01 = std::is_same<double, arg01>::value;   // v01 = true
```

### liter::linq

```
std::vector<Person> v = {
    {21, "a", "shanghai"}, 
    {22, "bb", "wuhan"},
    {21, "a", "zhuhai"}
};

auto p = liter::from(v).where([](const Person& p){
     return p.age > 21;
}).begin();     // p->age = 22
```

### liter::MessageBus

```
static int g = 0;
static void f0(){
    g += 10;
};

liter::MessageBus m;
m.attach(f0);
m.send<void>();     // g = 10
```

### liter::ObjectPool

```
class A {}; 
liter::ObjectPool<A, 1> pool;
pool.init(1);
auto p0 = pool.get();
```

### liter::optional

```
liter::optional<int> op;
op.init_p();      // false
op.emplace(1);
*op;              // 1
op.init_p();      // true
```

### liter::range

```
auto sum = 0;
for(auto&& i : liter::range(10)) {
    sum += i;
}     // sum = 45
```

### liter::sequence

```
using f = typename liter::make_sequence<5>::type;
auto v = std::is_same<f, liter::sequence<0, 1, 2, 3, 4>>::value;    // v = true
```

### liter::ThreadPool

```
liter::ThreadPool pool;
int g = 0;
pool.add([&](){g = 1;});    // g = 1
```

### liter::variant

```
using T0 = liter::variant<int, char>;
auto v0 = T0();
v0.is<int>();       // false
v0.is<char>();      // false
 v0 = T0(10);
v0.is<int>();       // true
v0.get<int>();      // 10
v0.is<char>();      // false
 v0 = T0('a');
v0.is<int>();       // false
v0.is<char>();      // true
v0.get<char>();     // 'a'
```
