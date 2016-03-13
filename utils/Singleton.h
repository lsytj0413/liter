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
			/// ����Ҫ���Ƕ��̵߳�Ӱ�죬C++11�лᱣֻ֤��ʼ��һ��
			static T instance_obj;
			return &instance_obj;
		}
	};
}