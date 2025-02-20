#include "ColSim/Physics/HardProcess.hpp"
#include "ColSim/Physics/Constants.hpp"

#include "LHAPDF/LHAPDF.h"

#include <cmath>

namespace ColSim {

	Double PP2Zg2ll::Kappa() const {
		return std::sqrt(2.0)*FERMI_CONSTANT*Z_MASS_2 / (4.0*PI*ALPHA);
	}

	Double PP2Zg2ll::Chi1(const double s_hat) const {
		return Kappa()*s_hat*(s_hat-Z_MASS_2) / (std::pow(s_hat-Z_MASS,2) + Z_WIDTH_2*Z_MASS_2);
	}
	
	Double PP2Zg2ll::Chi2(const Double s_hat) const {
		return std::pow(Kappa(),2) * pow(s_hat,2) / (std::pow(s_hat-Z_MASS_2,2) + Z_WIDTH_2*Z_MASS_2);
	}
	
	Double PP2Zg2ll::A0(const UInt8 quarkType, const Double s_hat) const {
		Double CAe = -0.5, CVe = -0.5 + 2*WEINBERG_ANGLE;
		Double CVf, CAf, Qf;
		if (quarkType == 0) { // up-type
			CVf = 0.5 - (4.0/3.0)*WEINBERG_ANGLE;
			CAf = 0.5;
			Qf = 2.0/3.0;
		} else { // down-type
			CVf = -0.5 + (2.0/3.0)*WEINBERG_ANGLE;
			CAf = -0.5;
			Qf = -1.0/3.0;
		}

	    return Qf*Qf - 2.0*Qf*CVe*CVf*Chi1(s_hat) + (CAe*CAe + CVe*CVe)*(CAf*CAf + CVf*CVf) * Chi2(s_hat);
	}
	
	Double PP2Zg2ll::A1(const UInt8 quarkType, const Double s_hat) const {
		Double A_mu = -0.5, V_mu = -0.5 + 2*WEINBERG_ANGLE;
		Double V_quark, A_quark, Q_quark;
		if (quarkType == 0) { // up-type
			V_quark = 0.5 -(4.0/3.0)*WEINBERG_ANGLE;
			A_quark = 0.5;
			Q_quark = 2.0/3.0;
		} else {
			V_quark = -0.5 + (2.0/3.0)*WEINBERG_ANGLE;
			A_quark = -0.5;
			Q_quark = -1.0/3.0;
		}

	    return -4.0*Q_quark*A_mu*A_quark*Chi1(s_hat) + 8.0*A_mu*V_mu*A_quark*V_quark*Chi2(s_hat);
	}


	Double PP2Zg2ll::dSigmaHat(const Double cosTheta, const UInt8 quarkType, const Double s_hat) {
		return (2.0*PI*std::pow(ALPHA,2) / (4.0*3.0*s_hat)) * (A0(quarkType,s_hat)*(1.0 + std::pow(cosTheta,2)) + A1(quarkType,s_hat)*cosTheta);
	}

	Double PP2Zg2ll::computeWeight(const Double s_hat, const Double x1, const Double x2, const Double cosTheta) {
		Double weight = 0.0F;
		LHAPDF::PDF* pdf = Settings::getInstance().pdf;
		// up-type quarks
		weight += dSigmaHat(cosTheta , 0, s_hat) * ((pdf->xfxQ2(2 , x1, s_hat) * pdf->xfxQ2(-2, x2, s_hat)) + (pdf->xfxQ2(4 , x1, s_hat) * pdf->xfxQ2(-4, x2, s_hat)));	
		weight += dSigmaHat(-cosTheta, 0, s_hat) * ((pdf->xfxQ2(-2, x1, s_hat) * pdf->xfxQ2(2 , x2, s_hat)) + (pdf->xfxQ2(-4, x1, s_hat) * pdf->xfxQ2(4 , x2, s_hat)));
		// down-type quarks
		weight += dSigmaHat(cosTheta , 1, s_hat) * ((pdf->xfxQ2(1 , x1, s_hat) * pdf->xfxQ2(-1, x2, s_hat)) + (pdf->xfxQ2(3 , x1, s_hat) * pdf->xfxQ2(-3, x2, s_hat)));
		weight += dSigmaHat(-cosTheta, 1, s_hat) * ((pdf->xfxQ2(-1, x1, s_hat) * pdf->xfxQ2(1 , x2, s_hat)) + (pdf->xfxQ2(-3, x1, s_hat) * pdf->xfxQ2(3 , x2, s_hat)));

		return weight;
	}

	Double PP2Zg2ll::Sigma() {
		LHAPDF::PDF* pdf = Settings::getInstance().pdf;
		const Double ecm = Settings::getInstance().ECM;
		PhaseSpace_TauYCosth phaseSpacePoint = generate();

		// independent variables
		const Double cosTheta = phaseSpacePoint.cosTheta;
		const Double rho = phaseSpacePoint.tau; // rho is used to determine tau/y
		const double rand_y = phaseSpacePoint.y;

	   
		// other variables
	    const Double jacobian = (MASS_TR*WIDTH_TR) / (std::pow(std::cos(rho),2) * ecm);

		const Double s_hat = MASS_TR*WIDTH_TR*std::tan(rho) + std::pow(MASS_TR,2);

		const Double yMax = -0.5*std::log(s_hat/ecm);
		const Double deltaY = 2.0*yMax;
		const Double y =  (2.0*rand_y - 1.0)*yMax;

		const Double x1 = std::sqrt(s_hat/ecm)*std::exp(y);
		const Double x2 = std::sqrt(s_hat/ecm)*std::exp(-y);

		Double weight = computeWeight(s_hat, x1, x2, cosTheta);
		// go ahead and scale it this way here
		weight *= (jacobian * deltaY);
		weight /= (x1 * x2);
		
		return weight;
	}

	XSResult PP2Zg2ll::Integrate() {
		return {.result=Sigma(), .error=0.0};
	}

   
}; // namespace ColSim
