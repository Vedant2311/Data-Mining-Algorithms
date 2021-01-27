import random
import matplotlib
matplotlib.use('Agg')
from matplotlib import pyplot as plt
import math 
import numpy as np
from statistics import mean 
import sys

## Defining the ranges of the values used by us
total_points = 10**6
dim_range = [1,2,4,8,16,32,64]
batch = 100

## Generating the random points. Indexed by math.log2(dim)
random_all = []
for dim in dim_range:
	random_dim = []
	for i in range(total_points):
		random_point = [random.random() for _ in range(dim)]
		random_dim.append(random_point)
	random_all.append(random_dim)
	
## Generating the values of all the distances for all the dimensions, after taking the samples
d1_list = []
d2_list = []
dinfi_list = []
for dim in dim_range:
	print(int(math.log2(dim)))
	list_all = random_all[int(math.log2(dim))]
	list_sampled = random.sample(list_all, batch)	
	
	d1_list_individual=[]
	d2_list_individual=[]
	dinfi_list_individual=[]

	for i in range(batch):
		
		temp_list_d1 = []
		temp_list_d2 = []
		temp_list_dinfi = []
		
		for j in range(total_points):	
		
			point_1 = np.array(list_sampled[i])
			point_2 = np.array(list_all[j])
			
			if np.array_equal(point_1,point_2):
				continue
			
			d1 = np.linalg.norm((point_1 - point_2), ord=1)
			d2 = np.linalg.norm((point_1 - point_2), ord=2)
			dinfi = np.linalg.norm((point_1 - point_2), ord=np.inf)
			
			temp_list_d1.append(d1)
			temp_list_d2.append(d2)
			temp_list_dinfi.append(dinfi)
			
		min_d1 = min(temp_list_d1)
		max_d1 = max(temp_list_d1)

		min_d2 = min(temp_list_d2)
		max_d2 = max(temp_list_d2)

		min_dinfi = min(temp_list_dinfi)
		max_dinfi = max(temp_list_dinfi)
		
		d1_list_individual.append(max_d1*1./min_d1)
		d2_list_individual.append(max_d2*1./min_d2)
		dinfi_list_individual.append(max_dinfi*1./min_dinfi)
		
	## Getting the minimum and maximum values in these lists 
	d1_list.append(mean(d1_list_individual))
	d2_list.append(mean(d2_list_individual))
	dinfi_list.append(mean(dinfi_list_individual))
	
# Making the plots and showing/storing them
plt.plot(dim_range,d1_list,'r',dim_range,d2_list,'b', dim_range, dinfi_list, 'g')
plt.xlabel('Dimensions of the points')
plt.ylabel('Average ratio of max to min distance')
plt.legend(['L1 dist','L2 dist','Linf dist'])

if sys.argv[1]=='log':
	plt.yscale('log')
	plt.title('Plot of average ratio v/s dimensions (Logarithmic y-scale)')
	plt.savefig('q1_plot_log.png')
else:
	plt.title('Plot of average ratio v/s dimensions (Normal y-scale)')
	plt.savefig('q1_plot.png')
