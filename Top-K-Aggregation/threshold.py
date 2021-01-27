##### Description: Dumps the output of top-k aggregation query using sequential scan
##### Assumpion: The input file is present in the same directory as this code 
### Usage: 		$ python3 threshold.py <file_name> <k>
### Example: 	$ python3 threshold.py file0 10
import os
import sys
from heapq import heapify, heappush, heappop 
import time
import copy

### Taking inputs from command line
### file_name -> Name of the file to be scanned
### k -> The value of k
file_name = sys.argv[1]
k = int(sys.argv[2])

if len(sys.argv)==3:
	output_name = file_name.replace('.txt','') + '_thr' + '_' + str(k) 
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
	
## Getting the sorting of these lists done for the threshold algorithm
x_list = copy.deepcopy(tuple_list)
y_list = copy.deepcopy(tuple_list)

## Sorting them in descending order
x_list = sorted(x_list, key = lambda tup: tup[1], reverse = True)
y_list = sorted(y_list, key = lambda tup: tup[2], reverse = True)

## Starting the time now
start = time.time()

## Defining an empty heap 
heap = []
heapify(heap)

## For adding the first k elements to the min heap. Implementing threshold algorithm
count = 0
for (xi,yi) in zip(x_list,y_list):
	## Storing the scores and other informations
	score_x, x_val, y_temp = xi
	score_y, x_temp, y_val = yi
	
	## Storing the max bound
	max_bound = x_val + y_val
	
	## Having different conditions on the values of count
	if count<k-1 and not (xi==yi):
		## Checking if repeated earlier or not
		if xi not in heap:
			heappush(heap, xi)
			count = count+1
		if yi not in heap:
			heappush(heap, yi)
			count = count + 1
		
	elif count < k-1 and (xi==yi):
		if xi not in heap:
			heappush(heap, yi)
			count = count + 1
		
	elif count<k:
		if score_x>score_y and xi not in heap:
			heappush(heap, xi)
			count = count+1
		elif yi not in heap:
			heappush(heap, yi)
			count = count + 1
		
	else:
		## Getting the minimum element of the heap
		(c,a,b) = heap[0]
		
		## If the score of minimum is higher than threshold, then terminate
		if c > max_bound:
			break
		else:
			### Check if the two tuples are identical or not
			if not xi==yi:
				## Replace the minimum elements by higher values
				if c < score_x and xi not in heap:
					temp = heappop(heap)
					heappush(heap, xi)
				
				(c,a,b) = heap[0]
			
				if c < score_y and yi not in heap:
					temp = heappop(heap)
					heappush(heap, yi)
				
			else:
				## Replace the minimum elements by higher values
				if c < score_x and xi not in heap:
					temp = heappop(heap)
					heappush(heap, xi)
				
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
