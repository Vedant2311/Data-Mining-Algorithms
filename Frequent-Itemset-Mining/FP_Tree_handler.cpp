#include "FP_Tree_handler.h"

// Custom function for sorting tuple in descending order
bool sort_tuple_descending(const tuple<int, int>& a, const tuple<int, int>& b) { 
    return (get<0>(a) > get<0>(b)); 
}

void FP_Tree_handler::insert(node* root, std::vector <tuple<int,int>> v_items){
    node* crawl = root;

    for (int i=0; i<v_items.size();i++){

        // Getting the second element of the tupe
        int index = get<1>(v_items[i]);
       
        // Child not present, so we make one
        if (crawl->children.find(index) == crawl->children.end()){
            node* temp = new node(index,1,crawl);
            crawl->children[index] = temp;

            // In O(1) time update the header_list
            // However one disadvantage is that there is no deinfed ordering
            // If it were we could have utilized this someway
            temp->link = header_list[index_map[index]];
            header_list[index_map[index]] = temp;
        }
        else{
            crawl->children[index]->conditioned_counts.back() += 1;
        }
        crawl = crawl->children[index];
    }
    // Marking the end node as a leaf as done in normal prefix trees
    crawl->end = true;
}


FP_Tree_handler::FP_Tree_handler(float supp, string in_filename, string out_file_name) {
    support_percent = supp;
    num_transactions = 0;
    max_item = -1;
    max_transaction_len = 0;
    k = 1;
    data_file_name = in_filename;
    output_file_name = out_file_name + ".txt";
    root = new node(-1,-1,NULL);
    initialize_transactions(in_filename);
    outfile.open(output_file_name);
    initialize_tree(in_filename);
}

void FP_Tree_handler::initialize_transactions(string filename) {
    ifstream infile(filename);
    if(!infile){
        cout<<"Unable to open file"<<endl;
        exit(0);
    }
    string nextLine = "";
    int size_of_buffer = 8192;
    char *sr = (char*) malloc(sizeof(char)*size_of_buffer);
    char *endS;
    char *itr;
    while(getline(infile,nextLine)){
        num_transactions += 1;
        int item;
        int len = 0;
        while(nextLine.length()  > size_of_buffer){
            size_of_buffer *=2;
            sr = (char *)realloc(sr,sizeof(char)*size_of_buffer);
        }
        strcpy(sr,nextLine.c_str());
        itr = sr;
        item = strtol(itr,&endS,10);
        while(itr != endS)
        {
            // len++;
            items_frequency[item]++;
            // max_item = max(max_item,item);
            itr = endS;
            item = strtol(itr,&endS,10);
        }
        // max_transaction_len = max(max_transaction_len,len);
    }
    num_sup = (int) (num_transactions * 0.01 * support_percent);

    // Generating the index that maps an item to the number of it in descending order of frequency
    vector<tuple<int,int>> for_sorting;
    for(auto it = items_frequency.begin() ; it != items_frequency.end() ; it++)
        if( it->second >= num_sup)
            for_sorting.push_back(make_tuple(it->second,it->first));
    
    // Sorting by frequency and then inserting the indexes in index map
    sort(for_sorting.begin(),for_sorting.end(),sort_tuple_descending);
    for(int i = 0; i < for_sorting.size() ; ++i)
        index_map[get<1>(for_sorting[i])] = i;
    
    // Initializing all headers to be NULL
    header_list.assign(for_sorting.size() , NULL);
}


void FP_Tree_handler::initialize_tree(string filename){
    ifstream infile(filename);
    if(!infile){
        cout<<"Unable to open file"<<endl;
        exit(0);
    }
    string nextLine = "";
    
    int size_of_buffer = 8192;
    char *sr = (char*) malloc(sizeof(char)*size_of_buffer);
    char *endS;
    char *itr;
    while(getline(infile,nextLine)){
        num_transactions += 1;
        int item;
        // Getting the tuple for the items and their individual counts
        std::vector<tuple<int,int>> v_items;
        while(nextLine.length()  > size_of_buffer)
            {
                size_of_buffer *=2;
                sr = (char *)realloc(sr,sizeof(char)*size_of_buffer);
            }
        strcpy(sr,nextLine.c_str());
        itr = sr;
        item = strtol(itr,&endS,10);
        
        while(itr != endS)
        {
            if(index_map.find(item) != index_map.end())
                v_items.push_back(make_tuple(items_frequency[item],item));
            itr = endS;
            item = strtol(itr,&endS,10);
        }
        // Sorting them by the First value
        sort(v_items.begin(), v_items.end(), sort_tuple_descending); 

        // Inserting the vector to the root node
        insert(root, v_items);
    }
}

