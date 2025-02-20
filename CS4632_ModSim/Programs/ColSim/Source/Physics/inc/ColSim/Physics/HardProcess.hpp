#ifndef __HARD_PROCESS_HPP
#define __HARD_PROCESS_HPP

#include "ColSim/Core.h"
#include "ColSim/Physics/PhaseSpace.hpp"

#include <memory>

namespace ColSim {

	struct XSResult {
		Double result, error;
	};

	class HardProcess {
    protected:
		HardProcess() {}
		
		virtual Double Sigma() { return -999.0; }       // returns the full cross section
	    virtual XSResult Integrate() { return {.result=-999.0, .error=-0.999}; }
	};

	// p + p -> l + l
	class PP2Zg2ll : public HardProcess {
    public:
		PP2Zg2ll() {}
		
		Double Kappa() const;
		Double Chi1(const Double s_hat) const;
		Double Chi2(const Double s_hat) const;
		Double A0(const UInt8 quarkType, const Double s_hat) const;
		Double A1(const UInt8 quarkType, const Double s_hat) const;

		Double dSigmaHat(const Double cosTheta, const UInt8 quarkType, const Double s_hat);
		Double computeWeight(const Double cosTheta, const Double x1, const Double x2, const Double s_hat);
		
		Double Sigma() override;
		XSResult Integrate() override;
	};
};



#endif // __HARD_PROCESS_HPP
