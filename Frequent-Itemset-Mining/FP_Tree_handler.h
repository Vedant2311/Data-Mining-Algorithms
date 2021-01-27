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
#include "cstring"

using namespace std;

// The root will have value and key as -1, which would act as Null here
// I have taken the number of children here as INT_MAX. Try to see if it can be optimised
class node{
public:
    int key;
    int value;
    /*
    Simpler approach to store childs
    One issue with this is that now their is no ordering as in left child right child
    However it would have been useful only if we were able to use that for header list but it was complicated
    */
    unordered_map<int, node *> children;
    node* parent;
    // Link to next node for same item
    node* link;
    bool end;
    // Conditioned counts are used for getting smaller trees without any need to make a separate tree
    // unordered_map<int,int> conditioned_counts;
    vector<int> conditioned_on;
    vector<int> conditioned_counts;

    node(int key, int value, node* parent){
        this->key = key;
        this->parent = parent;
        // this->conditioned_counts[key] = value;
        this->conditioned_on.clear();
        this->conditioned_counts.clear();
        this->conditioned_on.push_back(key);
        this->conditioned_counts.push_back(value);
        this->end = false;
        this->link = NULL;
    }
};

class FP_Tree_handler{
    public:
        string data_file_name;
        string output_file_name;
        unordered_map<int,int> items_frequency;
        int num_transactions;
        int max_item;
        int max_transaction_len;
        float support_percent;
        float num_sup;
        int k;
        node* root;
        // This is used to locate a node pointer for items
        vector<node *> header_list;
        // Map the item to an index based on frrquency
        unordered_map<int,int> index_map;
        ofstream outfile;

        FP_Tree_handler(float,string,string);
        void initialize_transactions(string filename);
        void initialize_tree(string filename);
        void insert (node* root, std::vector <tuple<int,int>> v_items);
        void condition_on(node *, vector<int>);
        void print_all_patterns(node *,vector<int>);
        void run_fp_tree();
        void print_itemset(vector<int>);
};