##### Description: Converts the given data file into the format expected by different methods
##### Assumpion: The input file to be converted is present in the same directory as this code 
### Usage: 		$ python3 convert_input.py <file_name> <method_name>
### Example: 	$ python3 convert_input.py aido99_all.txt fsg
import os
import sys

### Taking inputs from command line
### file_name -> Name of the file to be converted
### method_name -> Name of the method which will be used. Available options: ['gspan','fsg','gaston']
file_name = sys.argv[1]
method_name = sys.argv[2]

## Naming the output file corresponding to each method
base_name = file_name.replace('.txt','')
output_name = base_name + '_' + method_name + '.txt'

## Opening the files for the processing
file_obj = open(file_name,'r')
file_lines = file_obj.readlines()
file_obj.close()

output_obj = open(output_name,'w')

if method_name=='gspan' or method_name=='gaston':
	## Flag to indicate whether the current value is the number of vertices or the number of edges
	## Also a count to indicate the count for a particular vertex or edge
	## Semantics: If 0 then vertices. Else Edges. Initially one since first flag-inversion should give zero
	flag = 1
	count = 0
	
	## A global count to indicate the current number of transaction
	count_global = 0
	
	# Storing a dictionary for mapping vertices to integers
	d_map = dict()
	vertex_count = 0
	
	for line in file_lines:
		
		# Corresponds to the graph id. Write 't' in output. And reset the flag and count. Increment the global count
		if line[0]=='#':
			output_obj.write('t # ' + str(count_global) + '\n')
			flag = 1
			count = 0
			count_global = count_global+1
		
		# Check if it is just having one number. Invert the flags and reset the counts
		elif line.replace('\n','').isdigit()==True:
			flag = (1-flag)
			count = 0
			
		# Insert vertices as: v <id> <label>
		elif flag==0:
			# Updating the dictionary
			word = line.replace('\n','')
			if word not in d_map:
				d_map[word] = vertex_count
				vertex_count = vertex_count+1
			
			line_vertex = 'v' + ' ' + str(count) + ' ' + str(d_map[word]) + '\n'
			count = count + 1
			output_obj.write(line_vertex)
		
		# Insert Edges as: u <id1> <id2> <label>
		elif flag==1:
			line_edge = 'e' + ' ' + line.replace('\n','') + '\n'
			output_obj.write(line_edge)
	
elif method_name=='fsg':
	## Flag to indicate whether the current value is the number of vertices or the number of edges
	## Also a count to indicate the count for a particular vertex or edge
	## Semantics: If 0 then vertices. Else Edges. Initially one since first flag-inversion should give zero
	flag = 1
	count = 0
	
	for line in file_lines:
		
		# Corresponds to the graph id. Write 't' in output. And reset the flag and count
		if line[0]=='#':
			output_obj.write('t\n')
			flag = 1
			count = 0
		
		# Check if it is just having one number. Invert the flags and reset the counts
		elif line.replace('\n','').isdigit()==True:
			flag = (1-flag)
			count = 0
			
		# Insert vertices as: v <id> <label>
		elif flag==0:
			line_vertex = 'v' + ' ' + str(count) + ' ' + line.replace('\n','') + '\n'
			count = count + 1
			output_obj.write(line_vertex)
		
		# Insert Edges as: u <id1> <id2> <label>
		elif flag==1:
			line_edge = 'u' + ' ' + line.replace('\n','') + '\n'
			output_obj.write(line_edge)
		
else:
	### Improper method name given
	print("Available options for method_name are: " + '[\'gspan\',\'fsg\',\'gaston\']')
	
## Closing the output file
output_obj.close()
