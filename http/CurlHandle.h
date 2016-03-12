#pragma once

#include <string>
#include <vector>
#include <functional>
#include <utility>

#include <curl/curl.h>

#include <lt/utils/uncopyable.h>

namespace lt
{
	class CCurlHandle : public uncopyable
	{
	private:
		typedef size_t(*write_callback)(void *contents, size_t size, size_t nmemb, void *userp);
		typedef std::function<size_t(char*, size_t)> Functor;

	private:
		CURL *m_curl;
		curl_slist *m_headers;

		Functor m_data_functor;
		Functor m_header_functor;

		std::string m_last_error;
	public:
		CCurlHandle();
		~CCurlHandle();

		bool set_url(std::string url);
		bool set_data_functor(Functor _fun);
		bool set_header_functor(Functor _fun);
		bool set_read_timeout(int _time_read);
		bool set_connect_timeout(int _time_connect);
		bool set_ssl_ca_file(std::string _ca_file_name);
		bool set_nosignal(long _b = 1L);
		bool set_accept_encoding(std::string _encode = "");
		bool set_http_headers(std::vector<std::string>& _headers);
		bool set_cookie_file(std::string _cookie_file_name = "");

		bool set_follow_location(bool _b);
		bool set_post(int _post = 1);
		bool set_post_field(char *_data, int _size);
		bool set_custom_request(std::string _request_type);

		std::pair<bool, long> perform();

		void reset();
		std::string get_last_error() const;

	private:
		static size_t write_data_callback(void *contents, size_t size, size_t nmemb, void *userp);
		static size_t write_header_callback(void *contents, size_t size, size_t nmemb, void *userp);

		template <typename... TArgs>
		bool set_curl_option(CURL *curl, CURLoption option, TArgs... args);
	};

}

