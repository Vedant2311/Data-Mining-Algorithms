##### Description: Dumps the output of top-k aggregation query using sequential scan
##### Assumpion: The input file is present in the same directory as this code 
### Usage: 		$ python3 sequential.py <file_name> <k>
### Example: 	$ python3 sequential.py file0 10
import os
import sys
from heapq import heapify, heappush, heappop 
import time

### Taking inputs from command line
### file_name -> Name of the file to be scanned
### k -> The value of k
file_name = sys.argv[1]
k = int(sys.argv[2])

if len(sys.argv)==3:
	output_name = file_name.replace('.txt','') + '_seq' + '_' + str(k) 
else:
	output_name = sys.argv[3]

## Opening the file for reading
file_obj = open(file_name,'r')
file_lines = file_obj.readlines()
file_obj.close()

## Getting the list of (x,y) tuples
tuple_list = []
for line in file_lines:
	vals = line.split(' ')
	x = float(vals[0])
	y = float(vals[1].replace('\n',''))
	tuple_list.append((x+y,x,y))
	
## Starting the time now
start = time.time()

## Defining an empty heap 
heap = []
heapify(heap)

## For adding the first k elements to the min heap
count = 0
for (z,x,y) in tuple_list:
	if count<k:	
		heappush(heap,(z,x,y))
		count = count + 1
	else:
		(c,a,b) = heap[0]
		
		## If the lowest element of the heap is smaller than the current element then replace it
		if c < z:
			temp = heappop(heap)
			heappush(heap, (z,x,y))

## Ending the time count
end = time.time()
total_time = end - start

## Heap invariant is that it will have the top-k elements. Now saving them to a file 
output_obj = open(output_name,'w')

## Writing the time taken in seconds
output_obj.write(str(total_time) + '\n')

## Wrting the top-k points in ascending order
for i in range(k):
	(z,x,y) = heap[0]
	line_write = str(x) + ' ' + str(y)
	output_obj.write(line_write + '\n')
	temp = heappop(heap)
	
output_obj.close()
