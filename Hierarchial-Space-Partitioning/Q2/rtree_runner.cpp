#include "iostream"
#include "string"
#include "cstring"
#include "sstream"
#include "vector"
#include "cstdlib"
#include <cstdlib>
#include <ctime>
#include <chrono>

#include "rtree.cpp"


using namespace std;

int main(int argc,char **argv) {
    string infile = "2_1000";
    string queryfile = "2_1000_test";
    if (argc < 2) {
        cout<<"Specify the part 1, 2 or 3"<<endl;
        exit(0);
    }
    int mode = stoi(argv[1]);
    if( mode == 1) {
        vector<string> files = {"2_pca","4_pca","8_pca","10_pca"};
        vector<int> dimensions = {2,4,8,10};
        ofstream outfile("rtree_1");
        for(int i= 0 ; i < 4 ; ++i) {
           rtree_create_and_query(dimensions[i],1,files[i],files[i]+ "_test",outfile);
        }
        outfile.close();
    } else if(mode == 2) {
        vector<int> subset_sizes = {1000,10000,100000,1000000};
        vector<int> dimensions = {2,8};
        ofstream outfile("rtree_2");
        for(auto d : dimensions) {
            for(auto s : subset_sizes) {
                rtree_create_and_query(d,1,to_string(d) + "_" + to_string(s),to_string(d) + "_" + to_string(s) + "_test",outfile);
            }
        }
        outfile.close();
    } else if (mode == 3) {
        vector<int> k_values = {1,5,50,100,500};
        vector<int> dimensions = {2,8};
        ofstream outfile("rtree_3");
        for(auto d : dimensions) {
            for(auto k : k_values) {
                rtree_create_and_query(d,k,to_string(d) + "_100000",to_string(d) + "_100000_test",outfile);
            }
        }
        outfile.close();
    } else if (mode == 0) {
        // ./rtree 0 datasetFile queryFile resultFile dimension k_in_k-nn
        string dataFile = (string) argv[2];
        string queryFile = (string) argv[3];
        string resultFile = (string) argv[4];
        int d = stoi(argv[5]);
        int k = stoi(argv[6]);
        ofstream outfile(resultFile);
        rtree_create_and_query(d,k,dataFile,queryFile,outfile);
        outfile.close();
    }
    
    
    return 0;
}