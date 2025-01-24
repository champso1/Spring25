#include <iostream>

#include "Utils.hpp"
using namespace Utils;

int main() {

	Logger logger("test.log", Logger::USE_TERMINAL_COLORS | Logger::USE_DEBUG);

	logger.LOG_ERROR("this is an error");
	logger.LOG_WARNING("this is a warning");
	logger.LOG_INFO("this is a info");
	logger.LOG_DEBUG("this is a debug!!");


	ConfigFile configFile;
	configFile.printConfig();

	
	return 0;
}
