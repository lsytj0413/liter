#pragma once

#include "liter/utils/uncopyable.h"


namespace liter
{

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
