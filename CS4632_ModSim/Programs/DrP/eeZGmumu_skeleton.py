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

## Print some stuff
print '\n'
print '----====================================================----'
print 'Exercise 1 (Advanced Scientific Computing Workshop, July 2014)'
print 'e+e- --> Z/gamma -> mu+mu-'
print 'SKELETON!'
print '----====================================================----'
print '\n'

## some parameters: equivalent to those in MadGraph
pb_convert = 3.894E8 # conversion factor GeV^-2 -> pb
MZ = 91.188    # Z boson mass
GAMMAZ = 2.4414 # Z boson width
alpha = 1/132.507 # alpha QED
Gfermi = 1.16639E-5 # fermi constant
sw2 = 0.222246 # sin^2(weinberg angle)

# e+e- COM energy in GeV
ECM = 90
hats = ECM**2
print "e+e- com energy:", ECM, "GeV"

## first we define a function that gives the differential cross section
## as a function of the parameters
## we include anything that depends on s, even though in this case they are fixed for future use with variable s (hadron collisions)
def dsigma(costh):
    # expression goes here
    return 0

## in the first step, we aim to calculate:
## - the cross section
## - and the maximum point of the phase space

# set the seed for random numbers 
# random.random() will give us random numbers using the Mersenne Twister algorithm
# see: https://docs.python.org/2/library/random.html
seed = 12342
random.seed(seed)

print 'integrating for cross section and maximum!'
print '...'

# the loop for the integration should start here
# N is the number of points we wish to integrate over
N = 1000
for ii in range(0,N):
    # print progress
    #sys.stdout.write("progress: %d%%   \r" % (float(ii)*100./(N)) )
    #sys.stdout.flush() 
    # We need to generate a random cos(theta)
    # this corresponds to a phase-space point 
    # then we calculate the "weight" for that
    # point and increment it to the sum of weights.
    # Since we also want to calculate the error,
    # we must also calculate the sum of the squares
    # of the weights
    
    # costh_ii = #

    # calc. phase space point weight
    # w_ii = #


    # add to the sums
    #sum_w = sum_w + w_ii

    # sum_w_sq = sum_w_sq + w_ii**2
    # for event generation:
    # check if this point is the maximum
    # and store it if so.

# calculate cross section
# sigma = ..

# and its error through the variance
# variance = ..
# error = ..

# now print the result
# don't forget to convert the GeV^-2 to pb! 
print 'done integrating!'
print '\n'
# print 'maximum value of dsigma = ..'
# print 'total cross section = ..'

# here we can also compare the 

## now that we have the maximum, we can generate "events"
## in this simple case, we only have one parameter, costh
## we can translate this parameter into momenta for the outgoing particles

# we will store "generated" costh in an array:

# required Number of events to generate
Neve = 10
# a counter of events generated in the loop
jj = 0
# start generating events (i.e. "hit or miss") 
print 'generating events...'

# jj counts the events, so stay in this loop until we have generated the required number
while jj < Neve:
    # print progress
    # sys.stdout.write("progress: %d%%   \r" % (float(jj)*100./(Neve)) )
    # sys.stdout.flush()
    # random costheta, chosen in the same way as in the integation loop

    # costh_ii = 

    # calc. phase space point weight also as above
    
    #w_ii = .. 

    # Divide the weight by the maximum weight stored in the integration step
    # this will create a "probability for the event"

    #    prob = ..

    # compare to a random number
    # and accept or reject the event
    # according to the hit-or-miss algorithm

    # if the random number is less than the probability of the PS point
    # accept
    
    #if rand_num < prob:
    #    jj = jj+1
    # ACCEPT HERE AND DO STUFF SUCH AS PLOTTING, SETTING UP THE MOMENTA 
        
        
