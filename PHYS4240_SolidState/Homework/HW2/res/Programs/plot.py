import numpy as np

m: float = 1.0 # use this as a base
M: float = 1000.0 * m # so we can scale this

# these don't matter
k1: float = 1.0 
k2: float = 1.1
k12: float = 0.9

hbar: float = 1.0 # natural units!

def exactSol(n1: int, n2: int) -> float:
    term1: float = -(k12*(M + m) + m*k1 + M*k2)/(M*m)
    term2: float = (k1*k12 + k1*k2 + k12*k2)/(M*m)
    omega_plus: float  = term1 + np.sqrt(term1**2 - 4*term2)
    omega_minus: float = term1 - np.sqrt(term1**2 - 4*term2)

    sol1: float = (n1 + 0.5) * omega_plus * hbar
    sol2: float = (n2 + 0.5) * omega_minus * hbar

    return sol1 + sol2

    


def approxSol(n1: int, n2: int) -> float:
    k1_prime: float = k1 + (2*k2*k12)/(k2 + k12)
    k2_prime: float = (k1 + k12)
    omega1_prime: float = np.sqrt(k1_prime/M)
    omega2_prime: float  = np.sqrt(k2_prime/m)

    e1: float = (n1 + 0.5)*omega1_prime*hbar + hbar*omega2_prime/2
    e2: float = (n2 + 0.5)*omega2_prime*hbar

    return e1 + e2;







def main():
    # define a bunch of n, m
    points = [(n,m) for n in range(15) for m in range(15)]
    
    # get solutions
    
    approx_sols = [approxSol(n1, n2) for (n1,n2) in points] 
    exact_sols = [exactSol(n1, n2) for (n1,n2) in points]
    #print(approx_sols)
    #print(exact_sols)

    errors = []

    for i in range(len(approx_sols)):
        approx_sol = approx_sols[i]
        exact_sol = exact_sols[i]
        errors.append((approx_sol - exact_sol)/(exact_sol))

    print(errors)
        


if __name__ == "__main__":
    main()
