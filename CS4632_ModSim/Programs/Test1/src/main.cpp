#include <math.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <functional>
#include <vector>

#include "LHAPDF/LHAPDF.h"

// general constants
#define ALPHA           7.2973525693e-3F // dimensionless
#define Z_MASS          91.1880F         // GeV/c^2
#define Z_WIDTH         2.4414F          // GeV
#define MAGIC_FACTOR    3.893793721e8F   // GeV^2 pb
#define FERMI_CONSTANT  1.1663788e-5F    // GeV^-2
#define WEINBERG_ANGLE  0.222246F        // dimensionless 
#define PI              3.141592653F     // retroencabulators / hr^2


// some problem specific constants
#define N_EVAL          1000000          // number of phase space points to evaluate for the integral
#define N_EVENTS        10000            // number of events to generate
#define ECM             14000.0F         // center of mass energy

#define Q_MIN           60.0F            // minimum energy cutoff
#define MASS_TR         60.0F            // transformed mass
#define WIDTH_TR        60.0F            // transformed width


constexpr const double BIG_S = ECM * ECM;

constexpr const double Q_MIN_2 = Q_MIN * Q_MIN;
constexpr const double MASS_TR_2 = MASS_TR * MASS_TR;
constexpr const double WIDTH_TR_2 = WIDTH_TR * WIDTH_TR;

constexpr const double RHO_MIN = atan((Q_MIN_2 - MASS_TR_2) / (WIDTH_TR * MASS_TR));
constexpr const double RHO_MAX = atan((BIG_S - MASS_TR_2) / (WIDTH_TR * MASS_TR));
constexpr const double DELTA_RHO = RHO_MAX - RHO_MIN;

constexpr const double Z_MASS_2 = Z_MASS * Z_MASS;
constexpr const double Z_WIDTH_2 = Z_WIDTH * Z_WIDTH;


struct CrossSectionResult {
	double sigma;
	double error;

	double maxWeight;
	double maxCosTheta;
	double maxS_hat;
};


struct PhaseSpacePoint {
	double cosTheta, rho, jacobian, s_hat, y, x1, x2, deltaY;

	PhaseSpacePoint() : cosTheta(0.0F), rho(0.0F), jacobian(0.0F),
						s_hat(0.0F), y(0.0F), x1(0.0F), x2(0.0F),
						deltaY(0.0F) {}
	PhaseSpacePoint(const double _cosTheta, const double _rho,
					const double _jacobian, const double _s_hat,
					const double _y, const double _x1,
					const double _x2, const double _deltaY)
		: cosTheta(_cosTheta), rho(_rho), jacobian(_jacobian),
		  s_hat(_s_hat), y(_y), x1(_x1), x2(_x2), deltaY(_deltaY) {}
};


struct FourMomentum {
	double e, px, py, pz;

	FourMomentum() : e(0.0F), px(0.0F), py(0.0F), pz(0.0F) {}
	FourMomentum(const double _e, const double _px, const double _py, const double _pz) : e(_e), px(_px), py(_py), pz(_pz) {}
	
    void zBoost(const double beta) {
	    const double gamma = sqrt(1.0F / (1.0F - pow(beta, 2)));
		const double e = this->e, pz = this->pz; // store these as constants
		
		this->e = gamma * e -  gamma * beta * pz;
		this->px = this->px;
		this->py = this->py;
		this->pz = -gamma * beta * e + gamma * pz;
	}

	friend std::ostream& operator<<(std::ostream& s, const FourMomentum & self) {
		s << std::setprecision(std::numeric_limits<double>::max_digits10)
		  << "[" << self.e << ", " << self.px << ", " << self.py << ", " << self.pz << "]\n";
		
		return s;
	}
};




struct EventResult {
	// phase space points in the event
	// Q, y, costheta, x1
	double points[N_EVENTS][4];

	// generated fourmomenta of q1, q2, mu-, mu+
	FourMomentum momenta[N_EVENTS][4];


	EventResult() {}

	void genDatafile(const char* filePath) {
		std::ofstream fileStream(filePath);
		if (fileStream.rdstate() & std::ofstream::badbit) {
			std::cout << "File \"" << filePath << "\" does not exist.\n";
			return;
		}

		for (int i=0; i<N_EVENTS; i++) {
			fileStream << std::setprecision(std::numeric_limits<double>::max_digits10)
					   << this->points[i][0] << " " << this->points[i][1] << " " << this->points[i][2] << " " << this->points[i][3] << "\n";
		}

		fileStream.close();
	}
};




// there are no such functions to return a random double
// so i make my own
double randDouble() {
	return (double)rand() / (double)RAND_MAX;
}







// =============================
//   Partonic cross section
// =============================

// this can be constexpr'ed, everything else can't... :(
double kappa() {
	const double kappa = sqrt(2.0F) * FERMI_CONSTANT * Z_MASS_2 / (4.0F * PI * ALPHA);
	return kappa;
}


