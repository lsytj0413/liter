#include "CurlHandle.h"

#include <assert.h>

namespace lt
{
#define CURL_HANDLE_CHECK(t_curl, rt) \
	{ \
		if (!t_curl) \
		{	\
			return rt;	\
		}	\
	}

	CCurlHandle::CCurlHandle()
		:m_curl(curl_easy_init())
		, m_headers(nullptr)
	{
		if (!m_curl)
		{
			m_last_error = std::string("can't init curl handle.");
		}
	}


	CCurlHandle::~CCurlHandle()
	{
		if (m_curl)
		{
			curl_easy_cleanup(m_curl);
		}

		if (m_headers)
		{
			curl_slist_free_all(m_headers);
		}
	}

	bool CCurlHandle::set_url(std::string url)
	{
		CURL_HANDLE_CHECK(m_curl, false);

		return set_curl_option(m_curl, CURLOPT_URL, url.c_str());
	}

	std::pair<bool, long> CCurlHandle::perform()
	{
		CURL_HANDLE_CHECK(m_curl, std::make_pair(false, -1));

		CURLcode t_rt = curl_easy_perform(m_curl);
		if (CURLE_OK != t_rt)
		{
			m_last_error = curl_easy_strerror(t_rt);
			return std::make_pair(false, -1);
		}

		long t_response_code = -1;
		t_rt = curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &t_response_code);
		if (CURLE_OK != t_rt &&
			!(t_response_code >= 200 && t_response_code < 300))
		{
			m_last_error = curl_easy_strerror(t_rt);
			return std::make_pair(false, t_response_code);
		}

		return std::make_pair(true, t_response_code);
	}

	void CCurlHandle::reset()
	{
		if (m_curl)
		{
			curl_easy_reset(m_curl);
		}
	}

	bool CCurlHandle::set_data_functor(Functor _fun)
	{
		CURL_HANDLE_CHECK(m_curl, false);

		if (set_curl_option(m_curl, CURLOPT_WRITEFUNCTION, write_data_callback)
			&& set_curl_option(m_curl, CURLOPT_WRITEDATA, this))
		{
			m_data_functor = _fun;
			return true;
		}

		return false;
	}

	bool CCurlHandle::set_header_functor(Functor _fun)
	{
		CURL_HANDLE_CHECK(m_curl, false);

		if (set_curl_option(m_curl, CURLOPT_HEADERFUNCTION, write_header_callback)
			&& set_curl_option(m_curl, CURLOPT_HEADERDATA, this))
		{
			m_header_functor = _fun;
			return true;
		}

		return false;
	}

	bool CCurlHandle::set_read_timeout(int _time_read)
	{
		CURL_HANDLE_CHECK(m_curl, false);

		return set_curl_option(m_curl, CURLOPT_TIMEOUT, _time_read);
	}

	bool CCurlHandle::set_connect_timeout(int _time_connect)
	{
		CURL_HANDLE_CHECK(m_curl, false);

		return set_curl_option(m_curl, CURLOPT_CONNECTTIMEOUT, _time_connect);
	}

	bool CCurlHandle::set_ssl_ca_file(std::string _ca_file_name)
	{
		CURL_HANDLE_CHECK(m_curl, false);

		if (_ca_file_name.empty())
		{
			return (set_curl_option(m_curl, CURLOPT_SSL_VERIFYPEER, 0L)
				&& set_curl_option(m_curl, CURLOPT_SSL_VERIFYHOST, 0L));
		}
		else
		{
			return (set_curl_option(m_curl, CURLOPT_SSL_VERIFYPEER, 1L)
				&& set_curl_option(m_curl, CURLOPT_SSL_VERIFYHOST, 2L)
				&& set_curl_option(m_curl, CURLOPT_CAINFO, _ca_file_name.c_str()));
		}
	}

	bool CCurlHandle::set_nosignal(long _b /* = 1L */)
	{
		CURL_HANDLE_CHECK(m_curl, false);

		return set_curl_option(m_curl, CURLOPT_NOSIGNAL, _b);
	}

	bool CCurlHandle::set_accept_encoding(std::string _encode /* = "" */)
	{
		CURL_HANDLE_CHECK(m_curl, false);

		return set_curl_option(m_curl, CURLOPT_ACCEPT_ENCODING, _encode.c_str());
	}

	bool CCurlHandle::set_http_headers(std::vector<std::string>& _headers)
	{
		CURL_HANDLE_CHECK(m_curl, false);

		if (!_headers.empty())
		{
			for (auto _header : _headers)
			{
				m_headers = curl_slist_append(m_headers, _header.c_str());
			}

			return set_curl_option(m_curl, CURLOPT_HTTPHEADER, m_headers);
		}

		return true;
	}

	bool CCurlHandle::set_cookie_file(std::string _cookie_file_name /* = "" */)
	{
		CURL_HANDLE_CHECK(m_curl, false);

		if (!_cookie_file_name.empty())
		{
			return (set_curl_option(m_curl, CURLOPT_COOKIEFILE, _cookie_file_name.c_str())
				&& set_curl_option(m_curl, CURLOPT_COOKIEJAR, _cookie_file_name.c_str()));
		}

		return true;
	}

	bool CCurlHandle::set_follow_location(bool _b)
	{
		CURL_HANDLE_CHECK(m_curl, false);

		return set_curl_option(m_curl, CURLOPT_FOLLOWLOCATION, _b);
	}

	bool CCurlHandle::set_post(int _post /* = 1 */)
	{
		CURL_HANDLE_CHECK(m_curl, false);

		return set_curl_option(m_curl, CURLOPT_POST, _post);
	}

	bool CCurlHandle::set_post_field(char *_data, int _size)
	{
		CURL_HANDLE_CHECK(m_curl, false);

		return (set_curl_option(m_curl, CURLOPT_POSTFIELDS, _data)
			&& set_curl_option(m_curl, CURLOPT_POSTFIELDSIZE, _size));
	}

	bool CCurlHandle::set_custom_request(std::string _request_type)
	{
		CURL_HANDLE_CHECK(m_curl, false);

		return set_curl_option(m_curl, CURLOPT_CUSTOMREQUEST, _request_type.c_str());
	}

	size_t CCurlHandle::write_data_callback(void *contents, size_t size, size_t nmemb, void *userp)
	{
		CCurlHandle *p_curl = (CCurlHandle*)(userp);
		return p_curl->m_data_functor((char*)contents, size*nmemb);
	}

	size_t CCurlHandle::write_header_callback(void *contents, size_t size, size_t nmemb, void *userp)
	{
		CCurlHandle *p_curl = (CCurlHandle*)(userp);
		return p_curl->m_header_functor((char*)contents, size*nmemb);
	}

	template <typename... TArgs>
	bool CCurlHandle::set_curl_option(CURL *curl, CURLoption option, TArgs... args)
	{
		assert(curl);

		CURLcode t_rt = curl_easy_setopt(curl, option, args...);
		if (CURLE_OK != t_rt)
		{
			m_last_error = curl_easy_strerror(t_rt);
			return false;
		}

		return true;
	}

	std::string CCurlHandle::get_last_error() const
	{
		return m_last_error;
	}
#undef CURL_HANDLE_CHECK
}

