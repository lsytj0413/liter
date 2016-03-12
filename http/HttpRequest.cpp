#include "HttpRequest.h"

namespace lt
{
	HttpRequest::HttpRequest(Functor _fun)
		:m_callback(_fun)
	{
		m_type = Type::HTTP_UNKNOWN;
	}


	HttpRequest::~HttpRequest()
	{
	}


	HttpRequest::Functor HttpRequest::get_callback() const
	{
		return m_callback;
	}

	void HttpRequest::set_url(std::string _url)
	{
		m_url = _url;
	}

	std::string HttpRequest::get_url() const
	{
		return m_url;
	}

	void HttpRequest::set_type(Type _type)
	{
		m_type = _type;
	}

	HttpRequest::Type HttpRequest::get_type() const
	{
		return m_type;
	}

	void HttpRequest::set_headers(std::vector<std::string>& _headers)
	{
		m_headers = _headers;
	}

	std::vector<std::string>& HttpRequest::get_headers()
	{
		return m_headers;
	}

	void HttpRequest::set_data(char *_data, int _size)
	{
		m_request_data.assign(_data, _data + _size);
	}

	void HttpRequest::set_data(std::vector<char>& _data)
	{
		m_request_data = _data;
	}

	char* HttpRequest::get_data() const
	{
		if (!m_request_data.empty())
		{
			return const_cast<char*>(&(m_request_data.front()));
		}

		return nullptr;
	}

	int HttpRequest::get_data_size() const
	{
		return m_request_data.size();
	}
}


