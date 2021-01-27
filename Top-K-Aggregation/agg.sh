### Usage: As specified in the problem statement
file_name=$1;
k=$2;
output_seq="seq.txt";
output_ta="ta.txt";

## Run the program for sequential algorithm and threshold algorithm
## Assumes that their particular python codes are in the same directory
python3 sequential.py $file_name $2 $output_seq
python3 threshold.py $file_name $2 $output_ta
