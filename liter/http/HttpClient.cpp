#include "HttpClient.h"

#include <functional>
#include <utility>

#include <curl/curl.h>

#include <utils/Singleton.h>
#include <http/HttpResponse.h>
#include <http/HttpRequest.h>
#include <http/CurlHandle.h>

using namespace liter::http;

//namespace liter
//{
#define INIT_AND_PERFORM(x) \
	{ \
		if ((x)) \
		{	\
			invoke_perform(t_response, t_handle);	\
		}	\
		else  \
		{  \
			t_response.set_success(false);	\
			t_response.set_last_error(t_handle.get_last_error());	\
		}	\
	}

	HttpClient::HttpClient()
	{

	}

	HttpClient::~HttpClient()
	{
		fini();
	}

	HttpClient* HttpClient::Instance()
	{
		return Singleton<HttpClient>::Instance();
	}

	bool HttpClient::init()
	{
		return CURLE_OK == curl_global_init(CURL_GLOBAL_ALL);
	}

	void HttpClient::fini()
	{
		curl_global_cleanup();
	}

	void HttpClient::send_request(HttpRequest& _request)
	{
		switch (_request.get_type())
		{
		case HttpRequest::Type::HTTP_GET:
			process_get_request(_request);
			break;
		case HttpRequest::Type::HTTP_POST:
			process_post_request(_request);
			break;
		case HttpRequest::Type::HTTP_PUT:
			process_put_request(_request);
			break;
		case HttpRequest::Type::HTTP_DELETE:
			process_delete_request(_request);
			break;
		case HttpRequest::Type::HTTP_UNKNOWN:
		default:
			break;
		}
	}

	void HttpClient::process_get_request(HttpRequest& _request)
	{
		HttpResponse t_response(&_request);
		CCurlHandle t_handle;

		INIT_AND_PERFORM(CHandleInitHelper::init_get_type_handle(t_handle, *this, _request, t_response));

		process_response(t_response);
	}

	void HttpClient::process_post_request(HttpRequest& _request)
	{
		HttpResponse t_response(&_request);
		CCurlHandle t_handle;

		INIT_AND_PERFORM(CHandleInitHelper::init_post_type_handle(t_handle, *this, _request, t_response));

		process_response(t_response);
	}

	void HttpClient::process_put_request(HttpRequest& _request)
	{
		HttpResponse t_response(&_request);
		CCurlHandle t_handle;

		INIT_AND_PERFORM(CHandleInitHelper::init_put_type_handle(t_handle, *this, _request, t_response));

		process_response(t_response);
	}

	void HttpClient::process_delete_request(HttpRequest& _request)
	{
		HttpResponse t_response(&_request);
		CCurlHandle t_handle;

		INIT_AND_PERFORM(CHandleInitHelper::init_delete_type_handle(t_handle, *this, _request, t_response));

		process_response(t_response);
	}

	void HttpClient::invoke_perform(HttpResponse& _response, CCurlHandle& _handle)
	{
		std::pair<bool, long> t_ret = _handle.perform();
		_response.set_success(t_ret.first);
		_response.set_response_code(t_ret.second);

		if (!t_ret.first)
		{
			_response.set_last_error(_handle.get_last_error());
		}
	}

	void HttpClient::process_response(HttpResponse& _response)
	{
		if (HttpRequest *p_request = _response.get_request())
		{
			std::function<void(HttpResponse&, HttpClient&)> t_callback = p_request->get_callback();
			if (t_callback)
			{
				t_callback(_response, *this);
			}
		}
	}

	void HttpClient::set_read_timeout(int _time_read)
	{
		m_read_timeout = _time_read;
	}

	int HttpClient::get_read_timeout() const
	{
		return m_read_timeout;
	}

	void HttpClient::set_connect_timeout(int _time_connect)
	{
		m_connect_timeout = _time_connect;
	}

	int HttpClient::get_connect_timeout() const
	{
		return m_connect_timeout;
	}

	void HttpClient::set_cookie_file(std::string _cookie_file)
	{
		m_cookie_file = _cookie_file;
	}

	std::string HttpClient::get_cookie_file() const
	{
		return m_cookie_file;
	}

	void HttpClient::set_ssl_ca_file(std::string _ssl_ca_file)
	{
		m_ssl_ca_file = _ssl_ca_file;
	}

	std::string HttpClient::get_ssl_ca_file() const
	{
		return m_ssl_ca_file;
	}

	bool HttpClient::CHandleInitHelper::init_get_type_handle(CCurlHandle& _handle,
		HttpClient& _client,
		HttpRequest& _request,
		HttpResponse& _response)
	{
		if (CHandleInitHelper::init_common(_handle, _client, _request, _response))
		{
			return _handle.set_follow_location(true);
		}

		return false;
	}

	bool HttpClient::CHandleInitHelper::init_put_type_handle(CCurlHandle& _handle,
		HttpClient& _client,
		HttpRequest& _request,
		HttpResponse& _response)
	{
		if (CHandleInitHelper::init_common(_handle, _client, _request, _response))
		{
			return (_handle.set_custom_request("PUT")
				&& _handle.set_post_field(_request.get_data(), _request.get_data_size()));
		}

		return false;
	}

	bool HttpClient::CHandleInitHelper::init_post_type_handle(CCurlHandle& _handle,
		HttpClient& _client,
		HttpRequest& _request,
		HttpResponse& _response)
	{
		if (CHandleInitHelper::init_common(_handle, _client, _request, _response))
		{
			return (_handle.set_post()
				&& _handle.set_post_field(_request.get_data(), _request.get_data_size()));
		}

		return false;
	}

	bool HttpClient::CHandleInitHelper::init_delete_type_handle(CCurlHandle& _handle,
		HttpClient& _client,
		HttpRequest& _request,
		HttpResponse& _response)
	{
		if (CHandleInitHelper::init_common(_handle, _client, _request, _response))
		{
			return (_handle.set_custom_request("DELETE")
				&& _handle.set_follow_location(true));
		}

		return false;
	}

	bool HttpClient::CHandleInitHelper::init_common(CCurlHandle& _handle,
		HttpClient& _client,
		HttpRequest& _request,
		HttpResponse& _response)
	{
		return (_handle.set_url(_request.get_url())
			&& _handle.set_data_functor(std::bind(&HttpResponse::write_data, &_response, std::placeholders::_1, std::placeholders::_2))
			&& _handle.set_header_functor(std::bind(&HttpResponse::write_header, &_response, std::placeholders::_1, std::placeholders::_2))
			&& _handle.set_read_timeout(_client.get_read_timeout())
			&& _handle.set_connect_timeout(_client.get_connect_timeout())
			&& _handle.set_ssl_ca_file(_client.get_ssl_ca_file())
			&& _handle.set_nosignal()
			&& _handle.set_accept_encoding()
			&& _handle.set_http_headers(_request.get_headers())
			&& _handle.set_cookie_file(_client.get_cookie_file()));
	}

#undef INIT_AND_PERFORM
//}                         

