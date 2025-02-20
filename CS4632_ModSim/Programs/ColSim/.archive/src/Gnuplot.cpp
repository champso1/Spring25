#include "Gnuplot.hpp"
#include "Logger.hpp"

#include <sstream>
#include <fstream>

#include <cstdlib>

namespace Utils {
	namespace Global {
		std::string DEFAULT_GNUPLOT_SCRIPT_PATH = "./res/script.dem";
	};

	// =========================
	// Constructors/Destructors
	// =========================

	Gnuplot::Gnuplot() {
		this->commands.emplace_back("set terminal pdfcairo enhanced color notransparent");
		this->commands.emplace_back("set output \"./res/output.pdf\"");
	}




	
	// =========================
	// Public Member Functions
	// =========================

	void Gnuplot::plot() {
		std::ofstream script_file;
		script_file.open(Global::DEFAULT_GNUPLOT_SCRIPT_PATH);
		if (!script_file) {
			GLOG_WARNING("Error opening gnuplot script file \"%s\%",
						 Global::DEFAULT_GNUPLOT_SCRIPT_PATH);
			return;
		}

		for (std::string& com : this->commands) {
			GLOG_DEBUG("\t%s", com.c_str());
			script_file << com << "\n";
		}
		
		script_file.close();

		std::ostringstream console_command_stream;
		console_command_stream << "gnuplot " << Global::DEFAULT_GNUPLOT_SCRIPT_PATH;
		std::string console_command = console_command_stream.str();
	    GLOG_DEBUG("Console command: %s", console_command.c_str());
		system(console_command.c_str());
	}


	void Gnuplot::setTitle(std::string title) {
		std::string command = "set title \"" + title + "\"";
		this->commands.emplace_back(command);
	}

	void Gnuplot::plotFunc(std::string func, float min, float max) {
		std::ostringstream command_stream;
		command_stream << "plot [" << min << ":" << max << "] " << func;
		
		this->addCommand(command_stream.str());
	}

}; // namespace Utils
