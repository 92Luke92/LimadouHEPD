import sys
import os
import glob
import time
# to run type 'python splitter.py <inpdir> <outdir>' (folder from the root)
def main():
	initialpath = os.getcwd()
	os.system('g++ -Wall separateMixRun.C -o separate `root-config --cflags --libs`')
        pathtofile = sys.argv[1]
	os.chdir(sys.argv[1])
	for file in glob.glob('*.root'):
		os.chdir(initialpath)
		print './separate '+file
                print os.getcwd()
                
		os.system('./separate '+os.path.abspath(pathtofile+file)+' '+sys.argv[2])
		#time.sleep()

if __name__ == '__main__':
	main()
