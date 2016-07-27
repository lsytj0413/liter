#pragma once

#include <functional>
#include <string>
#include <vector>

namespace liter
{
	namespace http
	{
		class HttpResponse;
		class HttpClient;

		class HttpRequest
		{
		public:
			typedef std::function<void(HttpResponse&, HttpClient&)> Functor;
			enum class Type
			{
				HTTP_GET,
				HTTP_PUT,
				HTTP_POST,
				HTTP_DELETE,
				HTTP_UNKNOWN,
			};

		private:
			Type m_type;

			Functor m_callback;
			std::string m_url;
			std::vector<std::string> m_headers;
			std::vector<char> m_request_data;

		public:
			HttpRequest(Functor _fun);
			~HttpRequest();

			Functor get_callback() const;

			void set_url(std::string _url);
			std::string get_url() const;

			void set_type(Type _type);
			Type get_type() const;

			void set_headers(std::vector<std::string>& _headers);
			std::vector<std::string>& get_headers();

			void set_data(char *_data, int _size);
			void set_data(std::vector<char>& _data);
			char* get_data() const;
			int get_data_size() const;
		};
	}
}


