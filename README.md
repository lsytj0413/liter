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

