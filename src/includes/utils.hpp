#ifndef UTILS_HPP
#define UTILS_HPP
#include <string>
#include <cctype>

namespace utils
{
	using std::string;
	const string make_url(const string& URL, const string& ip_addr)
	{
		string url_template = URL;

		size_t query_pos = URL.find("{query}");
		if (query_pos != string::npos)
			url_template.replace(query_pos, 7, ip_addr); // Where 7 is length of {query}

		return url_template;
	}

	string strip(const string& str)
	{
		auto it_start = std::find_if_not(str.begin(), str.end(), ::isspace);
		auto it_end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();

		// Handle case where string is all spaces
		if (it_start >= it_end)
			return "";

		return string(it_start, it_end);
	}

	string strip(string&& str)
	{
		auto it_start = std::find_if_not(str.begin(), str.end(), ::isspace);
		auto it_end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();

		// Handle case where string is all spaces
		if (it_start >= it_end)
			return "";

		return string(it_start, it_end);
	}
}

#endif