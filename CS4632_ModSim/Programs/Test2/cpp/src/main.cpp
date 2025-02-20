#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>
#include <random>
#include <functional>

#include "LHAPDF/Factories.h"
#include "LHAPDF/LHAPDF.h"

#include "gsl/gsl_roots.h"
#include "gsl/gsl_errno.h"

#define DEBUG

#define NC 3.0F
#define TR (1.0F / 2.0F)
#define CA NC
#define CF (((NC*NC) - 1.0F) / (2.0F * NC))

#define N_EVOLUTIONS   1000    // number of branches to evolve
#define Q_CUTOFF       1.0F    // min energy (cutoff) GeV
#define Q_HARD         1000.0F // initial energy of the hard process, GeV

#define Q_SCALE_FIXED  (Q_HARD / 2.0F) // fixed scale for alphaS


// globals
gsl_root_fsolver* root_solver;

LHAPDF::PDF* pdf;

std::default_random_engine engine;
std::uniform_real_distribution<double> distribution;
auto randNum = std::bind(distribution, engine);


class AlphaS {
private:
	int order;
	double aS_mZ; // value of alphaS with scale at mass of Z
	double mZ, mC, mB;    // actual value of masses
	double aS_mB; // value of alphaS with scale at mass of B
	double aS_mC; // value of alphaS with scale at mass of C

	// squared versions of masses
	double mZ_2, mC_2, mB_2;

public:
	AlphaS(double _aS_mZ, double _mZ)
		: order(1), aS_mZ(_aS_mZ), mZ(_mZ), mC(1.27F), mB(4.75F)
	{
		mZ_2 = mZ*mZ;
		mC_2 = mC*mC;
		mB_2 = mB*mB;

		aS_mB = calcAlphaS(mB);
		aS_mC = calcAlphaS(mC);
	}

	double beta0(int nf) { //nf = number of effectively massless flavors
		return (11.0F / 6.0F) * CA - (2.0F / 3.0F) * TR * nf;
	}
	double beta1(int nf) {
		return (17.0F / 6.0F) * CA*CA - ((5.0F / 3.0F) * CA + CF) * TR * nf;
	}

	double calcAlphaS_LO(double t) {
		// based on the input evolution scale,
		// choose either mass of z, b, or c
		// as our reference scale
	    double tref, asref;
		double b0; // value of beta0
		
		if (t >= mB_2) { // use z
			tref = mZ_2;
			asref = aS_mZ;
			b0 = beta0(5) / (2.0F * M_PI); // 5 flavors if t > mB
		} else if (t >= mC_2) {
			tref = mB_2;
			asref = aS_mB;
			b0 = beta0(4) / (2.0F * M_PI); // 4 flavors if mB > t > mC
		} else {
			tref = mC_2;
			asref = aS_mC;
			b0 = beta0(3) / (2.0F * M_PI); // 3 flavors if mC > t
		}

		return 1.0F / ((1.0F / asref) + b0 * std::log(t / tref));
		
	}
	double calcAlphaS_NLO(double t) {
		// based on the input evolution scale,
		// choose either mass of z, b, or c
		// as our reference scale
	    double tref, asref;
		double b0,b1; // value of beta0,1
		
		if (t >= mB_2) { // use z
			tref = mZ_2;
			asref = aS_mZ;
			b0 = beta0(5) / (2.0F * M_PI);
			b1 = beta1(5) / std::pow(2.0F * M_PI, 2);
		} else if (t >= mC_2) {
			tref = mB_2;
			asref = aS_mB;
			b0 = beta0(4) / (2.0F * M_PI);
			b1 = beta1(4) / std::pow(2.0F * M_PI, 2);
		} else {
			tref = mC_2;
			asref = aS_mC;
			b0 = beta0(3) / (2.0F * M_PI);
			b1 = beta1(3) / std::pow(2.0F * M_PI, 2);
		}
		double term = 1.0F + b0 * asref * std::log(t / tref);
		return asref / term * (1.0F - (b1 / b0) * asref * (std::log(term) / term));
	}

	double calcAlphaS(double t) {
		if (order == 0) // LO
			return calcAlphaS_LO(t);
		else
			return calcAlphaS_NLO(t);
	}

	static double trueAlphaS(const LHAPDF::PDF* pdf) {
		return pdf->alphasQ(Q_SCALE_FIXED) / (2.0F * M_PI);
	}

	// get overestimate
	// for fixed scale, just gets actual value
	static double alphaSOver(const LHAPDF::PDF* pdf) {
		return trueAlphaS(pdf);
	}
	
};




