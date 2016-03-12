#pragma once

#include <string>

#include <lt/utils/uncopyable.h>

namespace lt
{
	class HttpRequest;
	class HttpResponse;
	class CCurlHandle;

	class HttpClient : public uncopyable
	{
	private:
		int m_read_timeout;
		int m_connect_timeout;
		std::string m_cookie_file;
		std::string m_ssl_ca_file;

		class CHandleInitHelper
		{
		public:
			static bool init_get_type_handle(CCurlHandle& _handle, HttpClient& _client, HttpRequest& _request, HttpResponse& _response);
			static bool init_put_type_handle(CCurlHandle& _handle, HttpClient& _client, HttpRequest& _request, HttpResponse& _response);
			static bool init_post_type_handle(CCurlHandle& _handle, HttpClient& _client, HttpRequest& _request, HttpResponse& _response);
			static bool init_delete_type_handle(CCurlHandle& _handle, HttpClient& _client, HttpRequest& _request, HttpResponse& _response);

		private:
			static bool init_common(CCurlHandle& _handle, HttpClient& _client, HttpRequest& _request, HttpResponse& _response);
		};

	public:
		HttpClient();
		~HttpClient();
		static HttpClient* Instance();

		bool init();
		void send_request(HttpRequest& _request);

		void set_read_timeout(int _time_read);
		int get_read_timeout() const;
		void set_connect_timeout(int _time_connect);
		int get_connect_timeout() const;
		void set_cookie_file(std::string _cookie_file);
		std::string get_cookie_file() const;
		void set_ssl_ca_file(std::string _ssl_ca_file);
		std::string get_ssl_ca_file() const;

	private:
		void fini();

		void process_get_request(HttpRequest& _request);
		void process_post_request(HttpRequest& _request);
		void process_put_request(HttpRequest& _request);
		void process_delete_request(HttpRequest& _request);

		void invoke_perform(HttpResponse& _response, CCurlHandle& _handle);
		void process_response(HttpResponse& _response);
	};
}



