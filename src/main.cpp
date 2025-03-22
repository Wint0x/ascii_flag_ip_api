#include <iostream>
#include <string>
#include <unistd.h>
#include "includes/json.hpp"
#include "includes/utils.hpp"
#include "includes/flag_enumerator.hpp"

#define MSGSIZE 16
#define RSPSIZE 4096

using namespace std;
using json = nlohmann::json;

static const string BASE_URL = "http://ip-api.com/json/{query}?fields=26472447";

string get_ip(void)
{
	const char *command = "curl -s eth0.me";
	char buff[MSGSIZE];

	FILE *fp;
	int status;
	if ((fp = popen(command, "r")) == NULL)
	{
		perror("popen");
		exit(1);
	}

	while (fgets(buff, MSGSIZE, fp) != NULL){};
	if ((status = pclose(fp)) == -1)
	{
		perror("pclose");
		exit(2);
	}

	return utils::strip(std::move(string(buff)));
}

json get_api_request(const string& ip_addr)
{

	const auto target_url = utils::make_url(BASE_URL, ip_addr);
	
	json parser;
	string command{"curl -s "};
	command.append(target_url);

	char buff[RSPSIZE];
	int status;
	FILE* fp;
	if ((fp = popen(command.c_str(), "r")) == NULL)
	{
		perror("popen");
		exit(3);
	}

	while (fgets(buff, RSPSIZE, fp ) != NULL) {};
	if ((status = pclose(fp)) == -1)
	{
		perror("pclose");
		exit(4);
	}

	parser = json::parse(utils::strip(std::move(std::string(buff))));

	return parser;
}

json json_sort_keys(const json& obj)
{
	if (!obj.is_object())
		return obj;

	map<string, json> sorted_map;

	for (const auto& [key, value] : obj.items()) 
        sorted_map[key] = value;

	return json(std::move(sorted_map));
}

void json_formatter(json& obj)
{
	obj = json_sort_keys(obj);
	string flag_code = obj["countryCode"];

	auto get_flags = flags::get_flags();

	auto it = get_flags.find(flag_code);
	if (it != get_flags.cend())
		cout << it->second << '\n';
	else
		cerr << "\x1b[33mFlag not found\x1b[0m\n\n";


	for (const auto& [key, value] : obj.items())
		cout << "[" << key << "] - " << (value == "" ? "Null" : value) << '\n'; // `dump()` returns a JSON-formatted string
}

int main(void)
{
	const string ip_addr = get_ip();
	json response = get_api_request(ip_addr);

	json_formatter(response);

	return EXIT_SUCCESS;
}