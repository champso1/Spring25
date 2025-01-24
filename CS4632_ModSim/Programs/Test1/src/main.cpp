#include <iostream>
#include <string>

#include "gsl/gsl_monte.h"
#include "gsl/gsl_rng.h"
#include "gsl/gsl_monte_plain.h"


#define PI            (3.141592653589793238F)
#define MAGIC_FACTOR  (0.3893793721F)
#define ALPHA         (0.00754677111397F)


typedef struct _DiffXCParams {
	int ECM;
} DiffXCParams;

double diffXC(double x[], size_t dim, void* p) {
    int ECM = ((DiffXCParams*)p)->ECM;
	if (dim != 1) abort(); // only 1 dimension!
	double res = ((PI * ALPHA*ALPHA) / (2 * ECM * ECM)) * (1 + x[0] * x[0]);

	// now to convert to milli barns, then to micro then nano then pico barns
	res *= MAGIC_FACTOR * 1000.0F * 1000.0F * 1000.0F;
	return res;
}


 
int main(int argc, char* argv[]) {
	if (argc != 2) return 1;

	const int ECM = std::stoi(argv[1]);

	gsl_monte_plain_state* s = gsl_monte_plain_alloc(1);

	gsl_monte_function F;
	DiffXCParams params = { ECM };

	F.f = &diffXC;
	F.dim = 1;
	F.params = (void*)(&params);

	double xl[1] = { -1 };
	double xu[1] = { 1 };

	const gsl_rng_type* T;
	gsl_rng* r;
	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc(T);

	double res, error;

	gsl_monte_plain_integrate(&F, xl, xu, 1, 500000, r, s, &res, &error);
	
	gsl_monte_plain_free(s);
	gsl_rng_free(r);

	std::cout << "Result: " << res << "\n";
	std::cout << "Error: " << error << std::endl;
}
