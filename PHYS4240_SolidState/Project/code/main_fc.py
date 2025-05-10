import numpy as np
import time
from tqdm import tqdm


KEPT = 1000
ITMAX = 70
LAMBDA = 3.0

F_NS = 4
MAXDIM = F_NS*KEPT
MAXBLKDIM = KEPT


Kode = np.zeros((MAXDIM))
Kodep = np.zeros((MAXDIM))
        
Sz = np.zeros((MAXDIM))
Q = np.zeros((MAXDIM))
dSz = np.array([0,1,-1,0]);
dQ = np.array([-1,0,0,1])


H = np.zeros((MAXDIM,MAXDIM))
c = np.zeros((MAXDIM,MAXDIM))
E = np.zeros((MAXDIM))

HK = np.zeros((MAXBLKDIM,MAXBLKDIM))
cK = np.zeros((MAXBLKDIM,MAXBLKDIM))
EK = np.zeros((MAXBLKDIM))

fdag_up = np.zeros((MAXDIM,MAXDIM))
fdag_down = np.zeros((MAXDIM,MAXDIM))

TOLERANCE = 1e-5


L = 4




def enkode(Sz, Q):
    return (Sz+50)*100 + (Q+50)


def dekode(kode):
    Q = np.remainder(kode,100)-50
    Sz = (kode - np.remainder(kode,100))/100 - 50
    return (Sz, Q)


def delta_enkode(Sz, Q):
    return Sz*100 + Q

        


class Thermodynamics:
    def __init__(self):
        self.temp = np.zeros((ITMAX))
        self.s = np.zeros((ITMAX))
        self.tchi = np.zeros((ITMAX))
        self.Cv = np.zeros((ITMAX))

        self.s_avg = np.zeros((ITMAX))
        self.tchi_avg = np.zeros((ITMAX))
        self.Cv_avg = np.zeros((ITMAX))

        self.s_imp = np.zeros((ITMAX))
        self.tchi_imp = np.zeros((ITMAX))
        self.Cv_imp = np.zeros((ITMAX))
        

    def compute(self, it, E, Sz):
        beta = 1.0;
        self.temp[it] = (LAMBDA**(-(it-1)/2))/beta;
    
        z = np.sum(np.exp(-beta*E[0:L]));

        self.s[it]    = beta*(sum(E[0:L]*np.exp(-beta*E[0:L]))/z) + np.log(z);
        self.tchi[it] = 0.25*np.sum(Sz[0:L]*Sz[0:L]*np.exp(-beta*E[0:L]))/z - 0.5*np.sum(Sz[0:L]*np.exp(-beta*E[0:L]))/z;
        self.Cv[it]   = np.sum(E[0:L]*E[0:L]*np.exp(-beta*E[0:L]))/z - np.sum(E[0:L]*np.exp(-beta*E[0:L]))/z;


    def determineAverages(self):
        for i in range(1,ITMAX-1):
            self.s_avg[i] = (self.s[i-1] + 2*self.s[i] + self.s[i+1])/4.0
            self.tchi_avg[i] = (self.tchi[i-1] + 2*self.tchi[i] + self.tchi[i+1])/4.0
            self.Cv_avg[i] = (self.Cv[i-1] + 2*self.Cv[i] + self.Cv[i+1])/4.0


therms = Thermodynamics()




def imposeDegeneracy(E0, L, tolerance):
    i = 0
    while (i < L):
        j = i + 1
        while (j < L and np.abs(E0[i]-E0[j]) < tolerance):
            j += 1

        E0[i:j] = np.mean(E0[i:j])
        i = j

    
    

        


def Esort(E0, c0, Kodep0, L):
    Es = np.sort(E0)
    ix = np.argsort(E0)

    cs = np.zeros((L,L)); Ks = np.zeros((L))

    for i in range(L):
        cs[:,i] = c0[:,ix[i]]
        Ks[i] = Kodep[ix[i]]

    return (Es,cs,Ks)





L = 4

# assemble impurity spin up/down
# creation/annihilation operators
# these are identical to conduction electron
# creation/annihilation operators
# we call them both f^\dagger -> fdag_{up/down}
fdag_up[0:L,0:L] = 0; fdag_down[0:L,0:L] = 0
fdag_up[1,0] = 1; fdag_up[3,2] = 1
fdag_down[2,0] = 1; fdag_down[3,1] = -1


#diagonalize
E[0:L], c[0:L,0:L] = np.linalg.eig(H[0:L,0:L])

fdag_up[0:L,0:L]   = c[0:L,0:L].T @ fdag_up[0:L,0:L]   @ c[0:L,0:L];
fdag_down[0:L,0:L] = c[0:L,0:L].T @ fdag_down[0:L,0:L] @ c[0:L,0:L];


