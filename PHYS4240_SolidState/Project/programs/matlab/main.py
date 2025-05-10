import numpy as np

temps = []
S = []
tchi = []
Cv = []
with open("Thermavg.dat", "r") as f:
    lines = f.readlines()
    for i,_line in enumerate(lines):
        if i < 2: # first line is a comment
            continue
        line = _line.split()
        temps.append(float(line[0]))
        S.append(float(line[1]))
        tchi.append(float(line[2]))
        Cv.append(float(line[3]))


temps = np.array(temps)
tchi = np.array(tchi)
np.savez("matlab.npz", temps=temps, S=S, tchi=tchi, Cv=Cv)
        
