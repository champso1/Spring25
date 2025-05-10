import numpy as np
from matplotlib import pyplot as plt

with np.load("./outputs/data/data.npz") as f:
    EvenIt = f['EvenIt']
    EvenE = f['EvenE']
    OddIt = f['OddIt']
    OddE = f['OddE']
    temp = f['temp']
    s = f['s_imp']
    tchi = f['tchi_imp']
    Cv = f['Cv_imp']
    Rw = f['Rw']

outputPrefix = "./outputs/plots/"


ax = plt.subplot()
ax.plot(EvenIt[1:-1], EvenE[1:-1])
ax.set_title("Energies for Even iterations")
ax.set_xlabel("Iteration")
ax.set_ylabel("Energy")
plt.savefig(outputPrefix+"even.pdf")

ax.clear()
ax.plot(OddIt[1:-1], OddE[1:-1])
ax.set_title("Energies for Odd iterations")
ax.set_xlabel("Iteration")
ax.set_ylabel("Energy")
plt.savefig(outputPrefix+"odd.pdf")

ax.clear()
ax.set_xscale('log')
ax.plot(temp[1:-1], s[1:-1] / 2.0)
ax.axhline(y=np.log(2), color='r', linestyle='--', label='$\\ln 2$')
ax.legend(loc="lower right")
ax.set_xlim((1e-15,1e-1))
ax.set_ylim((0.0,0.75))
ax.set_title("Entropy")
ax.set_xlabel("Temperature")
ax.set_ylabel("$S_{\\mathrm{imp}}/k_B$")
plt.savefig(outputPrefix+"s.pdf")

ax.clear()
ax.set_xscale('log')
ax.plot(temp[1:-1], tchi[1:-1])
ax.axhline(y=(1.0/8.0), color='r', linestyle='--', label="1/8")
ax.legend(loc="lower right")
ax.set_xlim((1e-13,1e-1))
ax.set_ylim((0.0,0.25))
ax.set_title("Impurity contribution to magnetic susceptibility")
ax.set_xlabel("Temperature")
ax.set_ylabel("$k_BT\\chi_{\\mathrm{imp}}$")
plt.savefig(outputPrefix+"tchi.pdf")

ax.clear()
ax.set_xscale('log')
ax.plot(temp[1:-1], Cv[1:-1])
ax.set_xlim((1e-15,1e-1))
ax.set_title("Impurity contribution to specific heat")
ax.set_xlabel("Temperature")
ax.set_ylabel("$C_{v,\\mathrm{imp}}/k_B$")
plt.savefig(outputPrefix+"Cv.pdf")


with open("outputs/data/Rw.dat", "w") as f:
    for i in range(len(Rw)):
        f.write("{:.3f}\n".format(Rw[i] * 4))

