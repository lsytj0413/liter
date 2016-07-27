#pragma once

namespace liter
{
	class uncopyable
	{
	protected:
		uncopyable(){};
		~uncopyable(){};

	private:
		uncopyable(const uncopyable&) = delete;
		uncopyable(const uncopyable&&) = delete;
		uncopyable& operator = (const uncopyable&) = delete;
		uncopyable& operator = (const uncopyable&&) = delete;
	};
}
