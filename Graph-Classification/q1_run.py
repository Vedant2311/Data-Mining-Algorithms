##### Description: Runs the given data file (converted) on the corresponding algorithm and plots the graph
##### Assumpion: The input file converted and the extracted folder for the algorithm are present in the same directory as this code 
### Usage: 		$ python3 q1_run.py <file_name> <method_name>
### Example: 	$ python3 q1_run.py aido99_all.txt fsg
import os
import sys
import time
import matplotlib.pyplot as plt

### Taking inputs from command line
### file_name -> Name of the file to be converted
### method_name -> Name of the method which will be used. Available options: ['gspan','fsg','gaston']
file_name = sys.argv[1]
method_name = sys.argv[2]

## Naming the output file corresponding to each method
base_name = file_name.replace('.txt','')
converted_name = base_name + '_' + method_name + '.txt'

## The list for all the required minsup (in percentage) 
minsup_list = [5.0,10.0,25.0,50.0,95.0]

## The list for all the time values
output_list = []

## The total number of graphs present
total_graphs = 42682

## Running the programs on all the methods and storing the times
for freq in minsup_list:

	if method_name=='gspan':
		cmd = './gSpan6/gSpan -f ' + converted_name + ' -s ' + str(freq/100.0) + ' -o'
	elif method_name=='fsg':
		cmd = './pafi-1.0.1/Linux/fsg -s ' + str(freq) + ' ' + converted_name
	elif method_name=='gaston':
		cmd = './gaston-1.1-re/gaston ' + str((freq * total_graphs)/100.0) + ' ' + converted_name
	else:
		print("Available options for method_name are: " + '[\'gspan\',\'fsg\',\'gaston\']')
		break
	
	# Measuring the time of the program
	begin = time.time()
	os.system(cmd)
	end = time.time()
	
	# Appending the difference in the output
	output_list.append(end-begin)
	print(freq, end-begin)

'''
## Making the respective plots
plt.plot(minsup_list, output_list)
plt.title('Plot for the running times for: ' + method_name)
plt.xlabel('Support Thresholds (in percentage)')
plt.ylabel('Running time (in seconds)')
plt.savefig('time_' + converted_name.replace('.txt','') + '.png')
'''
