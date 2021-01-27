# Top-K-Aggregation

## Members
	1. Vedant Raval
	
## Running the System
First of all, run the binary **corr.pyc** to generate three different dataset files; named file0, file1, and file2. Then the script **agg.sh** will be used to solve the top-k aggregation query for all these three different 2D datasets created.

Script Usage: 
```
sh agg.sh <filename> <k>
Output format: should generate two files
		1. ta.txt: line 1 should contain the time taken and the rest of the lines should contain each of the top-k points in ascending order of their aggregation score. One point per line.
		2. seq.txt: same format as ta.txt. Needless to say, that you should ensure that the top-k scores for both sequential and threshold algorithm are the same. 
```

You can find the following different programs here:
	1. sequential.py => Takes the dataset and K (for top-k) are input and dumps the top-k values in a file using the sequential algorithm. Used python heap library
	2. threshold.py => Takes the dataset and K (for top-k) are input and dumps the top-k values in a file using the Threshold algorithm. Used python heap library
	3. agg.sh => Computes the required files are stated in the problem statement with the same usage

All the information regarding usage, functionality, and directory assumptions are present in these codes. 

## Studying these algorithms
We plotted the growth rate of querying times with k at [1,10,50,200,500, 5000] on each of the datasets for both algorithms. For threshold algorithm, we do not include the time taken to sort items based on each dimension value. Also, for both sequential scan and threshold algorithm we do not include the time taken to dump the answer set to the file. You can find the corresponding plots for these different files in the various png files, named appropriately. The explanations corresponding to these plots can be found in explanations.pdf file
