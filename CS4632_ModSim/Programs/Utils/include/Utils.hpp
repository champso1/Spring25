/** @file */

#ifndef __UTILS_HPP
#define __UTILS_HPP



// makes my life easier to define types like this
#include <cstdint>

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;



// terminal colors 
#define ANSI_COLOR_RED         "\x1b[31m"
#define ANSI_COLOR_GREEN       "\x1b[32m"
#define ANSI_COLOR_YELLOW      "\x1b[33m"
#define ANSI_COLOR_BLUE        "\x1b[34m"
#define ANSI_COLOR_MAGENTA     "\x1b[35m"
#define ANSI_COLOR_CYAN        "\x1b[36m"

#define ANSI_COLOR_RESET       "\x1b[0m"




#include <string>
#include <unordered_map>
#include <fstream>




namespace Utils {
    
	/** Takes in a full, absolute path, and grabs only the actual file name,
	 *  i.e. the last part of the path.
	 *  The full path can be cumbersome and occupy far too much space in a terminal
	 *  depending on where the project is located -- this is a way to handle that.
	 */
    std::string FileNameFromAbsPath(const char* file_path_cstr);


#define CFG_DEFAULT_FILE_PATH "./res/config.in"



	/** Stores information given in the configuration file.
	 *  This information is then distributed wherever it needs to
	 *  during program execution.
	 */
	class ConfigFile {
	
	private:
		std::string file_path;

		std::unordered_map<std::string, float> values;
	
	public:
		ConfigFile() : ConfigFile(CFG_DEFAULT_FILE_PATH) {}
		ConfigFile(std::string file_path);


		/** Sets a value after loading the config file.
		 */
		void setVal(std::string val_name, float val);

		/** Grabs the corresponding entry from the config file data.
		 *  Returns 0.0f if the value doesn't exist.
		 *
		 *  @TODO: Do error handling for this a little better;
		 *         It's possible a data entry could be zero.
		 */
		float getVal(std::string val_name);

		/** Prints all of the values loaded from the config.
		 */
		void printConfig();
	
	};

	


	/** Handles dispatching of messages to stderr
	 *  as well as a dedicated file path, if provided.
	 *  Considers different severities of messages:
	 *    + Error, Warning, Info, Debug
	 */
	class Logger {
	private:
		// handles for the file path and the actual file stream
		std::string file_path;
		std::ofstream file_stream;
	
		bool use_file;

		u64 flags;

	
		/** Internal function to test if a flag is set.
		 */
		bool __isFlagSet(u64 flag);

	
		/** Internal function that actually logs the message to the files.
		 */
		void __logMessage(const char* func, const char* file, u32 line,
						  const char* color, const char* prefix,
						  std::string message);

	public:
	
		/** Creates a logger that will only log to stderr.
		 */
		Logger() : Logger("", 0) {}

		/** Creates a logger that will log to stderr and the given file.
		 *  If the file does not exist, the logger will log a warning to stderr
		 *  and return a Logger with no set file_path; i.e. it will log to only stderr.
		 */
		Logger(const char* file_path, u64 flags=0);

		/** Closes the file stream if one is set.
		 */
		~Logger();


		/** Sets the output file path for logging information
		 *  or changes it, if one was passed.
		 *  Returns true on success, i.e. the file was found and is writable,
		 *  false if there is some other error.
		 *  Should an error occur, logging will be done via the previously defined file path
		 *  if it exists, as well as stderr.
		 */
		bool setFilePath(const char* file_path);



		/** The following enumeration defines a set of flags that can be passed to the logger.
		 *  This can be done via bitwise OR'ing multiple flags,
		 *  or just sending the one.
		 */
		enum Flags : u64 {
			// whether to display colors in the terminal
			// Default: no
			USE_TERMINAL_COLORS = 1 << 0,
			// whether to allow debug messages to be logged
			// Default: no
			USE_DEBUG           = 1 << 1
		};


		void setFlags(u64 flags) {this->flags = flags; };


	

		// logging functions

		/** Logs serious messages to stderr and possible an output file (if provided).
		 *  If your terminal supports colors, these are shown in red.
		 *  Logging via this function is intended if something occurs that should stop the program execution.
		 */
		void logError(const char* func, const char* file, u32 line, std::string message);

		/** Logs warnings to stderr and possibly an output file (if provided).
		 *  If your terminal supports colors, these are shown in yellow.
		 *  Logging via this function is intended if something unexpected occurs during program execution,
		 *  but it is not critical and program may continue.
		 */
		void logWarning(const char* func, const char* file, u32 line, std::string message);

		/** Logs information to stderr and possibly an output file (if provided).
		 *  These are not given any color.
		 *  Logging via this function is indended for simple informational messages.
		 *  There is no expectation that such information is logged due to averse program execution.
		 */
		void logInfo(const char* func, const char* file, u32 line, std::string message);

		/** Logs debug information to stderr and a separate debug file.
		 *  Only does this if the LOGGER_DEBUG is set;
		 *  otherwise, does nothing.
		 *  Logging via this function is only intended for debugging purposes.
		 */
		void logDebug(const char* func, const char* file, u32 line, std::string message);
	};

	namespace INTERNAL {

		/** Internal function that collects the variadic arguments
		 *  passed to the logging functions.
		 *  The format of the variadic arguments is just like that of 
		 *  `printf`.
		 *  Note that std::string arguments will need to be passed 
		 *  via the `c_str()` method.
		 */
		std::string __collectLogArgs(const char* format, ...);



		/** File path for the global logger to print out to.
		 */ 
		#define GLOBAL_LOGGER_FILE_PATH "./log.out"


		/** Global logger to be used by the rest of the program
		 *  Declared in `Logger.cc`
		 */
		extern Logger GLOBAL_LOGGER;
	}

// some defines that make logging nicer to look operate
// the final NULL is to provide a definitive ending point to the arguments
#define LOG_ERROR(...)   logError(__FUNCTION__, __FILE__, __LINE__, INTERNAL::__collectLogArgs(__VA_ARGS__, NULL))
#define LOG_WARNING(...) logWarning(__FUNCTION__, __FILE__, __LINE__, INTERNAL::__collectLogArgs(__VA_ARGS__, NULL))
#define LOG_INFO(...)    logInfo(__FUNCTION__, __FILE__, __LINE__, INTERNAL::__collectLogArgs(__VA_ARGS__, NULL))
#define LOG_DEBUG(...)   logDebug(__FUNCTION__, __FILE__, __LINE__, INTERNAL::__collectLogArgs(__VA_ARGS__, NULL))





	
} // namespace Utils


#endif // __UTILS_HPP
