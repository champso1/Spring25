#ifndef __LOGGER_HPP
#define __LOGGER_HPP

#include "Utils.hpp"

#include <fstream>

namespace Utils {

	/** Handles dispatching of messages to stderr
	 *  as well as a dedicated file path, if provided.
	 *  Considers different severities of messages:
	 *    + Error, Warning, Info, Debug
	 */
	class Logger {
	private:
		std::string file_path; /*!< Handle for the file path to log. Empty if not logging to a file. */
		std::ofstream file_stream; /*!< Handle for the file stream. Uninitialized if not logging to a file. */
		bool use_file; /*!< Indicator of whether logging is being done to a file. */
		u64 flags; /*!< Bitmask for flags. */

	public:
		/** The following enumeration defines a set of flags that can be passed to the logger.
		 *  This can be done via bitwise OR'ing multiple flags,
		 *  or simply sending a single one.
		 */
		enum Flags : u64 {
			// whether to display colors in the terminal
			// Default: no
			USE_TERMINAL_COLORS = 1 << 0,
			// whether to allow debug messages to be logged
			// Default: no
			USE_DEBUG           = 1 << 1
		};


		
	
		/** Creates a logger that will only log to stderr.
		 */
		Logger() : Logger("", 0) {}

		/** Creates a logger that will log to stderr and the given file.
		 *  If the file does not exist, the logger will log a warning to stderr
		 *  and return a Logger with no set file_path; i.e. it will log to only stderr.
		 *
		 *  @param file_path The file path to use for logging.
		 *  @param flags Any flags that should bypass the default (zero).
		 */
		Logger(const char* file_path, u64 flags=0);

		/** Closes the file stream if one is set.
		 */
		~Logger();


		/** Sets the output file path for logging information
		 *  or changes it, if one was passed earlier via this function or constructor.
		 *  Should an error occur, logging will be done via the previously defined file path
		 *  if it exists, as well as stderr.
		 *
		 *  @param file_path The file path to use for logging.
		 *
		 *  @returns True on success, i.e. the file was found and is writable,
		 *           false if there is some other error.
		 */
		bool setFilePath(const char* file_path);


		/** Setter for flags.
		 *
		 *  @param flags Any combination of flags defined
		 *         in the enumeration @a Flags.  
		 */ 
		void setFlags(u64 flags);

		
		/** Logs serious messages to stderr and possible an output file (if provided).
		 *  If your terminal supports colors, these are shown in red.
		 *  Logging via this function is intended if something occurs that should stop the program execution.
		 *
		 *  @param func Name of the function from which we are logging.
		 *  @param file Name of the file from which we are logging.
		 *  @param line Number of the line from which we are logging.
		 *  @param Message given from the user.
		 */
		void logError(const char* func, const char* file, u32 line, std::string message);

		/** Logs warnings to stderr and possibly an output file (if provided).
		 *  If your terminal supports colors, these are shown in yellow.
		 *  Logging via this function is intended if something unexpected occurs during program execution,
		 *  but it is not critical and program may continue.
		 *
		 *  @param func Name of the function from which we are logging.
		 *  @param file Name of the file from which we are logging.
		 *  @param line Number of the line from which we are logging.
		 *  @param Message given from the user.
		 */
		void logWarning(const char* func, const char* file, u32 line, std::string message);

		/** Logs information to stderr and possibly an output file (if provided).
		 *  These are not given any color.
		 *  Logging via this function is indended for simple informational messages.
		 *  There is no expectation that such information is logged due to averse program execution.
		 *
		 *  @param func Name of the function from which we are logging.
		 *  @param file Name of the file from which we are logging.
		 *  @param line Number of the line from which we are logging.
		 *  @param Message given from the user.
		 */
		void logInfo(const char* func, const char* file, u32 line, std::string message);

		/** Logs debug information to stderr and a separate debug file.
		 *  Only does this if the LOGGER_DEBUG is set, otherwise does nothing.
		 *  Logging via this function is only intended for debugging purposes.
		 *
		 *  @param func Name of the function from which we are logging.
		 *  @param file Name of the file from which we are logging.
		 *  @param line Number of the line from which we are logging.
		 *  @param Message given from the user.
		 */
		void logDebug(const char* func, const char* file, u32 line, std::string message);

		


	private:
		/** Internal function to test if a flag is set.
		 *
		 *  @param flag The flag (from the enumeration) to test.
		 *
		 *  @returns True if the flag is set, false if not.
		 */
		bool isFlagSet(u64 flag) const;
		
		/** Internal function that actually logs the message to the files.
		 *
		 *  @param func The name of the function from which the log was called.
		 *  @param file The name of the file from which the log was called.
		 *  @param line The number of the line on which the log was called.
		 *  @param color The colors to use for logging to the terminal.
		 *  @param prefix Indicates the type of log, i.e. "ERROR", "WARNING", "INFO", or "DEBUG."
		 *  @param message The actual message given by the user.
		 */
		void logMessage(const char* func, const char* file, u32 line,
						  const char* color, const char* prefix,
						  std::string message);


		
	};



	
	namespace Global {
		/** Global logger to be used by the rest of the program.
		 *  Declared in `Globals.cc`.
		 */
		extern Logger GLOBAL_LOGGER;
	}

};



// some defines that make logging nicer to look operate
#define LOG_ERROR(...)   logError(__FUNCTION__, __FILE__, __LINE__, Utils::CollectLogArgs(__VA_ARGS__, NULL))
#define LOG_WARNING(...) logWarning(__FUNCTION__, __FILE__, __LINE__, Utils::CollectLogArgs(__VA_ARGS__, NULL))
#define LOG_INFO(...)    logInfo(__FUNCTION__, __FILE__, __LINE__, Utils::CollectLogArgs(__VA_ARGS__, NULL))
#define LOG_DEBUG(...)   logDebug(__FUNCTION__, __FILE__, __LINE__, Utils::CollectLogArgs(__VA_ARGS__, NULL))

// logging via the global logger
#define GLOG_ERROR(...) Utils::Global::GLOBAL_LOGGER.LOG_ERROR(__VA_ARGS__)
#define GLOG_WARNING(...) Utils::Global::GLOBAL_LOGGER.LOG_WARNING(__VA_ARGS__)
#define GLOG_INFO(...) Utils::Global::GLOBAL_LOGGER.LOG_INFO(__VA_ARGS__)
#define GLOG_DEBUG(...) Utils::Global::GLOBAL_LOGGER.LOG_DEBUG(__VA_ARGS__)



#endif // __LOGGER_HPP
