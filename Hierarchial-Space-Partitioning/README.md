# Hierarchial-Space-Partitioning
## Problem Statement
Kindly find the detailed problem statement and the expected usage of the programs as clearly described in the document **hw2.pdf**. The programs written by us follow the same structure.

## Members
  - Vedant Raval
  - Mohammad Kamal Ashraf

# 1. Running the programs and setting up the system
## Running Q1 scripts
- Assuming that this README file is present in the cwd, run the following commands: 
``` Bash
$ cd Q1
$ python3 q1.py log
$ python3 q1.py normal
$ cd ..
```
- The command line argument of *log* above means that the plots will be generated in logarithmic scale. Otherwise, they will be generated in a normal scale

## Running Q2 scripts
### Setting Up Libraries and Compiling Code
- Assuming that this README file is present in the cwd, run the following commands that will setup the HPC environment, clone the libraries and install them in a local folder:
``` Bash
$ cd Q2
$ sh setup.sh
$ python gen_files.py <path_to_dataset>
```
- This will generates different files in the system for simplicity in runnning the 3 parts. The different file name conventions are as follows:
    - The file name of **2_pca** specifies that it is the file having 1 million datapoints but reduced to two dimensions by the PCA technique.
    - The file name of **2_pca_test** specifies that a test set of 100 points corresponding to two dimensions was taken from the **2_pca** dataset
    - The file name of **2_10000** specifies that a subset of 10000 points was chosen from the dataset, reduced to two dimensions
    - The file name of **2_10000_test** specifies that a test set of 100 points correspoding to two dimensions was taken from the **2_10000** dataset
- Note that these different test sets generated will actually be disjoint from the considered dataset.

### Running and Generating Plots
- The following command will run the files and generate required plots. The results file will be generated as "rtree_partno"/"lsh_partno" where partno is 1,2 and 3 for each of the three parts
``` Bash
$ sh run.sh
```
### Generating Data Files
```Bash
$ python gen_files.py <path to dataset>
```
### Plotting
- This assumes that all reults file generted by the binaries are already present in the current directry:
```Bash
$ python plot.py
```
- This command will generate all plots and save them in current directory named as Q2_a_1.png, Q2_a_2.png, and so on. For each part the first plot is for number of inspections and the second one is for the running time for queries. To generate plots for a single part use commands `python plot.py 1` for part-a and similary argument 2 or 3 for other parts.
### Cleaning temporary data files
```Bash
$ sh clean.sh
```
### Re-compiling
```Bash
$ sh compile.sh
```

### Running the Individual Parts
#### R-Tree
- Script setup.sh or compile.sh will generate binary for R-Tree named `rtree`.
- Note that for running binary for R-Tree follwing library path must be set
``` Bash
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${PWD}/Libs/lib
$ ./rtree partno
$ ./rtree 0 datasetFile queryFile resultFile dimension k_in_k-nn
```
- Where partno is 1,2 or 3 for the three parts of Q2. This will generate file 'rtree_partno' with required statistics for plotting for corresponding part
- The second command is a lower level command to run for different datasets
- Note that we do not store the resulting k-nearest neighbours

#### LSH
- Script setup.sh or compile.sh will generate binary for LSH named `lsh`. Usage:
``` Bash
$ ./lsh partno
$ ./lsh 0 datasetFile queryFile reultFile dimensions k_in_k-nn num_hash_tables num_hash_functions precision
```
- Where partno is 1,2 or 3 for the three parts of Q2. This will generate file 'lsh_partno' with required statistics for plotting for corresponding part
- The second command is a lower level command to run for different datasets
- Note that we do not store the resulting k-nearest neighbours


# 2. Results for the different parts
## Q1 Results and Explanations

#### Plots 
- The plots for this can be found in the **Plots** directory. One plot is made with a logarithmic scale for y-axis to show the results better. While the other plot is made with normal scales for both the axis. The file names can be inferred easily (They start with q1)

#### Observed Trends
- The results clearly show that with increasing dimension the ratio of farthest to nearest neighbour points decreases exponentially
- For metrics other than L2 we also see that similar trend in followed, but L2 ratio is lowest follwed by L1 while highest is for L-infinity norm.

