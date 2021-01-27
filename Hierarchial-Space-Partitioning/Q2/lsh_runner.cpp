#include "iostream"
#include "string"
#include "cstring"
#include "sstream"
#include "vector"
#include "cstdlib"
#include <cstdlib>
#include <ctime>
#include <chrono>

#include "lsh_q2.cpp"


using namespace std;

int main(int argc,char **argv) {
    if (argc < 2) {
        cout<<"Specify the part 1, 2 or 3"<<endl;
        exit(0);
    }
    int mode = stoi(argv[1]);
    int num_hash_tables = 3;
    int num_hash_functions = 3;
    float precision = 0.9;

    if( mode == 1) {
        vector<string> files = {"2_pca","4_pca","8_pca","10_pca"};
        vector<int> dimensions = {2,4,8,10};
        ofstream outfile("lsh_1");
        for(int i= 0 ; i < 4 ; ++i) {
           LSH_create_and_query(files[i], files[i] +"_test", num_hash_tables,num_hash_functions,precision,dimensions[i],1,outfile);
        }
        outfile.close();
    } else if(mode == 2) {
        vector<int> subset_sizes = {1000,10000,100000,1000000};
        vector<int> dimensions = {2,8};
        ofstream outfile("lsh_2");
        for(auto d : dimensions) {
            for(auto s : subset_sizes) {
                LSH_create_and_query(to_string(d) + "_" + to_string(s),to_string(d) + "_" + to_string(s) + "_test", num_hash_tables,num_hash_functions,precision,d,1,outfile);
            }
        }
        outfile.close();
    } else if (mode == 3) {
        vector<int> k_values = {1,5,50,100,500};
        vector<int> dimensions = {2,8};
        ofstream outfile("lsh_3");
        for(auto d : dimensions) {
            for(auto k : k_values) {
                LSH_create_and_query(to_string(d) + "_100000",to_string(d) + "_100000_test", num_hash_tables,num_hash_functions,precision,d,k,outfile);
            }
        }
        outfile.close();
    } else if (mode == 0) {
        // ./lsh 0 datasetFile queryFile reultFile dimensions k num_hash_tables num_hash_functions precision
        string dataFile = (string) argv[2];
        string queryFile = (string) argv[3];
        string resultFile = (string) argv[4];
        int dimension = stoi(argv[5]);
        int k = stoi(argv[6]);
        int num_hash_tables = stoi(argv[7]);
        int num_hash_funtions = stoi(argv[8]);
        float precision  = stof(argv[9]);
        ofstream outfile(resultFile);
        LSH_create_and_query(dataFile,queryFile,num_hash_tables,num_hash_functions,precision,dimension,k,outfile);
        outfile.close();
    }
    return 0;
}