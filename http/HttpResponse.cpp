#include "HttpResponse.h"

#include <functional>

using namespace liter::http;

//namespace liter
//{
	HttpResponse::HttpResponse(HttpRequest *_request)
		:m_request(_request)
		, m_is_success(false)
		, m_response_code(-1)
	{
		m_data.clear();
		m_header.clear();
	}


	HttpResponse::~HttpResponse()
	{
		m_data.clear();
		m_header.clear();
	}

	HttpRequest* HttpResponse::get_request() const
	{
		return m_request;
	}

	size_t HttpResponse::write_data(char *_content, size_t _size)
	{
		m_data.insert(m_data.end(), _content, _content + _size);
		return _size;
	}

	size_t HttpResponse::write_header(char *_content, size_t _size)
	{
		m_header.insert(m_header.end(), _content, _content + _size);
		return _size;
	}

	std::vector<char>& HttpResponse::get_header()
	{
		return m_header;
	}

	std::vector<char>& HttpResponse::get_data()
	{
		return m_data;
	}

	void HttpResponse::set_success(bool _success)
	{
		m_is_success = _success;
	}

	bool HttpResponse::is_success() const
	{
		return m_is_success;
	}

	void HttpResponse::set_last_error(std::string& _error)
	{
		m_last_error = _error;
	}

	std::string HttpResponse::get_last_error() const
	{
		return m_last_error;
	}

	void HttpResponse::set_response_code(long _code)
	{
		m_response_code = _code;
	}

	long HttpResponse::get_response_code() const
	{
		return m_response_code;
	}
//}


