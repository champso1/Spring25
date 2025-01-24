#include "Utils.hpp"


#include <cstdio>
#include <cmath>
#include <cstdarg>
#include <cstring>

#include <iostream>
#include <sstream>



namespace Utils {



	namespace INTERNAL {

		// instantiate the global logger
		Logger GLOBAL_LOGGER(GLOBAL_LOGGER_FILE_PATH);

	}
	


	Logger::Logger(const char* file_path, u64 flags) {
		this->file_stream.open(file_path);

		// if the open failed, tell logger to not use a file to output
		if ((file_stream.rdstate() & std::ofstream::failbit) != 0) {
			this->use_file = false;
			return;
		}

		// otherwise, all is good:

		this->file_path = file_path;
		this->use_file = true;
		this->flags = flags;
	}


	Logger::~Logger() {
		std::cerr << std::endl;
		if (this->use_file) this->file_stream.close();
	}




	bool Logger::__isFlagSet(u64 flag) {
		if (((this->flags & flag) >> static_cast<u64>(log2(flag))) == 1) return true;
		return false;
	}

	namespace INTERNAL {

		std::string __collectLogArgs(const char* format, ...) {
			va_list args;
	
			va_start(args, format);

			const u32 buf_size = 256;
			char logBuffer[buf_size];
			memset(logBuffer, 0, buf_size);
			vsnprintf(logBuffer, buf_size, format, args);
			
			va_end(args);

			std::string output_str(logBuffer);
			
			return output_str;
		}

	}


	void Logger::__logMessage(
							  const char* func, const char* file, u32 line,
							  const char* color, const char* prefix,
							  std::string message)
	{
		std::string file_name = FileNameFromAbsPath(file);

		if (this->__isFlagSet(Logger::USE_TERMINAL_COLORS)) {
			std::cerr <<
				color <<
				"[" << prefix <<"] (" <<
				file_name << ":" << line << " in " << func << ") " <<
				"\t" << message << "\n" <<
				ANSI_COLOR_RESET;
		}
		else 
			std::cerr <<
				"[" << prefix << "] (" <<
				file_name << ":" << line << " in " << func << ") " <<
				"\t" << message << "\n";
	
	
		if (this->use_file) {
			this->file_stream <<
				"[" << prefix << "] (" <<
				file_name << ":" << line << " in " << func << ") " <<
				"\t" << message << "\n";
		}
	}





	void Logger::logError(const char* func, const char* file, u32 line, std::string message) {

		this->__logMessage(func, file, line, ANSI_COLOR_RED, "ERROR", message);
	}


	void Logger::logWarning(const char* func, const char* file, u32 line, std::string message) {
	
		this->__logMessage(func, file, line, ANSI_COLOR_YELLOW, "WARNING", message);
	}


	void Logger::logInfo(const char* func, const char* file, u32 line, std::string message) {
	
		// just pass in the "reset" color since we want no colors for info messages
		this->__logMessage(func, file, line, ANSI_COLOR_RESET, "INFO", message);
	}



	void Logger::logDebug(const char* func, const char* file, u32 line, std::string message) {
		if (!__isFlagSet(Logger::USE_DEBUG)) return;
		this->__logMessage(func, file, line, ANSI_COLOR_CYAN, "DEBUG", message);
	}


}