// splitting function and its overesimate
double Pqq(double z) {
	return CF * (1.0F + z*z) / (1.0F - z);
}

double Pqq_over(double z) {
	return 2.0F * CF / (1.0F - z);
}




// the t*Gamma function and its inverse
// these use the overestimated value of alphaS
double tGamma(double z, double aS_over) {
	return -2.0F * aS_over * CF * std::log(1.0F - z);
}

double tGammaInverse(double r, double aS_over) {
	return 1.0F - std::exp(-0.5F * r / (CF * aS_over));
}


// lower/upper overestimates for the z integral
double zp_Over(double t, double Q_cut) {
	return 1.0F - std::sqrt(Q_cut*Q_cut / t);
}
double zm_Over(double t, double Q_cut) {
	return std::sqrt(Q_cut*Q_cut / t);
}



struct Params {
	double Q, Q_cut, r, aS_over;
};


double emissionScaleFunc(double logT_over_Q2, void* _params)
{
	Params params = *(Params*)_params;
	double Q = params.Q;
	double Q_cut = params.Q_cut;
	double r = params.r;
	double aS_over = params.aS_over;
	
	// recalculate t and find rho
	double t = Q*Q * std::exp(logT_over_Q2);
	double rho = tGamma(zp_Over(t, Q_cut), aS_over) - tGamma(zm_Over(t, Q_cut), aS_over);

	// solve E
	return logT_over_Q2 - (1.0F / rho) * std::log(r);
}

double getTEmission(double Q, double Q_cut, double R,
					double tfac, double aS_over)
{
	gsl_function func;
	func.function = emissionScaleFunc;
	Params params = {Q, Q_cut, R, aS_over};
	func.params = (void*)(&params);

	double low = std::log(tfac * Q_cut*Q_cut / (Q*Q));
	double high = 0.0F;
	gsl_root_fsolver_set(root_solver, &func, low, high);

	double max_iter = 1000;
	double tolerance = 1e-3F;
	int status, num_iter = 0;
	double root;

	do {
		num_iter++;
		status = gsl_root_fsolver_iterate(root_solver);

		// do a check
		root = gsl_root_fsolver_root(root_solver);
		double x_low = gsl_root_fsolver_x_lower(root_solver);
		double x_high = gsl_root_fsolver_x_upper(root_solver);
		status = gsl_root_test_interval(x_low, x_high, 0.0F, tolerance);

		if (status == GSL_SUCCESS)
			std::cout << "Converged:\n";
#ifdef DEBUG
		std::cout << num_iter << " [" << x_low << ", " << x_high
				  << "] " << root << "\n";
#endif			
		
	} while ((status == GSL_CONTINUE) && (num_iter < max_iter));

	// we solved for the log:
	double t_sol = Q*Q * std::exp(root);
	if (emissionScaleFunc(root, (void*)(&params)) > tolerance)
		return 0.0F; // 0 obviously not possible, so this is an error condition

	return t_sol;
}


// helper function to return kinematics from the emission
double getZEmission(double t, double Q_cut, double r, double aS_over) {
	return tGammaInverse( tGamma(zm_Over(t, Q_cut), aS_over) +
						  r * (tGamma(zp_Over(t, Q_cut), aS_over) - tGammaInverse(zm_Over(t, Q_cut), aS_over)), aS_over);
}

double getPT_2(double t, double z) {
	return z*z * std::pow(1.0F - z, 2) * t;
}

double getM_2(double t, double z) {
	return z * (1.0F - z) * t;
}


struct Emission {
	double t, z, pT_2, m_2;
	bool generated, continue_evolution;
};



