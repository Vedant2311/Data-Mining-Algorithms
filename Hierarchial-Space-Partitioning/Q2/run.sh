libdir="${PWD}"/Libs/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${libdir}
./rtree 1
./rtree 2
./rtree 3
./lsh 1
./lsh 2
./lsh 3
python plot.py