module load compiler/gcc/7.4.0/compilervars
module load apps/anaconda/3EnvCreation
mkdir Libs
cd Libs

# Making libspatialindex
curl -L http://download.osgeo.org/libspatialindex/spatialindex-src-1.8.5.tar.gz | tar xz
libdir="${PWD}"
echo "${libdir}"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${libdir}/lib
cd spatialindex-src-1.8.5
./configure --prefix=${libdir}
make
make install

cd ..
# Cloning falconn-lib. This is header only library so no need to make
git clone https://github.com/FALCONN-LIB/falconn.git
cd ..

g++ -std=c++11 -O3 -o rtree rtree_runner.cpp -I${libdir}/include -L${libdir}/lib -lspatialindex_c -lspatialindex
g++ -std=c++11 -O3 -o lsh lsh_runner.cpp -march=native -I${libdir}/falconn/src/include -I${libdir}/falconn/external/eigen -I${libdir}/falconn/external/simple-serializer -pthread