double chi1(const double s_hat) {
	const double chi1 = kappa() * s_hat * (s_hat - Z_MASS_2) / (pow(s_hat - Z_MASS_2, 2) + Z_WIDTH_2 * Z_MASS_2);
	return chi1;
}

double chi2(const double s_hat) {
	const double chi2 = pow(kappa(), 2) * pow(s_hat, 2) / (pow(s_hat - Z_MASS_2, 2) + Z_WIDTH_2 * Z_MASS_2);
	return chi2;
}


double A0(const int qtype, const double s_hat) {
	double CAe = -0.5F, CVe = -0.5F + 2 * WEINBERG_ANGLE;
	double CVf, CAf, Qf;
	if (qtype == 0) { // up-type
	    CVf = 0.5F - (4.0F / 3.0F) * WEINBERG_ANGLE;
		CAf = 0.5F;
		Qf = 2.0F / 3.0F;
	} else { // down-type
	    CVf = -0.5F + (2.0F / 3.0F) * WEINBERG_ANGLE;
		CAf = -0.5F;
		Qf = -1.0F / 3.0F;
	}

	const double a0 = Qf*Qf - 2.0F * Qf * CVe * CVf * chi1(s_hat) + (CAe*CAe + CVe*CVe) * (CAf*CAf + CVf*CVf) * chi2(s_hat);
	
	// std::cout << std::setprecision(std::numeric_limits<double>::max_digits10)
	// 		  << "A0: " << a0 << "\n";
	
	return a0;
}

double A1(const int qtype, const double s_hat) {
	double A_mu = -0.5F, V_mu = -0.5F + 2 * WEINBERG_ANGLE;
	double V_quark, A_quark, Q_quark;
	if (qtype == 0) { // up-type
		V_quark = 0.5F - (4.0F / 3.0F) * WEINBERG_ANGLE;
		A_quark = 0.5F;
		Q_quark = 2.0F / 3.0F;
	} else {
		V_quark = -0.5F + (2.0F / 3.0F) * WEINBERG_ANGLE;
		A_quark = -0.5F;
		Q_quark = -1.0F / 3.0F;
	}

	const double a1 = -4.0F * Q_quark * A_mu * A_quark * chi1(s_hat) + 8.0F * A_mu * V_mu * A_quark * V_quark * chi2(s_hat);

	// std::cout << std::setprecision(std::numeric_limits<double>::max_digits10)
	// 		  << "A1: " << a1 << "\n";

	return a1;
}


double dSigmaHat(const double cosTheta, const int qtype, const double s_hat) {
	// this extra 3.0 is for the initial color averaging
	// --------------------------------------\|/
	return (2 * PI * pow(ALPHA, 2) / (4.0F * 3.0 * s_hat)) * (A0(qtype, s_hat) * (1 + pow(cosTheta, 2)) + A1(qtype, s_hat) * cosTheta);
}



// computes the weight for all light quark types: u, d, s, c
// we use z mass for PDF energy scale?
double computeWeight(const double s_hat, const double x1, const double x2, const double cosTheta, const LHAPDF::PDF *const pdf) {
	double weight = 0.0F;
	// up-type quarks
	weight += dSigmaHat(cosTheta , 0, s_hat) * ((pdf->xfxQ2(2 , x1, s_hat) * pdf->xfxQ2(-2, x2, s_hat)) + (pdf->xfxQ2(4 , x1, s_hat) * pdf->xfxQ2(-4, x2, s_hat)));	
	weight += dSigmaHat(-cosTheta, 0, s_hat) * ((pdf->xfxQ2(-2, x1, s_hat) * pdf->xfxQ2(2 , x2, s_hat)) + (pdf->xfxQ2(-4, x1, s_hat) * pdf->xfxQ2(4 , x2, s_hat)));
	// down-type quarks
	weight += dSigmaHat(cosTheta , 1, s_hat) * ((pdf->xfxQ2(1 , x1, s_hat) * pdf->xfxQ2(-1, x2, s_hat)) + (pdf->xfxQ2(3 , x1, s_hat) * pdf->xfxQ2(-3, x2, s_hat)));
	weight += dSigmaHat(-cosTheta, 1, s_hat) * ((pdf->xfxQ2(-1, x1, s_hat) * pdf->xfxQ2(1 , x2, s_hat)) + (pdf->xfxQ2(-3, x1, s_hat) * pdf->xfxQ2(3 , x2, s_hat)));

	return weight;
}




PhaseSpacePoint computePhaseSpacePoint() {
	const double cosTheta = -1.0F + randDouble() * 2.0F;
	const double rho = RHO_MIN + randDouble() * DELTA_RHO;
		
	// jacobian for the transformation
	const double jacobian = (MASS_TR * WIDTH_TR)/ (pow(cos(rho), 2) * BIG_S);

	const double s_hat = MASS_TR * WIDTH_TR * tan(rho) + pow(MASS_TR, 2);

	const double yMax = -0.5F * log(s_hat / BIG_S);
	const double deltaY = 2.0F * yMax;
	const double y = (2 * randDouble() - 1.0F) * yMax;

	const double x1 = sqrt(s_hat / BIG_S) * exp(y);
	const double x2 = sqrt(s_hat / BIG_S) * exp(-y);

	PhaseSpacePoint point{cosTheta, rho, jacobian, s_hat, y, x1, x2, deltaY};
	return point;
}




