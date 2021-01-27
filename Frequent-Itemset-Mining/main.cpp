#include "Apriori_handler.h"
#include "FP_Tree_handler.h"

using namespace std;
int main(int argc, char** argv) {
    if(argc < 5)
        cout<<"Error in number of command line inputs"<<endl;
    string infilename = argv[1];
    string outfilename = argv[4];
    string supp_per = argv[2];
    float support_percent =  stof(supp_per);
    string mode = argv[3];
    auto start = chrono::high_resolution_clock::now();
    if(mode.compare("-fptree") == 0) {
        FP_Tree_handler dm = FP_Tree_handler(support_percent,infilename,outfilename);
        dm.run_fp_tree();
    } else {
        Apriori_handler dm = Apriori_handler(support_percent,infilename,outfilename);
        dm.run_apriori();
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);
	cout<<"Duration: "<<duration.count()/(1000000.0)<<" seconds"<<endl;
    return 0;
}

