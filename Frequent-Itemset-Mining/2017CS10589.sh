## Usage: 
# Apriori: ./2017CS10589.sh retail.dat X -apriori <filename>
# FP tree: ./2017CS10589.sh retail.dat X -fptree <filename>
# Plot: ./2017CS10589.sh retail.dat -plot

aprioriStr="-apriori";
fpStr="-fptree";
plotStr="-plot";

inputfile=$1;

# Check for plotting
secondarg=$2;
if [ "$secondarg" = "$plotStr" ];
then
	# Perform plotting. Runs the plot command
	python3 Plot.py $inputfile
else
	minsup=$2;
	outputfile=$4;
	method=$3
	
	# Run the program. Checks in main whether it's apriori or fptree
	./run $inputfile $minsup $method $outputfile;
fi

