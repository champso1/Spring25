#include "Logger.hpp"


#include <cstdio>
#include <cmath>
#include <cstring>

#include <iostream>
#include <sstream>


namespace Utils {
	namespace Global {
		std::string GLOBAL_LOGGER_FILE_PATH = "./res/global/log";
		Logger GLOBAL_LOGGER(GLOBAL_LOGGER_FILE_PATH.c_str(), Logger::USE_DEBUG);
	};
	
	// ============================
	//   Constructors/Destructors
	// ============================

	Logger::Logger(const char* file_path, u64 flags) {
		this->file_stream.open(file_path);

		// if the open failed, tell logger to not use a file to output
		if (!this->file_stream) {
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



	// ============================
	//   Public Member Functions
	// ============================

	


	bool Logger::setFilePath(const char* file_path) {
		this->LOG_INFO("Changing file path from \"%s\" to \"%s\"", this->file_path.c_str(), file_path);
		
	    // test that it is a valid file first
		this->file_stream.open(file_path);
		if (!file_stream) {
			if (this->use_file) {
				// if we are currently using a good file,
				// go back to that one
				this->LOG_WARNING("Error opening file \"%s\". Keeping old file...", file_path);
				this->file_stream.close();
				this->file_stream.open(this->file_path);
			} else {
				// otherwise, just do nothing
				this->LOG_WARNING("Error opening file \"%s\". Continuing to use no file...", file_path);
			}
			return false; // epic fail!
		}

		// if we were successful, also set the file path string
		this->file_path = std::string(file_path);

		return true; // success
	}


	void Logger::setFlags(u64 flags) {
		// no need to check anything
		this->flags = flags;
	}


	void Logger::logError(const char* func, const char* file, u32 line, std::string message) {

		this->logMessage(func, file, line, ANSI_COLOR_RED, "ERROR", message);
	}


	void Logger::logWarning(const char* func, const char* file, u32 line, std::string message) {
	
		this->logMessage(func, file, line, ANSI_COLOR_YELLOW, "WARNING", message);
	}


	void Logger::logInfo(const char* func, const char* file, u32 line, std::string message) {
	
		// just pass in the "reset" color since we want no colors for info messages
		this->logMessage(func, file, line, ANSI_COLOR_RESET, "INFO", message);
	}



	void Logger::logDebug(const char* func, const char* file, u32 line, std::string message) {
		// only log if the debug flag is set
		if (!isFlagSet(Logger::USE_DEBUG)) return;
		this->logMessage(func, file, line, ANSI_COLOR_CYAN, "DEBUG", message);
	}



	// ============================
	//   Private Member Functions
	// ============================



	
	bool Logger::isFlagSet(u64 flag) const {
		if (((this->flags & flag) >> static_cast<u64>(log2(flag))) == 1) return true;
		return false;
	}


	void Logger::logMessage(
							  const char* func, const char* file, u32 line,
							  const char* color, const char* prefix,
							  std::string message)
	{
		std::string file_name = FileNameFromAbsPath(file);

		if (this->isFlagSet(Logger::USE_TERMINAL_COLORS)) {
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


	

	


}
