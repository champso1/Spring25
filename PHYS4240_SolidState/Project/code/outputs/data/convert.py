import numpy as np


with np.load("./data.npz") as f:
    temps = f["temp"]
    s = f["s_imp"]
    s = s / 2 # make all observables of the same order
    tchi = f["tchi_imp"]
    Cv = f["Cv_imp"]

    with open("./data.dat", "w") as ff:
        for i in range(len(temps)):
            ff.write("{:.6}\t{:.6}\t{:.6}\t{:.6}\n".format(
                temps[i], s[i], tchi[i], Cv[i]
            ))


with np.load("./nrgljubljana.npz") as f:
    temps = f["temps"]
    s = f["s"]
    s = s / np.log(2) # make all observables of the same order
    tchi = f["tchi"]
    Cv = f["Cv"]

    with open("./nrgljubljana.dat", "w") as ff:
        for i in range(len(temps)):
            ff.write("{:.6}\t{:.6}\t{:.6}\t{:.6}\n".format(
                temps[i], s[i], tchi[i], Cv[i]
            ))

with np.load("./matlab.npz") as f:
    temps = f["temps"]
    s = f["S"]
    s = s / np.log(2) # make all observables of the same order
    tchi = f["tchi"]
    Cv = f["Cv"]

    with open("./matlab.dat", "w") as ff:
        for i in range(len(temps)):
            ff.write("{:.6}\t{:.6}\t{:.6}\t{:.6}\n".format(
                temps[i], s[i], tchi[i], Cv[i]
            ))
