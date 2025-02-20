#include "Utils.hpp"

/** This is where the rest of the miscellaneous utility functions 
 *  not delegated to a class will find themselves.
 */


#include <cstdarg>
#include <cstring>


namespace Utils {

	std::string CollectLogArgs(const char* format, ...) {
		va_list args;
	
		va_start(args, format);

		// surely an argument shouldn't be longer than 256 bytes...
		const u32 buf_size = 256;
		char logBuffer[buf_size];
		memset(logBuffer, 0, buf_size);
		vsnprintf(logBuffer, buf_size, format, args);
			
		va_end(args);

		std::string output_str(logBuffer);
			
		return output_str;
	}

	std::string FileNameFromAbsPath(const char* file_path_cstr) {
		std::string file_path(file_path_cstr);
		// may need to change this if on windows
		std::size_t last_slash_pos = file_path.find_last_of("/");
		return file_path.substr(last_slash_pos + 1);
	}

	void SplitString(const std::string& _str, const std::string& delim, std::vector<std::string> results) {
		std::string str = _str; // make a copy to not modify original string
		
		std::size_t pos = 0;
		std::string token;
		while ((pos = str.find(delim)) != std::string::npos) {
			token = str.substr(0, pos);
			str.erase(0, pos + delim.length());
		    results.emplace_back(token);
		}
		// the loop leaves the last token in `str`,
		// since no more delimiters would exist at that point
		// so we emplace_back just the string itself
		results.emplace_back(str);
	}

}
