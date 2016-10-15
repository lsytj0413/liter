// @file uncopyable.h
// @brief 禁止拷贝
// @author
// @version
// @date

#pragma once


namespace liter
{

// @class uncopyable
// @brief 不可拷贝类
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
