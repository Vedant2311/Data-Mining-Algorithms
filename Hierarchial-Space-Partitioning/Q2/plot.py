import sys
import numpy as np
import matplotlib
matplotlib.use('Agg')
from matplotlib import pyplot as plt

def plot_part_a():
    data = np.loadtxt('rtree_1')
    x = data[:,1].tolist()
    y = data[:,3].tolist()
    data = np.loadtxt('lsh_1')
    y1 = data[:,3].tolist()
    plt.plot(x,y,'r',x,y1,'g')
    plt.xlabel("Dimensions")
    plt.ylabel("Average Number of inspections")
    plt.title("Dimension vs Inspections for 1-NN on whole Dataset")
    plt.legend(['R-Tree','LSH'])
    plt.savefig("Q2_a_1.png")
    plt.clf()

    # Running Times
    data = np.loadtxt('rtree_1')
    y = data[:,4].tolist()
    data = np.loadtxt('lsh_1')
    y1 = data[:,4].tolist()
    plt.plot(x,y,'r',x,y1,'g')
    plt.xlabel("Dimensions")
    plt.ylabel("Average Time per Query (milliseconds)")
    plt.title("Running Time for 1-NN on whole Dataset")
    plt.legend(['R-Tree','LSH'])
    plt.savefig("Q2_a_2.png")
    plt.clf()

def plot_part_b():
    data = np.loadtxt('rtree_2')
    x = (data[:4,0] / 1000000).tolist()
    y1 = data[:4,3].tolist()
    y2 = data[4:,3].tolist()
    data = np.loadtxt('lsh_2')
    y3 = data[:4,3].tolist()
    y4 = data[4:,3].tolist()
    plt.plot(x,y1,'r',x,y2,'g',x,y3,'b',x,y4,'y')
    plt.xlabel("Dataset Size (million points)")
    plt.ylabel("Average Number of inspections")
    plt.title("Dataset Size vs Inspections for 1-NN")
    plt.legend(['R-Tree - 2','R-Tree - 8','LSH - 2','LSH - 8'])
    plt.savefig("Q2_b_1.png")
    plt.clf()

    # Running Times
    data = np.loadtxt('rtree_2')
    y1 = data[:4,4].tolist()
    y2 = data[4:,4].tolist()
    data = np.loadtxt('lsh_2')
    y3 = data[:4,4].tolist()
    y4 = data[4:,4].tolist()
    plt.plot(x,y1,'r',x,y2,'g',x,y3,'b',x,y4,'y')
    plt.xlabel("Dataset Size (million points)")
    plt.ylabel("Average Time per Query (milliseconds)")
    plt.title("Dataset Size vs Running Time for 1-NN")
    plt.legend(['R-Tree - 2','R-Tree - 8','LSH - 2','LSH - 8'])
    plt.savefig("Q2_b_2.png")
    plt.clf()
    
def plot_part_c():
    data = np.loadtxt('rtree_3')
    x = data[:5,2].tolist()
    y1 = data[:5,3].tolist()
    y2 = data[5:,3].tolist()
    data = np.loadtxt('lsh_3')
    y3 = data[:5,3].tolist()
    y4 = data[5:,3].tolist()
    plt.plot(x,y1,'r',x,y2,'g',x,y3,'b',x,y4,'y')
    plt.xlabel("k value for k-NN query")
    plt.ylabel("Average Number of inspections")
    plt.title("k vs Inspections for k-NN")
    plt.legend(['R-Tree - 2','R-Tree - 8','LSH - 2','LSH - 8'])
    plt.savefig("Q2_c_1.png")
    plt.clf()

    # Running Times
    data = np.loadtxt('rtree_3')
    y1 = data[:5,4].tolist()
    y2 = data[5:,4].tolist()
    data = np.loadtxt('lsh_3')
    y3 = data[:5,4].tolist()
    y4 = data[5:,4].tolist()
    plt.plot(x,y1,'r',x,y2,'g',x,y3,'b',x,y4,'y')
    plt.xlabel("k value for k-NN query")
    plt.ylabel("Average Time per Query (milliseconds)")
    plt.title("k vs Running Time for k-NN")
    plt.legend(['R-Tree - 2','R-Tree - 8','LSH - 2','LSH - 8'])
    plt.savefig("Q2_c_2.png")
    plt.clf()

def main():    
    if len(sys.argv) < 2:
        plot_part_a()
        plot_part_b()
        plot_part_c()
    elif (int(sys.argv[1]) == 1) :
        plot_part_a()
    elif (int(sys.argv[1]) == 2) :
        plot_part_b()
    elif (int(sys.argv[1]) == 3) :
        plot_part_c()

main()