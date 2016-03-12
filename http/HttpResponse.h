#pragma once

#include <vector>

namespace lt
{
	class HttpRequest;

	class HttpResponse
	{
	private:
		HttpRequest *m_request;

		std::vector<char> m_header;
		std::vector<char> m_data;
		std::string m_last_error;

		long m_response_code;
		bool m_is_success;

	public:
		HttpResponse(HttpRequest *_request);
		~HttpResponse();

		HttpRequest* get_request() const;

		size_t write_header(char *_content, size_t _size);
		size_t write_data(char *_content, size_t _size);

		std::vector<char>& get_header();
		std::vector<char>& get_data();

		void set_success(bool _success);
		bool is_success() const;

		void set_last_error(std::string& _error);
		std::string get_last_error() const;

		void set_response_code(long _code);
		long get_response_code() const;
	};
}



