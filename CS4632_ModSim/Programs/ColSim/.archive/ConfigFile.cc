#include "ConfigFile.hpp"
#include "Logger.hpp"

#include <fstream>
#include <iostream>

#include <algorithm>
#include <cctype>
#include <locale>


namespace Utils {
	namespace Global {
		std::string DEFAULT_CONFIG_FILE_PATH = "./res/config.in";
	};

	// =========================
	// Constructors/Destructors
	// =========================
   
	ConfigFile::ConfigFile(std::string file_path) {
		std::ifstream file_stream;
		file_stream.open(file_path);

		if ((file_stream.rdstate() & std::ofstream::failbit) != 0) {
			GLOG_ERROR("Config file (%s) not found!", file_path.c_str());
			return;
		}

		std::string line;
		while (std::getline(file_stream, line)) {
			// skip empty or comment lines
			if ((line.size() < 1) || (line[0] == '#')) continue;

			std::size_t equals_pos = line.find('=');
		
			std::string key = line.substr(0, equals_pos);		
			std::string value = line.substr(equals_pos+1, line.size() - equals_pos);

		    GLOG_INFO("\n\tKey: %s\n\tValue: '%s'\n", key.c_str(), value.c_str());		

			values[key] = std::stof(value);
		}
		file_stream.close();

	}



	// =========================
	// Public Member Functions
	// =========================



	void ConfigFile::setVal(std::string val_name, float val) {
		// don't care if it exists or not
		this->values[val_name] = val;
	}


	float ConfigFile::getVal(std::string val_name) {
		// make sure it exists first
	    if (this->values.count(val_name) > 0) return this->values[val_name];
		// return 0 on error i guess
		return 0.0f;
	}

	void ConfigFile::printConfig() {
		for (auto it = this->values.begin(); it != this->values.end(); it++) {
			std::cout << it->first << " -> " << it->second << "\n";
		}
		std::cout << std::endl;
	}


}; // namespace Utils
