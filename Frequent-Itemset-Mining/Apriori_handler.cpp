#include "Apriori_handler.h"

Apriori_handler::Apriori_handler(float supp, string in_filename, string out_file_name) {
    support_percent = supp;
    num_transactions = 0;
    max_item = -1;
    max_transaction_len = 0;
    k = 1;
    data_file_name = in_filename;
    output_file_name = out_file_name  + ".txt";
    initialize_transactions(in_filename);
}

void Apriori_handler::initialize_transactions(string filename) {
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
            items_frequency[item]++;
            itr = endS;
            item = strtol(itr,&endS,10);
        }
    }
    num_sup = (int) (num_transactions * 0.01 * support_percent);

    for(auto it = items_frequency.begin(); it!= items_frequency.end() ;){
        if(it->second < num_sup)
            items_frequency.erase(it++);
        else
            {
                vector<int> s = {it->first};
                target_set.push_back(s);
                ++it;
            }
    }
}

int Apriori_handler::compare_cust(int i, int j){
    int k_size = target_set[i].size() - 1;
    int ret = 0;
    if(target_set[i][k_size] < target_set[j][k_size]) {
        ret = -1;
    } else if(target_set[i][k_size] > target_set[j][k_size])
    {
        ret = 1;
    } else
    {
        return 0;
    }

    for(int k = target_set[i].size() - 2 ; k >=0 ; --k ){
        if(target_set[i][k] != target_set[j][k])
            return 0;
    }
    return ret;
}

bool Apriori_handler::apply_apriori_property(vector<int> candidate) {
    bool is_infrequent = true;
    for(auto it_candidate = candidate.begin() ; it_candidate != candidate.end() ; it_candidate++){
        int removed_item = *it_candidate;
        candidate.erase(it_candidate);
        for(int i = 0; i < target_set.size() ; ++i){
            int j = 0 ;
            for(; j < candidate.size() ; j++)
                if(candidate[j] != target_set[i][j])
                    break;
            if( j == candidate.size())
                {
                    is_infrequent = false;
                    break;
                }
            is_infrequent = true;
        }
        if(is_infrequent)
            return false;
        candidate.insert(it_candidate,removed_item);
    }
    return true;
}

void Apriori_handler::generate_candidate_sets(){
    candidate_set.clear();
    for(int i = 0; i < target_set.size() ; ++i) {
        for(int j = i+1; j < target_set.size() ; ++j) {
            int merging_way = compare_cust(i,j);
            if(merging_way == -1) {
                vector<int> v(target_set[i]);
                v.push_back(target_set[j][k-1]);
                if(apply_apriori_property(v))
                    candidate_set.push_back(v);
            } else if(merging_way == 1) {
                vector<int> v(target_set[j]);
                v.push_back(target_set[i][k-1]);
                if(apply_apriori_property(v))
                    candidate_set.push_back(v);
            }
        }
    }
    // cout<<"Candidate Set Size : "<<candidate_set.size()<<endl;
}

void Apriori_handler::determine_target_itemsets() {
    target_set.clear();
    items_frequency.clear();

    // read through the dataset to determine the number count of candidate itemsets
    ifstream infile(data_file_name);
    string nextLine;
    int item;
    int size_of_buffer = 8192;
    char *sr = (char*) malloc(sizeof(char)*size_of_buffer);
    char *endS;
    char *itr;
    while(getline(infile,nextLine)) {
        // stringstream ss(s);
        vector<int> v;
        while(nextLine.length()  > size_of_buffer){
            size_of_buffer *=2;
            sr = (char *)realloc(sr,sizeof(char)*size_of_buffer);
        }
        strcpy(sr,nextLine.c_str());
        itr = sr;
        item = strtol(itr,&endS,10);
        while(itr != endS)
        {
            v.push_back(item);
            itr = endS;
            item = strtol(itr,&endS,10);
        }
        // while(ss>>item) {
        //     v.push_back(item);
        // }
        sort(v.begin(),v.end());
        vector<int> comp(k);
        for(int i = 0; i < candidate_set.size() ; i++) { 
            auto it =  set_intersection(candidate_set[i].begin(),candidate_set[i].end(), v.begin(),v.end(), comp.begin());
            if((it-comp.begin()) == k)
                items_frequency[i] += 1;
        }
    }
    infile.close();
    
    // transfer all candidate itemsets to target on the basis of the support counts
    for(int i = 0 ; i < candidate_set.size() ; ++i)
        if(items_frequency[i]  >= num_sup)
            target_set.push_back(candidate_set[i]);

}

void Apriori_handler::write_target_itemsets(bool first_write) {
    if(target_set.size() <= 0)
        return;
    ofstream outfile;
    int x = 0;
    if(first_write) {
        outfile.open(output_file_name);
        vector<string> p;
        for(auto r : target_set[x])
            p.push_back(to_string(r));
        sort(p.begin(),p.end());
        string s = p[0];
        for(int i = 1; i < k ; ++i)
            s += " " + p[i];
        outfile<<s<<endl;
        ++x;
    } else {
        outfile.open(output_file_name,ofstream::app);
    }
    while(x < target_set.size()) {
        vector<string> p;
        for(auto r : target_set[x])
            p.push_back(to_string(r));
        sort(p.begin(),p.end());
        string s = p[0];
        for(int i = 1; i < k ; ++i)
            s += " " + p[i];outfile<<s<<endl;
        ++x;
    }
    outfile.close();
}

void Apriori_handler::run_apriori() {
    write_target_itemsets(true);
    while(true) {
        // cout<<"K value : "<<k<<endl;
        generate_candidate_sets();
        if(candidate_set.size() <= 0)
            return;
        ++k;
        determine_target_itemsets();
        // cout<<"Target Set size : "<<target_set.size()<<endl;
        if(target_set.size() <= 0)
            return;
        write_target_itemsets(false);
    }
}