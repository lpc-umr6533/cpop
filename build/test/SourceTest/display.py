#Print 3D points stored in a file

import sys
import numpy as np
from matplotlib import pyplot
from mpl_toolkits.mplot3d import Axes3D


def main(filename):
	
	points = np.loadtxt(filename)

	fig = pyplot.figure()
	ax = Axes3D(fig)

	ax.scatter(points[:,0], points[:,1], points[:,2])
	pyplot.show()



if __name__ == "__main__":
	if len(sys.argv) == 2:
		main(sys.argv[1])
	else:
		print("Error no input file")
			
