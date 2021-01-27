# Frequent-Itemset-Mining
**Problem Statement**
Kindly find the detailed problem statement and the expected usage of the programs as clearly described in the document **hw1.pdf**. The programs written by us follow the same structure.

**Members**
  - Vedant Raval
  - Mohammad Kamal Ashraf

**Files Submitted**

  - main.cpp : main file for running the fp-tree or apriori. Give command as : ./run input_file X -fptree/-apriori outputfile
  - Apriori_handler.h : header file for Apriori functions class
  - Apriori_handler.cpp : imlpementation for Apriori
  - FP_Tree_handler.h : header file for FP_Tree functions and tree node
  - FP_Tree_handler.cpp : imlpementation for Apriori
  - compile.sh : generates a single executable named run
  - 2017CS10589.sh : shell script for all commands as required
  - Plot.py: generates the plots and saves in a file named "running_times_comp.png". This script will generate plots for the minimum support values as [1,5,10,25,50,90]. Note that because of the apriori algorithm being very slow, we couldn't show the results for lower thresholds  

**Explanation for Part-b**

 - Apriori needs k dataset accesses to generate k-frequent itemsets while FP-Tree needs only two dataset access. This increases running time drastically for Apriori as with an increase in k the number of frequent itemsets increases exponentially
- Apriori is like a breadth first search algorithm, while FP-Tree is a depth first algorithm so it behaves better by early pruning of paths that would not give frequent itemsets as in this process it is aided by the ordering in terms of single itemsets frequencies
- Also with decrease in minsup, the number of frequent itemsets increases drastically, and so both algorithms take more time. However since Apriori requires larger number of database accesses it becomes increasingly slow
 
**Running Times (in seconds)**

Since the running times very extremely large for smaller values for Apriori, those are not being included in the graphs

| Minsup %        | Apriori           | FP-Tree  |
| ------------- |:-------------:| -----:|
| 10      | 40278.1 | 1894.4 |
| 25      | 217.318      |   44.7453 |
| 50 | 94.1686      |    35.727 |
| 90 | 74.7968      |    29.7487 |

The first plot below corresponds to the min sup values being [25,50,90], while the other graph also includes the results for min support as 10

![Alt text](plots/running_times_comp_more_than_10.png?raw=true "Apriori vs FP-Tree")
![Alt text](plots/running_times_comp_trunc.png?raw=true "Apriori vs FP-Tree")

