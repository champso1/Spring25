#ifndef __SETTINGS_HPP
#define __SETTINGS_HPP

#include "ColSim/Core/Types.hpp"

#include "LHAPDF/LHAPDF.h"

#include <unordered_map>
#include <fstream>

namespace ColSim {

	struct Settings {
		Double ECM, S;
		std::string PDFName;
		UInt32 PDFMemberNo;
		std::string Process;
		LHAPDF::PDF* pdf;
	    
		Settings() : PDFName(""), Process(""), pdf(nullptr) {}
		~Settings() { delete pdf; }
		void loadConfigFile(const std::string& fileStream);

		static Settings& getInstance() {
			static Settings settings;
			return settings;
		}
	};

#define SETTINGS Settings::getInstance()
	
}; // namespace ColSim



#endif // __SETTINGS_HPP
