import numpy as np
import time
from tqdm import tqdm
import math

GAMMA = 0.004
EPS_D = -0.00005
U = 0.0001

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

# for plotting
ENMAX_PL = 32
EvenIt = np.zeros((int(ITMAX/2)+1))
EvenE = np.zeros((int(ITMAX/2)+1, ENMAX_PL))
OddIt = np.zeros((int(ITMAX/2)))
OddE = np.zeros((int(ITMAX/2), ENMAX_PL))


TOLERANCE = 1e-5



L = 4



outfile = open("./outputs/out.log", "w")




def enkode(Sz, Q):
    return (Sz+50)*100 + (Q+50)


def dekode(kode):
    Q = np.remainder(kode,100)-50
    Sz = (kode - np.remainder(kode,100))/100 - 50
    return (Sz, Q)


def delta_enkode(Sz, Q):
    return Sz*100 + Q

        


class Thermodynamics:
    def __init__(self, fcFilePath):
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

        self.s_0 = np.zeros((ITMAX))
        self.tchi_0 = np.zeros((ITMAX))
        self.Cv_0 = np.zeros((ITMAX))

        self.Rw = np.zeros(ITMAX)


        # load "initial" matrices from fermionic chain
        with np.load(fcFilePath) as f:
            s_avg_fc = f["s_avg"]
            tchi_avg_fc = f["tchi_avg"]
            Cv_avg_fc = f["Cv_avg"]

        lastIdx = len(s_avg_fc)
        for i in range(1,len(s_avg_fc)):
            self.s_0[i] = s_avg_fc[i-1]
            self.tchi_0[i] = tchi_avg_fc[i-1]
            self.Cv_0[i] = Cv_avg_fc[i-1]
        
        if lastIdx < ITMAX:
            for i in range(lastIdx,ITMAX):
                self.s_0[i] = s_avg_fc[lastIdx-1]
                self.tchi_0[i] = tchi_avg_fc[lastIdx-1]
                self.Cv_0[i] = Cv_avg_fc[lastIdx-1]
        
        

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


    def computeImpurityTerms(self):
        for i in range(ITMAX):
            self.s_imp[i] = self.s_avg[i] - self.s_0[i]
            self.tchi_imp[i] = self.tchi_avg[i] - self.tchi_0[i]
            self.Cv_imp[i] = self.Cv_avg[i] - self.Cv_0[i];


    def computeWilsonRatios(self):
        for i in range(ITMAX):
            if self.Cv_imp[i] < 1e-10:
                continue
            self.Rw[i] = (4*np.pi**2/3.0)*(self.tchi_imp[i]/self.Cv_imp[i])

            


            
therms = Thermodynamics("./outputs/data/data_fc.npz")



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



def saveEvenOddEnergies(it, EvenIt, EvenE, OddIt, OddE):
    if (it%2 == 0):
        EvenIt[int(it/2)] = it
        for i in range(np.min([L, ENMAX_PL])):
            EvenE[int(it/2),i] = E[i]
    else:
        OddIt[int(it/2)-1] = it
        for i in range(np.min([L, ENMAX_PL])):
            OddE[int(it/2)-1,i] = E[i]



def printData(it, Sz, Q, E):
    outfile.write('%s %i\n' % ('Iteration =', it));
    outfile.write('--------------\n');
    outfile.write(' %3s %2s %5s %3s %10s\n' % ('It.', '#', '2Sz', 'Q', 'E'));
    outfile.write('-------------------------------------\n');
    for i in range(L):
        outfile.write('%3i %3i %4i %4i %20.16f\n' % (0, i, Sz[i], Q[i], E[i]));




# assemble impurity spin up/down
# creation/annihilation operators
# these are identical to conduction electron
# creation/annihilation operators
# we call them both f^\dagger -> fdag_{up/down}
fdag_up[0:L,0:L] = 0; fdag_down[0:L,0:L] = 0
fdag_up[1,0] = 1; fdag_up[3,2] = 1
fdag_down[2,0] = 1; fdag_down[3,1] = -1

# ---------------
# assemble first matrix
# ---------------

# start by placing the creation/annihilation operators
# in from the hybridization term
H[0:F_NS*L,0:F_NS*L] = 0
H[0:4,4:8]    = GAMMA*fdag_up[0:L,0:L]
H[0:4,8:12]   = GAMMA*fdag_down[0:L,0:L]
H[4:8,12:16]  = GAMMA*fdag_down[0:L,0:L]
H[8:12,12:16] = -GAMMA*fdag_up[0:L,0:L]

# symmetrize
H = H + H.conj().T

# place in the diagonal impurity occupation energies

for i in range(F_NS):
    H[i*F_NS+1,i*F_NS+1] = EPS_D
    H[i*F_NS+2,i*F_NS+2] = EPS_D
    H[i*F_NS+3,i*F_NS+3] = 2*EPS_D + U

            
fdag_up[0:F_NS*L,0:F_NS*L] = 0.0; fdag_down[0:F_NS*L,0:F_NS*L] = 0.0
fdag_up[L:2*L,0:L] = np.eye(L); fdag_up[3*L:4*L,2*L:3*L] = np.eye(L)
fdag_down[2*L:3*L,0:L] = np.eye(L); fdag_down[3*L:4*L,L:2*L] = -np.eye(L)

