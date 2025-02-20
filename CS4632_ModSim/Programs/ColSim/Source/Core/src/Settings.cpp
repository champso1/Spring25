#include "ColSim/Core/Settings.hpp"
#include "ColSim/Core/Logger.hpp"
#include "ColSim/Core/Utils.hpp"

#include <cmath>
#include <vector>
#include <fstream>
#include <unordered_map>


namespace ColSim {

	void Settings::loadConfigFile(const std::string& configFilePath) {
		std::ifstream configFileStream;
		configFileStream.open(configFilePath);
		if (!configFileStream) 
			throw std::runtime_error("Could not open config file " + configFilePath);

		std::unordered_map<std::string, std::string> items;
		std::string line;
		std::vector<std::string> tokens;
		while (std::getline(configFileStream, line)) {
			// skip empty lines and comments
			if ((line.size() <= 1) || (line[0] == '#'))
				continue;
			
			SplitString(line, "=", tokens);

			items.insert({tokens[0], tokens[1]});
			
			tokens.clear();
		}
		configFileStream.close();

		ECM = std::stod(items.at("ECM"));
		LOGGER.logMessage("Using ECM=%lf", ECM);

		PDFName = items.at("PDFName");
		LOGGER.logMessage("Using PDF=%s", PDFName.c_str());

		PDFMemberNo = std::stoi(items.at("PDFMemberNo"));
		LOGGER.logMessage("\t Member ID=%d", PDFMemberNo);

		Process = items.at("Process");
		LOGGER.logMessage("Process string=%s", Process.c_str());
	    

	    LOGGER.logMessage("Finished loading configuration file data.");

		pdf = LHAPDF::mkPDF(PDFName, PDFMemberNo);
	}
}; // namespace ColSim
