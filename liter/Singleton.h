// @file Singleton.h
// @brief 单例模式的实现模板
// @author
// @version
// @date

#pragma once

#include <liter/uncopyable.h>


namespace liter
{

// @class Singleton
// @brief 单例类父类
template <typename T>
class Singleton : public uncopyable
{
public:
		static T* Instance()
		{
        /// in c++11, this instance_obj will be construct once.
        static T instance_obj;
        return &instance_obj;
		}
};


}
