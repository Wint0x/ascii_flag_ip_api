#ifndef FLAG_ENUMERATOR_HPP
#define FLAG_ENUMERATOR_HPP
#include <iostream>
#include <string>
#include <map>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <sstream>

namespace flags
{
	using std::map;
	using std::string;

	static const string FLAG_DIR = "./flags";
	static map<string, string> flag_info{};

	map<string, string>& get_flags()
	{
		struct stat dir_info;
		struct dirent* entry;

		DIR* dir = nullptr;
		std::ifstream is;
		std::ostringstream os;

		size_t dot_pos;

		if (stat(FLAG_DIR.c_str(), &dir_info) != 0)
		{
			std::cerr << "\x1b[31m./flags directory missing! Program will not display a flag!\x1b[0m\n";
			return flag_info;
		}

		else
		{
			dir = opendir(FLAG_DIR.c_str());
			if (!dir)
			{
				std::cerr << "\x1b[31mFailed to open ./flags directory.\x1b[0m\n";
				return flag_info;
			}

			while ((entry = readdir(dir)) != nullptr)
			{
				// Skip ., ..
				if (entry->d_name[0] == '.' && 
					(entry->d_name[1] == '\0' || 
						(entry->d_name[1] == '.' && entry->d_name[2] == '\0')))
						continue;

			        // Remove .txt extension
				string filename = entry->d_name;
				dot_pos = filename.find_last_of('.');

				if (dot_pos == string::npos || filename.substr(dot_pos) != ".txt")
					continue;

				string full_path = FLAG_DIR + "/" + filename;

			        // Read current file
				is.open(full_path, std::ios_base::in);
				if (!is.is_open())
			        continue; // Can be improved...

			    os.str("");
			    os.clear();

			    os << is.rdbuf();
			    is.close();

			    flag_info[std::move(filename.substr(0, dot_pos))] = os.str();
			}
			closedir(dir);
		}

		return flag_info;
	}
}

#endif