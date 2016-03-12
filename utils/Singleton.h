#pragma once

#include <mutex>

namespace lt
{
	template <typename T>
	class Singleton
	{
	private:
		static T* m_ins;
		static std::once_flag m_once_flag;

	public:
		static T* Instance()
		{
			std::call_once(m_once_flag, Singleton::create_obj);
			return m_ins;
		}

	private:
		static void create_obj()
		{
			static T _ins;
			Singleton::m_ins = &_ins;
		}
	};

	template <typename T>
	T* Singleton<T>::m_ins = nullptr;

	template <typename T>
	std::once_flag Singleton<T>::m_once_flag;
}