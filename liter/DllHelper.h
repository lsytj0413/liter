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

class DllHelper
{
public:
    DllHelper(): m_module(nullptr){};
    ~DllHelper()
    {
        unload();
    }

    bool load(const string& dll_path)
    {
        m_module = LoadLibraryA(dll_path.data());
        if (nullptr == m_module)
        {
            return false;
        }

        return true;
    }

    bool unload()
    {
        if(nullptr == m_module)
        {
            return true;
        }

        auto b = FreeLibrary(m_module);
        if(!b)
        {
            return false;
        }

        m_module = nullptr;
        return true;
    }

    template <typename T>
    function<T> function(const string& func_name)
    {
        auto it = m_map.find(func_name);
        if (m_map.end() == it)
        {
            auto addr = GetProcAddress(m_module, func_name.c_str());
            if (!addr)
            {
                return nullptr;
            }

            m_map.insert(std::make_pair(func_name, addr));
            it = m_map.find(func_name);
        }

        return function<T>((T*)(it->second));
    }

    template <typename T, typename... Args>
    typename std::result_of<std::function<T>(Args...)>::type execute(const string& func_name,
                                                                     Args&&... args)
    {
        auto f = function(func_name);
        if (nullptr == f)
        {
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
