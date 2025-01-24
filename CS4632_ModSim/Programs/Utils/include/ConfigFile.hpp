#ifndef __CONFIG_FILE_HPP
#define __CONFIG_FILE_HPP

#include <string>
#include <unordered_map>


#define CFG_DEFAULT_FILE_PATH "./res/config.in"


namespace Utils {

	

	class ConfigFile {
	
	private:
		std::string file_path;

		std::unordered_map<std::string, float> values;
	

	public:
		ConfigFile() : ConfigFile(CFG_DEFAULT_FILE_PATH) {}
		ConfigFile(std::string file_path);


		// setters/getters
		void setVal(std::string val_name, float val);
		float getVal(std::string val_name);
	
	};



}


#endif // __CONFIG_FILE_HPP
