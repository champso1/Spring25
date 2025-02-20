#ifndef __GNUPLOT_HPP
#define __GNUPLOT_HPP

#include "Utils.hpp"

#include <vector>

namespace Utils {
	namespace Global {
		extern std::string DEFAULT_GNUPLOT_SCRIPT_PATH; /*!< Default path for name of compiled script. */
	}

	
	/** Provides an interface for plotting things using Gnuplot
	 */ 
	class Gnuplot {
		
	private:
		std::vector<std::string> commands; /*!< List of string commands before compilation into script. */


	public:
		/** Default constructor.
		 *  Only thing it does it add a few commands to allow saving plots as PDF's.
		 */ 
		Gnuplot();

		/** Default destructor. Does nothing.
		 */ 
		~Gnuplot() {}

		/** Sets the title of the plot.
		 *  Can also be done via the constructor.
		 *
		 *  @param title Title to set for the plot.
		 */ 
		void setTitle(std::string title);

		/** Takes in a function as a string to plot.
		 *  Plots in the range [min, max].
		 *
		 *  @param func String representation of the function to call; e.g. "sin(x)"
		 *  @param min Min value to plot.
		 *  @param max Max value to plot.
		 */ 
		void plotFunc(std::string func, float min, float max);


		/** Saves all commands to a file and passes it to Gnuplot.
		 */ 
		void plot();



	private:
		/** Internal function to add a command to the list of commands.
		 *  Done this way since many commands require some "dressing up"
		 *  that the user shouldn't have to worry about.
		 *
		 *  @param command Gnuplot command to call.
		 */ 
		void addCommand(std::string command);
	};


	
	
	
}; // namespace Utils




#endif // __GNUPLOT_HPP
