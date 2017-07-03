import numpy as np

col3 = np.loadtxt("trackercal.txt", usecols=2)  # consider only the third column (sigma)
print "FILE: trackercal.txt\n"
print "Number of lines: %d" % len(col3)
sigma = []
for i in xrange(len(col3)):
  if col3[i] < 10.0:  # cut for sigmas > 10 ADC
    sigma.append(col3[i])
print "Number of sigmas left after the cut: %d\n" % len(sigma)
sigma_mean = np.mean(sigma)
print "sigma mean = %1.10f ADC\n" % sigma_mean  # mean = 3.6027857719
