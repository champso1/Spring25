#!//////////////////////////////////////////////////////////#
#!//e+e- -> Z/gamma -> mu+mu-
#!//Andreas Papaefstathiou
#!//July 2014
#!//////////////////////////////////////////////////////////#
#! /usr/bin/env python

#python stuff that may or may not be useful
from __future__ import with_statement
import math, cmath, string, os, sys, fileinput, pprint
from optparse import OptionParser
import random

## import LHAPDF and initialize PDFs
## need to compile the LHAPDF library first to be able to do this!
## get it at:
## http://lhapdf.hepforge.org/
import lhapdf
## initializes PDF member object (for protons)
## for antiprotons the PDFs are in fact identical,
## an antiquark taken from a proton has the same probability as a quark from a proton
p = lhapdf.mkPDF("cteq6l1", 0)

## Print some stuff
print '\n'
print '----====================================================----'
print 'Exercise 2 (Advanced Scientific Computing Workshop, July 2014)'
print 'pp --> Z --> mu+ mu-'
print 'SKELETON!'
print '----====================================================----'
print '\n'

## define a fake parton density function:
## if you do not wish (or cannot) use lhapdf 6
## a function of q2 (the momentum transfer) and the momentum fraction x
## this will not give the correct cross section! 
def xfxQ_fake(id, x, q2):
    if abs(id) > 5 and id != 21:
        return 0
    else:
        return 0.15 * math.sin(20.0*x) * math.sin(20.0*q2)

## this function boosts along the z-direction
## and is used to go from the COM frame to the lab frame
def boostz(fourvector, beta):
  gamma = math.sqrt( 1  / (1 - beta**2) )
  boosted = [ gamma * fourvector[0] - gamma * beta * fourvector[3], fourvector[1], fourvector[2], - gamma * beta * fourvector[0] + gamma * fourvector[3] ]
  return boosted

## some parameters
pb_convert = 3.894E8 # conversion factor GeV^-2 -> pb
MZ = 91.188    # Z boson mass
GAMMAZ = 2.4414 # Z boson width
alpha = 1/132.507 # alpha QED
Gfermi = 1.16639E-5 # fermi constant
sw2 = 0.222246 # sin^2(weinberg angle)

# PP COM energy in GeV
ECM = 14000
s = ECM**2
print "hadron com energy:", ECM, "GeV"

# we also set minimum Q
# for this exercise we choose 60 GeV
Q_min = 60


## define a function that gives the differential cross section
## as a function of the parameters
## it has to depend on the partonic cross section, s hat, the cosine of the scattering angle, theta,
## and the type of parton involved in the interaction (up, down, strange, charm)
def dsigma(costh, hats, qtype):
    # ..
    return 0

## for convenience,
## define a function that calculates the weight for the phase space point
## we will multiply by the range of integration later on
## this function will also take care of the parton density functions as now they are 
def weight(hats, mu, x1, x2):
    # the function must take care of up-type quarks
    # (in LHAPDF these are up: id 2, charm: id 4)
    # by:
    # w_ii = differential cross section * relevant PDFs for up, charm
    # and must also take care of down-type quarks
    # down: id 1, strange: id 3
    # w_ii = w_ii + differential cross section * relevant PDFs for up, down
    # (we neglect the bottom quark as the contribution would be rather small: you can try adding it if you wish to see what the effect is:
    # bottom: id 5)
    # REMEMBER:
    # after multiply by ranges and Jacobian, divide by the x1 and x2 since xfxQ gives x * f(x)
    return 0


## in the first step, we aim to calculate:
## - the cross section
## - and the maximum point of the phase space

# set the seed for random numbers 
# random.random() will give us random numbers using the Mersenne Twister algorithm
# see: https://docs.python.org/2/library/random.html
seed = 12342
random.seed(seed)

# choose the transform mass and width
# these can be taken to be some values related to the Z-mass
# or to the minimum cut
# play around with these to figure out the optimal values
#MTR = 
#GTR = 

