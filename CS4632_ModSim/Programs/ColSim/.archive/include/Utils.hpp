/** @file */

#ifndef __UTILS_HPP
#define __UTILS_HPP







// terminal colors 
#define ANSI_COLOR_RED         "\x1b[31m"
#define ANSI_COLOR_GREEN       "\x1b[32m"
#define ANSI_COLOR_YELLOW      "\x1b[33m"
#define ANSI_COLOR_BLUE        "\x1b[34m"
#define ANSI_COLOR_MAGENTA     "\x1b[35m"
#define ANSI_COLOR_CYAN        "\x1b[36m"

#define ANSI_COLOR_RESET       "\x1b[0m"




#include <string>
#include <vector>



namespace Utils {

	/** Internal function that collects variadic arguments into one string.
	 *  The format of the variadic arguments is just like that of `printf`.
	 * 
	 *  @note `std::string` arguments will need to be passed 
	 *  via the `c_str()` method.
	 *
	 *  @param format the C-style format string.
	 *  @param ... arguments to use for the format string.
	 *
	 *  @returns A single `std::string` with all of the collected arguments.
	 */
	std::string CollectLogArgs(const char* format, ...);
	
    
	/** Takes in a full, absolute path, and grabs only the actual file name,
	 *  i.e. the last part of the path.
	 *  The full path can be cumbersome and occupy far too much space in a terminal
	 *  depending on where the project is located -- this is a way to handle that.
	 *
	 *  @param file_path_cstr The full, absolute file path (as a c-string).
	 *
	 *  @returns An `std::string` that contains just the name of the file.
	 */
    std::string FileNameFromAbsPath(const char* file_path_cstr);



	/** Takes in a string and a delimiter,
	 *  splits the string into tokens based on the delimiter.
	 *  The results are placed in the @a results vector.
	 *
	 *  @param[in] str The string to tokenize.
	 *  @param[in] delim The delimiter to split the string by.
	 *  @param[out] results The vector to place the tokens in.
	 */
	void SplitString(const std::string& str, const std::string& delim, std::vector<std::string> results);
	
} // namespace Utils



#endif // __UTILS_HPP
