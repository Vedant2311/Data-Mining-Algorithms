#include "iostream"
#include "stdio.h"
#include "fstream"
#include "vector"
#include "string"
#include "sstream"
#include "map"
#include "unordered_map"
#include "unordered_set"
#include "limits"
#include "algorithm"
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <set>
#include <cstring>

using namespace std;

class Apriori_handler{
    public:
        string data_file_name;
        string output_file_name;
        map<int,int> items_frequency;
        int num_transactions;
        int max_item;
        int max_transaction_len;
        float support_percent;
        vector<vector<int>> target_set;
        vector<vector<int>> candidate_set;
        float num_sup;
        int k;

        Apriori_handler(float,string,string);
        void initialize_transactions(string filename);
        void generate_candidate_sets();
        int compare_cust(int,int);
        bool apply_apriori_property(vector<int>);
        int get_itemset_counts(vector<int>);
        void determine_target_itemsets();
        void write_target_itemsets(bool);
        void run_apriori();
};