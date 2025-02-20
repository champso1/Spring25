#ifndef __MONTE_CARLO_HPP
#define __MONTE_CARLO_HPP


#include "ColSim/Core.h"

#include <iostream>
#include <iomanip>
#include <limits>


namespace ColSim {

#define DEFAULT_SEED 12345

	struct IntegrationResult {
		Double result;
		Double error, variance;
		Double maxWeight;
		Double* maxPoints;

		IntegrationResult(const USize num_dims)
			: result(0.0F), error(0.0F), variance(0.0F), maxWeight(0.0F),
			  maxPoints(new Double[num_dims])
		{}
		~IntegrationResult() { delete[] maxPoints; }

		friend std::ostream& operator<<(std::ostream& s,
										const IntegrationResult& self)
		{
			s << std::setprecision(std::numeric_limits<double>::max_digits10)
			  << self.result << " +- " << self.error << "\n";
			return s;
		}
	};


	// follows the same form of the GSL integration routines
	// the first parameter is an array of the independent variables
	// second is a void* for any additional required parameters
	typedef Double (*MonteCarloFunc)(Double*, void*);


	struct MonteCarloParams {
		USize num_evals;
		USize num_dims;
		Double* min;
		Double* max;
		void* extra_params;
		MonteCarloFunc func;
	};



	extern IntegrationResult MonteCarloIntegrate(const MonteCarloParams& params);
}


#endif // __MONTE_CARLO_HPP