# for costh this is 1 - (-1) = 2
deltath = 2
# choose rho limits (see transformation)
rho1 = math.atan( Q_min**2 - MTR**2 ) / (GTR*MTR) 
rho2 = math.atan( (s - MTR**2) / (GTR*MTR) )
deltarho = rho2 - rho1


# define number of integration points
N = 1000000

# loop over N phase space points,
# sum the weights up and the weights squared (for the variance)
print 'integrating for cross section and maximum!'
print '...'

for ii in range(0,N):
    # print progress
    sys.stdout.write("progress: %d%%   \r" % (float(ii)*100./(N)) )
    sys.stdout.flush() 
    # pick random phase space point:
    # we need to pick three variables 

    # costh_ii = ..
    # rho = ..

    # get maximum rapidity of dilepton, 

    # Y = - 0.5 * math.log(hats/s)

    # and get a random value of y
    # y = ( (2 * random.random()) - 1 ) * Y

    # The Jacobian (as defined in the lecture)
    Jac = (MTR * GTR) / ( math.cos(rho)**2 * s)
    
    # we will also get s-hat, as it is easier to call the
    # functions and calculate other stuff
    hats = MTR * GTR * math.tan(rho) + MTR**2
 
    # calculate momentum fractions x1, x2 from the expressions given in the lectures

    # x1 = math.sqrt(hats/s) * math.exp(y)
    # x2 = math.sqrt(hats/s) * math.exp(-y)
    
    # set the scale for the pdfs

    # calc. phase space point weight
    # w_ii = weight(hats, mu, x1, x2) * RANGES * Jac / (x1 * x2)

    # add to the sums for the cross section
    # and the variance

    # sum_w = sum_w + w_ii
    # sum_w_sq = sum_w_sq + w_ii**2
    
    # check if this point is largeer than maximum
    # if so, store before going to the next point

    # if w_ii > w_max:
        #w_max = w_ii
     
 # calculate cross section
sigma = sum_w / N

# and its error through the variance
# variance = ..
# error = ..

print 'done integrating!'
print '\n'
# print total cross section. Remember to convert to pb!

#print 'total cross section =', ..

## now that we have the maximum, we can generate "events

# choose the transform mass and width
MTR = MZ
GTR = GAMMAZ

# we will store "generated" costh in an array:
PScosth = []
PSQ = []
PSx = []
PSy = []

# Number of events to generate
Neve = 10
# counter of events generate 
jj = 0
# start generating events (i.e. "hit or miss")
print 'generating events...'

while jj < Neve:
    #print progress
    # sys.stdout.write("progress: %d%%   \r" % (float(jj)*100./(Neve)) )
    # sys.stdout.flush()
    # random costheta and rho
    
    # costh_ii = ..
    # rho = ..

    # jacobian as before
    Jac = (MTR * GTR) / ( math.cos(rho)**2 * s)
    
    # get s hat
    # hats = MTR * GTR * math.tan(rho) + MTR**2
    # get maximum rapidity of dilepton, Y and find the range of integration for y (=2*Y)
    # Y = - 0.5 * math.log(hats/s)
    # get a random value of y
    y = ( (2 * random.random()) - 1 ) * Y
        
    # calculate momentum fractions x1, x2
    # x1 = math.sqrt(hats/s) * math.exp(y)
    # x2 = math.sqrt(hats/s) * math.exp(-y)

    # set the scale for the pdfs, we choose MZ to be compatible with MadGraph
    mu = MZ
    
    # calc. phase space point weight

    # w_ii = weight(hats, mu, x1, x2) * RANGES OF INTEGRATION * Jac / (x1 * x2)

    # now divide by maximum and compare to probability

    # prob = w_ii / w_max

    # pick a random number

    # rand_num = random.random()

    # if the random number is less than the probability of the PS point
    # accept
    if rand_num < prob:
        #jj = jj+1
        # here one can either analyze the event
        # or store them for later convenience
       
