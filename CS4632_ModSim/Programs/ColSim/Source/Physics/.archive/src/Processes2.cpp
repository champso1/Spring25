#include "ColSim/Physics/Processes.hpp"
#include "ColSim/Physics/Constants.hpp"

#include "LHAPDF/LHAPDF.h"

#include <cmath>

namespace ColSim {


	// ==============================
	// The following are functions related
	// to constants in the electroweak Lagrangian
	// ==============================
	
	static constexpr Double kappa() {
		return sqrt(2.0F) * FERMI_CONSTANT * Z_MASS_2 / (4.0F * PI * ALPHA);
	}

	// chi1/chi2 depend on the partonic center-of-mass energy
	static Double chi1(const Double s_hat) {
	    return kappa() * s_hat * (s_hat - Z_MASS_2) / (pow(s_hat - Z_MASS, 2) + Z_WIDTH_2 * Z_MASS_2);
	}

	static Double chi2(const Double s_hat) {
		return pow(kappa(), 2) * pow(s_hat, 2) / (pow(s_hat - Z_MASS_2, 2) + Z_WIDTH_2 * Z_MASS_2);
	}

	// A0/A1 are more directly related to the couplings
	// between the partons and Z bosons
	// so it also depends on the quark type (up/down)
	// massive leptons (not neutrinos) are the same
	// so this is only for the quark type (we aren't producing neutrinos
	static Double A0(const UInt8 qtype, const Double s_hat) {
		Double CAe = -0.5F, CVe = -0.5F + 2 * WEINBERG_ANGLE;
		Double CVf, CAf, Qf;
		if (qtype == 0) { // up-type
			CVf = 0.5F - (4.0F / 3.0F) * WEINBERG_ANGLE;
			CAf = 0.5F;
			Qf = 2.0F / 3.0F;
		} else { // down-type
			CVf = -0.5F + (2.0F / 3.0F) * WEINBERG_ANGLE;
			CAf = -0.5F;
			Qf = -1.0F / 3.0F;
		}

	    return Qf*Qf - 2.0F * Qf * CVe * CVf * chi1(s_hat) + (CAe*CAe + CVe*CVe) * (CAf*CAf + CVf*CVf) * chi2(s_hat);
	}

	static Double A1(const UInt8 qtype, const Double s_hat) {
		Double A_mu = -0.5F, V_mu = -0.5F + 2 * WEINBERG_ANGLE;
		Double V_quark, A_quark, Q_quark;
		if (qtype == 0) { // up-type
			V_quark = 0.5F - (4.0F / 3.0F) * WEINBERG_ANGLE;
			A_quark = 0.5F;
			Q_quark = 2.0F / 3.0F;
		} else {
			V_quark = -0.5F + (2.0F / 3.0F) * WEINBERG_ANGLE;
			A_quark = -0.5F;
			Q_quark = -1.0F / 3.0F;
		}

	    return -4.0F * Q_quark * A_mu * A_quark * chi1(s_hat) + 8.0F * A_mu * V_mu * A_quark * V_quark * chi2(s_hat);
	}
	
	
	static Double dSigmaHat(const Double cosTheta, const UInt8 qtype, const Double s_hat) {
		// this extra 3.0 is for the initial color averaging
		// --------------------------------------\|/
		return (2 * PI * pow(ALPHA, 2) / (4.0F * 3.0 * s_hat)) * (A0(qtype, s_hat) * (1 + pow(cosTheta, 2)) + A1(qtype, s_hat) * cosTheta);

	}

	static Double computeWeight(const Double s_hat, const Double x1, const Double x2,
						 const Double cosTheta, const LHAPDF::PDF* pdf) {
		double weight = 0.0F;
		// up-type quarks
		weight += dSigmaHat(cosTheta , 0, s_hat) * ((pdf->xfxQ2(2 , x1, s_hat) * pdf->xfxQ2(-2, x2, s_hat)) + (pdf->xfxQ2(4 , x1, s_hat) * pdf->xfxQ2(-4, x2, s_hat)));	
		weight += dSigmaHat(-cosTheta, 0, s_hat) * ((pdf->xfxQ2(-2, x1, s_hat) * pdf->xfxQ2(2 , x2, s_hat)) + (pdf->xfxQ2(-4, x1, s_hat) * pdf->xfxQ2(4 , x2, s_hat)));
		// down-type quarks
		weight += dSigmaHat(cosTheta , 1, s_hat) * ((pdf->xfxQ2(1 , x1, s_hat) * pdf->xfxQ2(-1, x2, s_hat)) + (pdf->xfxQ2(3 , x1, s_hat) * pdf->xfxQ2(-3, x2, s_hat)));
		weight += dSigmaHat(-cosTheta, 1, s_hat) * ((pdf->xfxQ2(-1, x1, s_hat) * pdf->xfxQ2(1 , x2, s_hat)) + (pdf->xfxQ2(-3, x1, s_hat) * pdf->xfxQ2(3 , x2, s_hat)));

		return weight;
	}
	
	
	Double PPtoDilepton(Double x[3], void* args) {
	    LHAPDF::PDF* pdf = reinterpret_cast<LHAPDF::PDF*>(args);
		// independent variables
		const Double cosTheta = x[0];
		const Double rho = x[1];
		const double rand_y = x[2];

		// other variables
	    const Double jacobian = (MASS_TR * WIDTH_TR)/ (pow(cos(rho), 2) * BIG_S);

		const Double s_hat = MASS_TR * WIDTH_TR * tan(rho) + pow(MASS_TR, 2);

		const Double yMax = -0.5F * log(s_hat / BIG_S);
		const Double deltaY = 2.0F * yMax;
		const Double y =  (2.0F * rand_y - 1.0F) * yMax;

		const Double x1 = sqrt(s_hat / BIG_S) * exp(y);
		const Double x2 = sqrt(s_hat / BIG_S) * exp(-y);

		Double weight = computeWeight(s_hat, x1, x2, cosTheta, pdf);
		// go ahead and scale it this way here
		weight *= (jacobian * deltaY);
		weight /= (x1 * x2);
		
		return weight;
	}
};
