#ifndef __UTILS_H
#define __UTILS_H


#include <string>

namespace Utils {

	// I don't like the entire file path
	std::string FileNameFromAbsPath(const char* file_path_cstr) {
		std::string file_path(file_path_cstr);
		// may need to change this if on windows
		std::size_t last_slash_pos = file_path.find_last_of("/");
		return file_path.substr(last_slash_pos + 1);
	}







	

}
	







#endif // __UTILS_H
