import sys, os, subprocess
import matplotlib.pyplot as plt

# Input file name here
inpname = sys.argv[1]

# Creating a loop for running the program iteratively
sup_list = [1,5,10,25,50,90]
apriori_list = []
fp_list = []

for x in sup_list:	
	# Storing the results for Apriori implementation
	cmd_ap = "./run " + inpname + " " + str(x) + " -apriori out_plot_ap.txt"
	result_ap = subprocess.check_output(cmd_ap, shell=True)
	result_ap = float(result_ap.split()[1])
	apriori_list.append(result_ap)
	
	# Storing the results for Apriori implementation
	cmd_fp = "./run " + inpname + " " + str(x) + " -fptree out_plot_fp.txt"
	result_fp = subprocess.check_output(cmd_fp, shell=True)
	result_fp = float(result_fp.split()[1])
	fp_list.append(result_fp)

# Making the plots and showing/storing them
plt.plot(sup_list,apriori_list,'r',sup_list,fp_list,'b')
plt.xlabel('Support Thresholds (in percentage)')
plt.ylabel('Running time (in seconds)')
plt.legend(['Apriori','FP-tree'])
plt.savefig('running_times_comp.png')