CrossSectionResult computeCrossSection(const LHAPDF::PDF *const pdf) {
    double weightSum = 0.0F, weightSquaredSum = 0.0F;
	double maxWeight = 0.0F, maxCosTheta = 0.0F, maxS_hat = 0.0F;
	const double NF = (double)N_EVAL;

	for (int i=0; i<N_EVAL; i++) {
		const PhaseSpacePoint point = computePhaseSpacePoint();

		// multiply by the jacobian, all the variable ranges,
		// then divide by x1, x2 since we can only get xf(x) instead of just f(x)
		const double weight = computeWeight(point.s_hat, point.x1, point.x2, point.cosTheta, pdf) *
			point.jacobian * 2.0F * DELTA_RHO * point.deltaY / (point.x1 * point.x2);

		weightSum += weight;
		weightSquaredSum += pow(weight, 2);

		if (weight > maxWeight) {
			maxWeight = weight;
			maxCosTheta = point.cosTheta;
			maxS_hat = point.s_hat;
		}
	}

	// get the cross section
	double sigma = weightSum / NF;
	sigma *= MAGIC_FACTOR;

	// calculate errors
	double variance = (weightSquaredSum / NF - pow(weightSum / NF, 2));
	double error = sqrt(variance / NF);
	error *= MAGIC_FACTOR;

	// collect everything and return
	CrossSectionResult res;
	res.sigma = sigma;
	res.error = error;

	res.maxWeight = maxWeight;
	res.maxCosTheta = maxCosTheta;
	res.maxS_hat = maxS_hat;

	return res;
}




EventResult generateEvents(const CrossSectionResult res, const LHAPDF::PDF *const pdf) {
	unsigned int numEvents = 0;

	EventResult events;
	
	while (numEvents < N_EVENTS) {
		const PhaseSpacePoint point = computePhaseSpacePoint();

		const double weight = computeWeight(point.s_hat, point.x1, point.x2, point.cosTheta, pdf) *
			point.jacobian * 2.0F * DELTA_RHO * point.deltaY / (point.x1 * point.x2);

		const double Q = sqrt(point.s_hat);
		
		const double prob = weight / res.maxWeight;
		const double randNum = randDouble();

		if (randNum > prob) {
			const double phi = randDouble() * 2.0F * PI;
			const double sinPhi = sin(phi);
			const double cosPhi = cos(phi);
			const double cosTheta = point.cosTheta;
			const double sinTheta = sqrt(1.0F - pow(cosTheta, 2));

			const double beta = (point.x2 - point.x1) / (point.x2 + point.x1);
			
			FourMomentum q1(0.5F * point.x1 * ECM, 0.0F, 0.0F, 0.5F * point.x1 * ECM);
			q1.zBoost(beta);
			FourMomentum q2(0.5F * point.x2 * ECM, 0.0F, 0.0F, 0.5F * point.x2 * ECM);
			q1.zBoost(beta);
			FourMomentum mu1(0.5F * Q, 0.5F * Q * sinTheta * cosPhi, 0.5F * Q * sinTheta * sinPhi, 0.5F * Q * cosTheta);
			mu1.zBoost(beta);
			FourMomentum mu2(0.5F * Q, -0.5F * Q * sinTheta * cosPhi, -0.5F * Q * sinTheta * sinPhi, -0.5F * Q * cosTheta);
			mu2.zBoost(beta);

			double points[4] = {Q, point.y, point.cosTheta, point.x1};
			std::copy(points, points+4, events.points[numEvents]);
			
			FourMomentum momenta[4] = {q1, q2, mu1, mu2};
			std::copy(momenta, momenta+4, events.momenta[numEvents]);

			numEvents++;
		}
	}

	return events;
}


 
int main() {
	LHAPDF::PDF* pdf = LHAPDF::mkPDF("cteq6l1", 0);
	
	CrossSectionResult res = computeCrossSection(pdf);

	std::cout << std::setprecision(std::numeric_limits<double>::max_digits10);
	
	std::cout << "Result: " << res.sigma << " +- " << res.error << " pb\n";
	
	std::cout << "Max weight/dsigma: " << res.maxWeight
			  <<" with cosTheta = " << res.maxCosTheta << " and s_hat = " << res.maxS_hat << "\n" << std::endl;

	std::cout << "Generating " << N_EVENTS << " events:\n";
	EventResult events = generateEvents(res, pdf);
	std::cout << "Printing first 10 events:\n";
	for (int i=0; i<10; i++) {
		std::cout << "Event #" << i + 1 << "\n";
		std::cout << "q1: "  << events.momenta[i][0]
				  << "q2: "  << events.momenta[i][1]
				  << "mu+: " << events.momenta[i][2]
				  << "mu-: " << events.momenta[i][3]
				  << std::endl;
	}
	events.genDatafile("./res/output.dat");
	
	return 0;
}
