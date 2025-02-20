#ifndef __CONFIG_FILE_HPP
#define __CONFIG_FILE_HPP


#include "Utils.hpp"

#include <unordered_map>


namespace Utils {
	namespace Global {
		extern std::string DEFAULT_CONFIG_FILE_PATH; /*!< Default name for the config file if none is passed. */
	}

	/** Stores information given in a configuration file.
	 *  This information is then distributed wherever it needs to
	 *  during program execution.
	 */
	class ConfigFile {
	
	private:		
		std::string file_path; /*!< File path of the config file. */
		std::unordered_map<std::string, float> values; /*/< The values from the config file stored in a map. */
	
	public:

		/** Loads the configuration file from the default directory
		 *  given by @a CFG_DEFAULT_FILE_PATH.
		 */ 
		ConfigFile() : ConfigFile(Global::DEFAULT_CONFIG_FILE_PATH) {}

		/** Loads the configuration file from the given file path.
		 *
		 *  @param file_path The path to the configuration file.
		 */ 
		ConfigFile(std::string file_path);



		

		/** Sets a value after loading the config file.
		 *
		 *  @param val_name Name of the value to set.
		 *  @param val Value to use.
		 */
		void setVal(std::string val_name, float val);

		/** Grabs the corresponding entry from the config file data.
		 *  Returns 0.0f if the value doesn't exist.
		 *
		 *  @param val_name Name of value to retrieve.
		 *
		 *  @returns the numeric entry for @a val_name
		 *
		 *  @todo Do error handling for this a little better;
		 *        it's possible a data entry could be zero.
		 */
		float getVal(std::string val_name);

		/** Prints all of the values loaded from the config.
		 */
		void printConfig();
	
	};






};





#endif // __CONFIG_FILE_HPP
