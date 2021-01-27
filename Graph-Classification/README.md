# Graph-Classification

## Members
	1. Vedant Raval

## Problem statement
Three different frequent subgraph mining tools were ran against the dataset of molecules tested against AIDS, [aido99_all](https://onedrive.live.com/?cid=ef92560ae8680184&id=EF92560AE8680184%2152239&authkey=%21ALSDv1I9NtOHDyU). The format being the following:
```
#graphID
number of nodes
Series of Node Labels
number of edges
Series of “Source node, Destination Node, Edge label”
```

Run gSpan (https://sites.cs.ucsb.edu/~xyan/software/gSpan.htm), FSG (http://glaros.dtc.umn.edu/gkhome/pafi/download), and Gaston (http://liacs.leidenuniv.nl/~nijssensgr/gaston/download.html) against frequency threshold in the AIDS dataset (a script would be needed to change the format of the dataset) at minSup = 5%, 10%, 25%, 50% and 95%. Plot the running times and explain the trend observed in the running times. Also suggest mechanisms through which the number of patterns can be reduced without losing too much information. Discuss why your suggestion is effective.

## Running the scripts
First of all, setup all these three different mining tools in your system as would be stated in their respective sites.

There are two codes attached here:
	1. convert_input.py => Converts the input dataset in the required format
	2. q1_run.py => Runs all the different algorithms for the given threshold values

You can find the informations, directory assumptions and usage instructions regarding all the codes included in the folder. Kindly go through the codes and follow the instructions written in them.

## Plots and explanations
You can find the plot for these running times to be included as a png file. The explanations for all these trends can be found in **explanations.pdf** file

## Suggestions for reducing the number of patterns
Those suggestions along with their explanations can be found in **suggestions.pdf**
