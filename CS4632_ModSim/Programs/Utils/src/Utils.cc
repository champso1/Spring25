#include "Utils.hpp"

/** This is where the rest of the miscellaneous utility functions 
 *  not delegated to a class will find themselves.
 */




namespace Utils {

	std::string FileNameFromAbsPath(const char* file_path_cstr) {
		std::string file_path(file_path_cstr);
		// may need to change this if on windows
		std::size_t last_slash_pos = file_path.find_last_of("/");
		return file_path.substr(last_slash_pos + 1);
	}

}
