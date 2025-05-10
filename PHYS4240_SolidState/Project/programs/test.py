import numpy as np

with np.load("data_fc.npz") as f:
    temp = f['temp']
    tchi = f['tchi_avg']

for i in range(len(temp)):
    print(temp[i], tchi[i])
