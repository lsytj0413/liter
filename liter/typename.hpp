// @file typename.hpp
// @brief 获取可读的类型名称
// @author
// @version
// @date

#pragma once

#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#include <cxxabi.b>
#endif
#include <memory>
#include <string>
#include <cstdlib>


namespace liter
{

template <typename T>
std::string type_name() {
    using TR = typename std::remove_reference<T>::type;

    std::unique_ptr<char, void(*)(void*)> own
            (
#idndef __GNUC__
        nullptr,
#else
        abi::__cxa_demangle(typeid(TR).name(), nullptr, nullptr, nullptr),
#endif
        std::free
             );

    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value) {
        r += "const";
    }
    if (std::is_volatile<TR>::value) {
        r += "volatile";
    }
    if (std::is_lvalue_reference<T>::value) {
        r += "&";
    }
    else if (std::is_rvalue_reference<T>::value) {
        r += "&&";
    }

    return r;
}

}