# assemble the quantum numbers Sz and Q,
# where Sz is actually twice the z-component of the spin
# so as to eliminate fractions,
# and Q is the "charge", which actually is the particle number
# with respect to half filling
# half filling is 1, so Q = N - 1
# with N being the occupation number of that state
Sz[0:L] = [0,1,-1,0]; Q[0:L] = [-1,0,0,1]

# "enkode" the quanatum numbers
# and assemble what the next state's look like (16 of them)
Kodep[0:L] = enkode(Sz[0:L], Q[0:L])
Kode[0:L]     = Kodep[0:L] + delta_enkode(dSz[0],dQ[0])
Kode[L:2*L]   = Kodep[0:L] + delta_enkode(dSz[1],dQ[1])
Kode[2*L:3*L] = Kodep[0:L] + delta_enkode(dSz[2],dQ[2])
Kode[3*L:4*L] = Kodep[0:L] + delta_enkode(dSz[3],dQ[3])

# increase the dimensionality by the number of added states (4)
L *= F_NS

    

# now we find the unique Kode values
# so that we only diagonalize what is necessary
# and avoid rediagonalizing multiple times
xK = np.unique(Kode[0:L])
nK = len(xK)
iK = 0

# E are the energies, c the coefficients of eigenvectors
# HK are the block matrices
E[0:L] = 0.0
c[0:L,0:L] = 0.0
HK[0:L,0:L] = 0.0

# loop over all blocks
for blk in range(nK):
    # find the index/indices in the original Hamiltonian
    # that match this unique value
    b = np.nonzero(Kode == xK[blk])[0];
    LK = len(b)

    # construct the block Hamiltonian for this/these state(s)
    for i in range(LK):
        for j in range(LK):
            HK[i,j] = H[b[i],b[j]]

    #diagonalize
    EK[0:LK], cK[0:LK,0:LK] = np.linalg.eig(HK[0:LK,0:LK])

    for i in range(LK):
        for j in range(LK):
            c[b[i],b[j]] = cK[i,j]

        E[b[i]] = EK[i]
        Kodep[b[i]] = xK[blk];


    # increase iK by the number of states we just solved for
    iK += LK

# add scaling factor
E[0:L] *= LAMBDA**(-0.5)

# sort the energies from smallest to largest
E[0:L], c[0:L,0:L], Kodep[0:L] = Esort(E[0:L], c[0:L,0:L], Kodep[0:L], L)


K = L;
if(L>KEPT):
    L=KEPT


# impose degeneracy over closely lying states,
# i.e. make similar ones identical
imposeDegeneracy(E, L, TOLERANCE)



# shift energies so that ground state is at 0 energy
E[0:L] -= E[0]

fdag_up[0:L,0:L]   = c[0:K,0:L].T @ fdag_up[0:K,0:K] @ c[0:K,0:L];
fdag_down[0:L,0:L] = c[0:K,0:L].T @ fdag_down[0:K,0:K] @ c[0:K,0:L];

# redetermine (decode) Sz and Q for the states in order (for printing)
Sz[0:L], Q[0:L] = dekode(Kodep[0:L])

# compute thermodynamics
therms.compute(0, E, Sz)

# save values of even/odd energy to plot later
saveEvenOddEnergies(0, EvenIt, EvenE, OddIt, OddE);

# print all of the data
printData(0, Sz, Q, E)


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
        H[i,i] = E[i]*LAMBDA**0.5;
        H[L+i,L+i] = E[i]*LAMBDA**0.5;
        H[2*L+i,2*L+i] = E[i]*LAMBDA**0.5;
        H[3*L+i,3*L+i] = E[i]*LAMBDA**0.5;

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
        EK[0:LK], cK[0:LK,0:LK] = np.linalg.eig(HK[0:LK,0:LK])

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
    saveEvenOddEnergies(it, EvenIt, EvenE, OddIt, OddE);
    printData(it, Sz, Q, E);


therms.determineAverages()
therms.computeImpurityTerms()
therms.computeWilsonRatios()
outfile.close()


Simp_UL = np.log(4); Simp_LL = np.log(2);
Tk = 0.0;
found = False; i = 1;
simp_cr = 0.5*(Simp_UL+Simp_LL);
print("Finding Tk")
print("Using simp_cr = {:.5}".format(simp_cr))

while (found == False) and (i < ITMAX-1):
    if(therms.s_imp[i] > simp_cr) and (therms.s_imp[i+1] < simp_cr):
        found = True;
        Tk = np.exp(np.log(therms.temp[i+1]) + (np.log(therms.temp[i])
                                                - np.log(therms.temp[i+1]))*(simp_cr - therms.s_imp[i+1])
                    /(therms.s_imp[i] - therms.s_imp[i+1]));
    i += 1;

if found == True:
    print("Found Tk at {:.9}".format(Tk))
    #therms.temp = therms.temp / Tk
else:
    print("Did not find Tk")

        

np.savez("./outputs/data/data.npz", EvenIt=EvenIt, EvenE=EvenE, OddIt=OddIt, OddE=OddE,
         temp=therms.temp, s_imp=therms.s_imp, tchi_imp=therms.tchi_imp, Cv_imp=therms.Cv_imp, Rw=therms.Rw)
