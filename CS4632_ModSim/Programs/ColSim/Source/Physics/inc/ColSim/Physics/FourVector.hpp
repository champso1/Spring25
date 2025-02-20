#ifndef __FOUR_VECTOR_HPP
#define __FOUR_VECTOR_HPP

#include "ColSim/Core.h"
#include <cmath>
#include <iostream>
#include <limits>
#include <iomanip>

class FourVector {
private:
	union {
		struct { Double e, px, py, pz; };
		struct { Double t, x, y, z; };
	};

public:
	FourVector() : e(0.0F), px(0.0F), py(0.0F), pz(0.0F) {}
	FourVector(Double _e, Double _px, Double _py, Double _pz)
		: e(_e), px(_px), py(_py), pz(_pz) {}


	inline Double square() {
		return e*e - px*px - py*py - pz*pz;
	}

	inline FourVector& zBoost(const Double beta) {
		const Double gamma = sqrt(1.0F / (1.0F - pow(beta, 2)));
		// store these to avoid modifying while computing
		const Double e = this->e,
			         pz = this->pz;

		this->e = gamma * e - gamma * beta * pz;
		this->pz = -gamma * beta * e + gamma * pz;

		return *this;
	}

	friend std::ostream& operator<<(std::ostream& s, const FourVector& self) {
		s << std::setprecision(std::numeric_limits<double>::max_digits10)
		  << "[" << self.e << ", " << self.px << ", " << self.py << ", " << self.pz << "]";
		return s;
	}
};





#endif // __FOUR_VECTOR_HPP
