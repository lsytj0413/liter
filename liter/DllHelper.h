// @file DllHelper.h
// @brief 动态库调用帮助类, 适用于 Windows
// @author
// @version
// @date

#pragma once

#include <string>
#include <map>
#include <functional>
using std::string;
using std::map;
using std::function;

#include <Windows.h>


namespace liter
{

// @class DllHelper
// @brief 动态库调用帮助类
class DllHelper
{
public:
    DllHelper(): m_module(nullptr){};
    ~DllHelper() {
        unload();
    }

    // @function
    // @brief 加载动态库
    // @param dll_path: 动态库路径
    // @return bool. 是否加载成功
    bool load(const string& dll_path)
    {
        m_module = LoadLibraryA(dll_path.data());
        if (nullptr == m_module) {
            return false;
        }

        return true;
    }

    // @function
    // @brief 卸载动态库
    // @return bool. 是否卸载成功
    bool unload()
    {
        if(nullptr == m_module) {
            return true;
        }

        if(FreeLibrary(m_module)) {
            return false;
        }

        m_module = nullptr;
        return true;
    }

    // @function
    // @brief 返回动态库中的函数
    // @param func_name: 函数名
    // @return std::function
    template <typename T>
    function<T> function(const string& func_name)
    {
        auto it = m_map.find(func_name);
        if (m_map.end() == it) {
            auto addr = GetProcAddress(m_module, func_name.c_str());
            if (!addr) {
                return nullptr;
            }

            m_map.insert(std::make_pair(func_name, addr));
            it = m_map.find(func_name);
        }

        return function<T>((T*)(it->second));
    }

    // @function
    // @brief 执行动态库中的函数
    // @param func_name: 待执行函数名称
    // @param args: 可变参数列表
    // @return 函数返回结果
    template <typename T, typename... Args>
    typename std::result_of<std::function<T>(Args...)>::type execute(const string& func_name,
                                                                     Args&&... args)
    {
        auto f = function(func_name);
        if (nullptr == f) {
            string s = "can not find this function " + func_name;
            throw std::exception(s.c_str());
        }

        return f(std::forward<Args>(args)...);
    }

private:
    HMODULE m_module;
    map<string, FARPROC> m_map;
}

}
