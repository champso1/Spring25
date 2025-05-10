import numpy as np

temps = []
s = []
tchi = []
Cv = []

with open("td-Sz2.dat", "r") as f:
    lines = f.readlines()
    for i,_line in enumerate(lines):
        if i == 0: # first line is a comment
            continue
        line = _line.split(" ")
        temps.append(float(line[0]))
        tchi.append(float(line[1]))

with open("td-S.dat", "r") as f:
    lines = f.readlines()
    for i,_line in enumerate(lines):
        if i == 0: # first line is a comment
            continue
        line = _line.split(" ")
        s.append(float(line[1]))

with open("td-C.dat", "r") as f:
    lines = f.readlines()
    for i,_line in enumerate(lines):
        if i == 0: # first line is a comment
            continue
        line = _line.split(" ")
        Cv.append(float(line[1]))

        
temps = np.array(temps)
s = np.array(s)
tchi = np.array(tchi)
Cv = np.array(Cv)
np.savez("nrgljubljana.npz", temps=temps, s=s, tchi=tchi, Cv=Cv)
        