Emission generateEmission(double Q, double Q_cut, double tfac, double aS_over) {
	bool generated = true;
	double r1 = randNum();
	double r2 = randNum();
	double r3 = randNum();
	double r4 = randNum();

	double t = getTEmission(Q, Q_cut, r1, tfac, aS_over);
	if (t == 0.0F) // no evolution found, so end
		return (Emission){t, 1.0F, 0.0F, 0.0F, generated, false};

	double z = getZEmission(t, Q_cut, r2, aS_over);
	double pT_2 = getPT_2(t, z);
	double m_2 = getM_2(t, z);

#ifdef DEBUG
	std::cout << "Candidate emission scale: sqrt(t) = " << std::sqrt(t) << "\n";
	std::cout << "Candidate momentum fraction: z = " << z << "\n";
	std::cout << "Candidate transverse momentum squared: pt^2 = " << pT_2 << "\n";
#endif

	if (pT_2 < 0.0F) {
		generated = false;
#ifdef DEBUG
		std::cout << "Emission rejected due to negative pT^2\n";
#endif 
	}

	double splitting_fn_over = Pqq(z) / Pqq_over(z);
	if (splitting_fn_over < r3) {
		generated = false;
#ifdef DEBUG
		std::cout << "Emission rejected due to splitting function overestimate: p = "
				  << splitting_fn_over << "; R = " << r3 << "\n";
#endif
	} else {
#ifdef DEBUG
		std::cout << "Emission NOT rejected due to splitting function overestimate: p = "
				  << splitting_fn_over << "; R = " << r3 << "\n";
#endif
	}


	double aS_fn_over =  AlphaS::trueAlphaS(pdf)/ AlphaS::alphaSOver(pdf);
	if (aS_fn_over < r4) {
		generated = false;
#ifdef DEBUG
		std::cout << "Emission rejected due to alphaS overestimate: p = "
				  << aS_fn_over << "; R = " << r4 << "\n";
#endif
	} else {
#ifdef DEBUG
		std::cout << "Emission NOT rejected due to alphaS overestimate: p = "
				  << aS_fn_over << "; R = " << r4 << "\n";
#endif
	}

	if (!generated) {
		z = 1.0F;
		pT_2 = 0.0F;
		m_2 = 0.0F;
	}

	return (Emission){t, z, pT_2, m_2, generated, true};
}




std::vector<Emission> Evolve(double Q, double Q_min, double aS_over) {
	double t_min = Q_min*Q_min;
	int num_emissions;

	std::vector<Emission> emissions;

	double fac_t = 3.9999F;
	double fac_cutoff = 4.0F;

	double t = Q*Q;
	double z = 1.0F;
#ifdef DEBUG
	std::cout << "Generating evolution for Q = " << Q << "\n";
#endif

	while ((std::sqrt(t)*z) > std::sqrt(fac_cutoff * t_min)) {
		Emission emission = generateEmission(std::sqrt(t)*z, std::sqrt(t_min), fac_t, aS_over);
		t = emission.t;
		z = emission.z;
		double pT_2 = emission.pT_2;
		double m_2 = emission.m_2;
		bool generated = emission.generated;
		bool continue_evolution = emission.continue_evolution;

		if (!continue_evolution) {
#ifdef DEBUG
			std::cout << "No further emissions.\n";
			std::cout << "Total emissions: " << num_emissions << "\n";
			std::cout << "-----\n";
#endif
			return emissions;
		}

		if (t < (fac_cutoff * t_min)) {
			#ifdef DEBUG
			std::cout << "Emission rejected at sqrt(t) = " << std::sqrt(t) << "since it is below cutoff\n";
			std::cout << "Total emissions: " << num_emissions << "\n";
			#endif
			return emissions;
		}

		if (z != 1.0F) {
			// two trues don't matter
			Emission emission = (Emission){std::sqrt(t), z, std::sqrt(pT_2), std::sqrt(m_2), true, true};
			emissions.emplace_back(emission);
#ifdef DEBUG
			std::cout << "Successful emission at sqrt(t) = " << std::sqrt(t)
					  << ", z = " << z
					  << ", pT = " << std::sqrt(pT_2)
					  << ", m = " << std::sqrt(m_2)
					  << "\n";
#endif
			num_emissions++;
		}
	}

#ifdef DEBUG
	std::cout << "No further emissions.\n";
	std::cout << "Total emissions: " << num_emissions << "\n------\n";
#endif

	return emissions;
}



void init() {
	root_solver = gsl_root_fsolver_alloc(gsl_root_fsolver_brent);
	pdf = LHAPDF::mkPDF("cteq6l1", 0);
	// use max double precision when printing stuff
	std::cout << std::setprecision(std::numeric_limits<double>::max_digits10); 
}
void deinit() {
	gsl_root_fsolver_free(root_solver);
	delete pdf;
}

int main() {
	init();
	AlphaS aS(0.118F, 91.1876F);
	double aS_over = AlphaS::alphaSOver(pdf);

	std::vector<Emission> all_emissions;
	
#ifdef DEBUG
	std::cout << "alphaS overestimate = " << aS_over << "\n";
	std::cout << "Using fixed scale " << Q_SCALE_FIXED << "\n";
	std::cout << "Evolving " << N_EVOLUTIONS << " branches from Q = " << Q_HARD << " GeV -> Q_c = " << Q_CUTOFF << " GeV\n";
#endif

	for (int i=0; i<N_EVOLUTIONS; i++) {
		std::vector<Emission> emissions = Evolve(Q_HARD, Q_CUTOFF, aS_over);
		for (const auto& x : emissions)
			all_emissions.emplace_back(std::move(x));
	}

	deinit();
	return 0;
}