#### Explanation
- As we increase the dimesnions, the data points become sparse. This is because the number of points remain the same while the volume increases and so the points become sparse. This in turn meams that earler we were having points near to each other but now on average the points are far away from each other
- For example consdier d be the dimensionality and c the total nnumber of points. Then in an hypercube of unit edge length, the number of points in a hyper sphere of radius r will be `(c*2r^d * π^d/2)/(Γ(d/2)) ` and the farthest distance between any two points can be `√d`. Now note that while the density decreases exponentially with d the farthest distance increases only as power of 1/2
- This in turn means that in the similar volume where there were earlier more points but now they are less in density while the ball radius remains constant. While on the same time, the growth in maximum possible distance in only sub-linear. This in turn explains the observed trends.


## Q2 Results and Explanation
### Part-A
#### Plots
- In the **Plots** directory, you can find the plots named *q2_a_1* (for dimensions vs inspection) and *q2_a_2* (for dimensions vs running time)
#### Observed Trends
- As we increase the dimensionality of data, a clear trend was increase in number of inspections and running time for R-Tree while for LSH both running time as well as the number of inspections decreased with the increase of dimensionality
#### Explanation
- In **R-tree** increase in running times is expected because with increase in dimensionality there is increase in computation cost for querying. We need to visit larger number of nodes because of increase in dimensionality as now with increase in dimensionality there will be more overlap in different MBR's so we need to search in more number of nodes to get the answers. This might be one reason as in higher dimensional data there are more chances of such overlap. This howver depends on the splitting startegy of the Tree construction
- In **LSH** the decrease in the number of inspections is expected because of the fact that with increase in dimensionality, the dara points become sparse. So for higher dimensions, these datapoints are farther away from each other. And the inspections (or the candidates, as used in LSH) correspond to the hash table collisions, which has higher probability for the points closer to each other. Thus for higher dimensions, these collisions will be less and thus LSH candidates will be less too. And since the algorithm has to go through lesser candidates now, the running time will also be lower. 
- Also for **LSH**, the running time for the case of 2 dimensions is lesser in our plots because while searching for the best probe size for the algorithm, we place a limit on the maximum number of probes allowed. This was done because in the case of 2-dimensional data the accuracy of 90% wasn't getting attained and a search for that led to the number of probes getting continuously increased without much change in the accuracy, while for data with dimensions like 4 etc. this issue doesn't arise and they reach this 90% accuracy soon (We start with the number of probes as the number of hash tables involved and increase the size of the probe till the accuracy obtained is atleast 90%). Thus, the running time for 2 dimensional dataset would actually be the highest. 
### Part-B
#### Plots
- In the **Q2/Plots** directory, you can find the plots named *q2_b_1* (for dataset-size vs inspection) and *q2_b_2* (for dataset-size vs running time)
#### Observed Trends
- In this part there were multiple observations. For R-Tree and LSH the running time as well as the inspections increased with increasing dataset size. 
- For, R-Tree both values were lower for 2 dimesnions than for 8 dimensional data. For LSH, there were higher for the 2 dimensional data.
#### Explanation
- For **R-tree** the first trend is directly explained as we increase the number of data points the size of tree will increase and so the searching cost will increase. Similarly values for 8 dimensional data are higher than 2-dimesional which has already been explanied for Part-a
- For **LSH** the first trend is directly explained as we increase the number of data points the size of tree will increase and so the searching cost will increase. Similarly values for 8 dimensional data are lower than 2-dimesional which has already been explanied for Part-a
### Part-C
#### Observed Trends
- For R-Tree the running time and inspections both increase with increse with increase in value of k for k-NN. Also, values for 8 dimensional data are higher than for 2-dimensional data. 
- For LSH, it is observed that the number of inspections kind of remain constant. While there is an increase in the running time in the case of the dimensions of the data being 8. For the dimensions being 2, there seems to be a weird trend and that can be affliated with the same reason as stated in part(a) that this happens because of having a bound on the number of probes in the program and thus the LSH system reaches that exact stage in the case of 2-dimensional data.
#### Explanation
- For **R-Tree** the increase in these values with increase in k is expected as in k-NN query we need a priority queue to keep track of the nodes. So we directly have an increase in the insertion cost beacuse of this and this explains the observed running times and number of inspections.
- For **LSH**, the number of inspections remain constant overall as the hash table construction isn't changed by changing this value of 'k'. And so, the number of inspections returned will be the same. But since we are search for k values now among those candidates, ideally the trend for the running will be that it will increase with increasing values of k and this is observed for the LSH algorithm on the top of 8 dimensional data. This is not the case for 2-dimensional data and that's because of the abrupt bound introducted by us because of resource shortage.