Sz[0:L] = [0,1,-1,0]
Q[0:L] = [-1,0,0,1]
Kodep[0:L] = enkode(Sz[0:L],Q[0:L])

# compute thermodynamics
therms.compute(0, E, Sz)



for it in tqdm(range(1,ITMAX)):
    n = it-1;
    t = (1+LAMBDA**(-1))*(1-LAMBDA**(-n-1))/(2*np.sqrt(1-LAMBDA**(-2*n-1))*np.sqrt(1-LAMBDA**(-2*n-3)));


    # Set up the Hamiltonian Matrix elements for current It.
    H[0:L*F_NS,0:L*F_NS] =  0.0
    H[0:L,L:2*L]         =  t*fdag_up[0:L,0:L]
    H[2*L:3*L,3*L:4*L]   = -t*fdag_up[0:L,0:L]
    H[0:L,2*L:3*L]       =  t*fdag_down[0:L,0:L]
    H[L:2*L,3*L:4*L]     =  t*fdag_down[0:L,0:L]
    
    # Symmetrize the Hamiltonian 
    H = H + H.conj().T

    # Set up main diagonal entries
    for i in range(L):
        H[i,i] = E[i]*np.sqrt(LAMBDA);
        H[L+i,L+i] = E[i]*np.sqrt(LAMBDA);
        H[2*L+i,2*L+i] = E[i]*np.sqrt(LAMBDA);
        H[3*L+i,3*L+i] = E[i]*np.sqrt(LAMBDA);

    # Set up the matrices for fdag_{u,d} for current It.
    fdag_up[0:L*F_NS,0:L*F_NS] = 0.0; fdag_down[0:L*F_NS,0:L*F_NS] = 0.0;
    fdag_up[L:2*L,0:L]   = np.eye(L); fdag_up[3*L:4*L,2*L:3*L] =  np.eye(L);
    fdag_down[2*L:3*L,0:L] = np.eye(L); fdag_down[3*L:4*L,L:2*L]   = -np.eye(L);

    # Assign the Kodes for all the basis states in terms of Kodes for prev. It.
    Kode[0:L]       = Kodep[0:L] + delta_enkode(dSz[0],dQ[0]);
    Kode[L:2*L]     = Kodep[0:L] + delta_enkode(dSz[1],dQ[1]);
    Kode[2*L:3*L]   = Kodep[0:L] + delta_enkode(dSz[2],dQ[2]);
    Kode[3*L:4*L]   = Kodep[0:L] + delta_enkode(dSz[3],dQ[3]);
    
    L = L*F_NS;

    xK = np.unique(Kode[0:L]);
    nK = len(xK);
    iK = 0;

    E[0:L] = 0.0;
    c[0:L,0:L] = 0.0;
    HK[0:L,0:L] = 0.0;

    for blk in range(nK):
        b = np.nonzero(Kode == xK[blk])[0];
        LK = len(b)
    
        # Form Hamilton Matrix HQ for the current block k
        for i in range(LK):
            for j in range(LK):
                HK[i,j] = H[b[i],b[j]];
    
        # Find eigenvalues and eigenvectors of the Blk. k
        EK[0:LK], cK[0:LK,0:LK] = np.linalg.eigh(HK[0:LK,0:LK])

        for i in range(LK):
            for j in range(LK):
                c[b[i],b[j]] = cK[i,j]

            E[b[i]] = EK[i]
            Kodep[b[i]] = xK[blk];

                
        # increase iK by the number of states we just solved for
        iK += LK
    

    # sort the energies from smallest to largest
    E[0:L], c[0:L,0:L], Kodep[0:L] = Esort(E[0:L], c[0:L,0:L], Kodep[0:L], L)

    K = L;
    if (L>KEPT):
        L=KEPT;

    imposeDegeneracy(E, L, TOLERANCE)

    # shift energies so that ground state is at 0 energy
    E[0:L] -= E[0]

    # redetermine (decode) Sz and Q for the states in order (for printing)
    Sz[0:L], Q[0:L] = dekode(Kodep[0:L])


    fdag_up[0:L,0:L]   = c[0:K,0:L].T @ fdag_up[0:K,0:K] @ c[0:K,0:L];
    fdag_down[0:L,0:L] = c[0:K,0:L].T @ fdag_down[0:K,0:K] @ c[0:K,0:L];

    therms.compute(it, E, Sz)


therms.determineAverages()


np.savez("./outputs/data/data_fc.npz", temp=therms.temp, s_avg=therms.s_avg, tchi_avg=therms.tchi_avg, Cv_avg=therms.Cv_avg)

with open("./outputs/data/data_fc.dat", "w") as f:
    for i in range(len(therms.temp)):
        f.write('{:.5} {:.5} \n'.format(therms.temp[i], therms.tchi_avg[i]))