void FP_Tree_handler::condition_on(node *item_start, vector<int> previous_items){
    if(!item_start) {
        cout<<"PANIC : Itemstart NULL"<<endl;
        exit(0);
    }
    int item = item_start->key;
    int mapped_index = index_map[item]; 
    node *item_node = item_start;
    // track support for current item in the conditional tree
    int count_of_current_conditioner = 0;
    int num_paths = 0;
    int temp_count;

    if(previous_items.empty()) {
        previous_items.push_back(item);
    }
    
    // Determine the support count for item pointed by item_start type node in the current conditional tree
    node * if_single = NULL;
    while(item_node) { 
        // Skipping the nodes not part of the current conditional tree
        if((item_node->conditioned_on.back() != previous_items.back())) {
            item_node = item_node->link;
            continue;
        }
        count_of_current_conditioner += item_node->conditioned_counts.back(); 
        ++num_paths;
        if(!if_single)
            if_single = item_node;
        item_node = item_node->link;
    }
    // Return as the current item cannot be part of any frequent superset
    if(count_of_current_conditioner < num_sup) {
       return;
    }

    item_node = if_single;
    // Iterate over all nodes, find nodes that are part of current conditional tree, and the climb_tree for those nodes
    // to further generate a smaller conditional tree
    while(item_node) { 
        if((item_node->conditioned_on.back() != previous_items.back())) {
            item_node = item_node->link;
            continue;
        }            
        temp_count = item_node->conditioned_counts.back();
        for(node *ancestor = item_node->parent ; ancestor->key > -1 ; ancestor = ancestor->parent)
            {
                if(ancestor->conditioned_on.back() != item)
                    { 
                        ancestor->conditioned_on.push_back(item);
                        ancestor->conditioned_counts.push_back(temp_count);
                    }
                else
                {
                    ancestor->conditioned_counts.back() += temp_count;
                }
            }
        item_node = item_node->link;
    }

    if(previous_items.back() != item)
        previous_items.push_back(item);

    // Recursively evalute for other conditional items
    if(num_paths > 1) {
        // Since prevoious_items is also frequent print it
        print_itemset(previous_items);
        // Recursively condition upon higher elements in the headdr list 
        for(int upper_item = mapped_index-1 ; upper_item >=0 ; --upper_item) {
            condition_on(header_list[upper_item],previous_items);
        }
    }
    else if(num_paths == 1) {
        // Determine the single node that has the path
        if(if_single) {
            print_all_patterns(if_single,previous_items);
        }
    }

    // Remove all the conditional values in the upper tree
    if(previous_items.size() > 1)
        previous_items.pop_back();

    item_node = if_single;
    while(item_node)  { 
        if((item_node->conditioned_on.back() != previous_items.back())) {
            item_node = item_node->link;
            continue;
        }    
        for(node *ancestor = item_node->parent ; ancestor->key > -1 ; ancestor = ancestor->parent)
            {
                if(ancestor->conditioned_on.back() == item) {
                    ancestor->conditioned_counts.pop_back();
                    ancestor->conditioned_on.pop_back();
                }
            }
        item_node = item_node->link;
    }

    return;
}


/*
    Generates all subsets for items refered by ancestor nodes of lower_end
    Concatenate them with previous_patterns and then print them
*/

// See if this can be further improved
// Right now this uses a set of vector<int> to keep track of differet subsets
void FP_Tree_handler::print_all_patterns(node *lower_end, vector<int> previous_patterns) {
    vector<int> upper_tree;
    node *ptr = lower_end->parent;
    
    // Determine all items in upper tree
    while(ptr && ptr->key > -1 ) {
        upper_tree.push_back(ptr->key);
        ptr = ptr->parent;
    }
    
    set<vector<int>> subsets;
    vector<int> empty;
    subsets.insert(empty);
    for(int i = 0; i <upper_tree.size() ; i++)
        {
            set<vector<int>>  temp = subsets;

            for(auto r : temp)
                r.push_back(upper_tree[i]);
            for(auto r : temp)
                subsets.insert(r);

        }
    for(auto r : subsets) {
        r.insert(r.end(), previous_patterns.begin(), previous_patterns.end());
        vector<string> p;
        for(auto item : r)
            p.push_back(to_string(item));
        sort(p.begin(),p.end());
        string s = p[0];
        int k = p.size();
        for(int i = 1; i < k  ; ++i)
            s += " " + p[i];
        outfile<<s<<endl;
    }
}


// Iterate over all the elements in the header list from bottom and apply fp_growth recursively
void FP_Tree_handler::run_fp_tree() {
    vector<int> empty;
    for(int j = header_list.size() -1 ; j >= 0 ; --j)
        {
            // Just an empty pattern to be passed initially
            condition_on(header_list[j],empty);
        }
}

// Print a single itemset after sorting ot the target file
void FP_Tree_handler::print_itemset(vector<int> itemset) {
    vector<string> p;
    for(auto r : itemset)
        p.push_back(to_string(r));
    sort(p.begin(),p.end());
    string s = p[0];
    int k = p.size();
    for(int i = 1; i < k  ; ++i)
            s += " " + p[i];
    outfile<<s<<endl;
}