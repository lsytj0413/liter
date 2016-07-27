#pragma once

#include "utils/uncopyable.h"

namespace liter
{
	template <typename T>
	class Singleton : public uncopyable
	{
	public:
		static T* Instance()
		{
			/// 不需要考虑多线程的影响，C++11中会保证只初始化一次
			static T instance_obj;
			return &instance_obj;
		}
	};
}