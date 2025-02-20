#ifndef __PHASE_SPACE_HPP
#define __PHASE_SPACE_HPP

#include "ColSim/Core.h"

#include <memory>
#include <random>

namespace ColSim {

	// phase space point using tau (s_hat/S), rapidity, and cos theta
	struct PhaseSpace_TauYCosth {
		Double tau, y, cosTheta;
	};

	inline PhaseSpace_TauYCosth generate() {
		return {.tau=0.6, .y=0.7, .cosTheta=0.1};
	}
};


#endif // __PHASE_SPACE_HPP
