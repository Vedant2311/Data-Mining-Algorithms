/*
 * Usage: $ ./lsh_q2 <file_name> <num_queries> <num_hash_tables> <num_hash_functions> <precision>
 */

#include <falconn/lsh_nn_table.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <queue>
#include <fstream>
#include <cstdio>
#include <tuple>

using namespace std;
using namespace std::chrono;
using namespace falconn;

typedef DenseVector<float> Point;

const int NUM_HASH_BITS = 20;
int NUM_ROTATIONS = 1;

void read_dataset(string infileName, vector<Point> *dataset,int dimensions) {
    ifstream infile(infileName);
    string s;
    char* buffer = (char *) malloc(sizeof(char)*15000);
    Point p;
    char *start;
    char *end;
    int j=0;
    p.resize(dimensions);
    while (getline(infile,s)) {
        if(s.empty() || (s.length() == 0))
            break;
        strcpy(buffer,s.c_str());
        start = buffer;
        j = 0;
        p[j++] = strtod(start,&end);
        while(j < dimensions && (start != end)) {
            start = end;
            p[j++] = strtod(start,&end);
        } 
        dataset->push_back(p);
    } 
    free(buffer);
}
/*
 * Normalizes the dataset.
 */
void normalize(vector<Point> *dataset) {
  for (auto &p : *dataset) {
    p.normalize();
  }
}

/*
 * Generates answers for the queries using the (optimized) linear scan.
 */
void gen_answers(const vector<Point> &dataset, const vector<Point> &queries, vector<vector<int>> *answers, int k) {
// TO DO: Here "answers" is a 1D vector corresponding to the index of the points in dataset found with the minumum Euclidean distance w.r.t the point in Query 
// TO DO: So for having k-NN, have it as a 2D vector with the dimensions of (k * queries.size())
  // answers->resize(queries.size());
  int outer_counter = 0;
  for (const auto &query : queries) {
    float best = INT_MAX;
    int inner_counter = 0;
    //
    answers->push_back(vector<int>(k,-1));
    priority_queue<pair<float,int>> pq;
    int track_k = 0;
    
    for (const auto &datapoint : dataset) {
      float score = (float) (query - datapoint).norm();
      // Checking for the case where the score gets 0
      if(track_k < (k) && score!=0) {
        (*answers)[outer_counter][track_k] = inner_counter;
        pq.push(make_pair(score,track_k++));
      } else if ((track_k == k) &&score < pq.top().first && score!=0) {
        int replacement_index = pq.top().second;
        (*answers)[outer_counter][replacement_index] = inner_counter;
        pq.pop();
        pq.push(make_pair(score,replacement_index));
      }
      ++inner_counter;
    }
    ++outer_counter;
  }
}


/*
 * Queries the data structure using a given number of probes.
 * It is much slower than 'evaluate_num_probes' and should be used to
 * measure the time.
 */
tuple<double, double,QueryStatistics> evaluate_query_time(LSHNearestNeighborTable<Point> *table, const vector<Point> &queries,const vector<vector<int>> &answers, int num_probes,int k, float precision) {
    int num_matches = 0;
	int len =0;
	unique_ptr<LSHNearestNeighborQuery<Point>> query_object;
	double total_time = 0;
  int outer_counter;
  int tries = 0;
	do{
    tries++;
	  query_object = table->construct_query_object(num_probes);
	  query_object->reset_query_statistics();
	  outer_counter = 0;
	  num_matches = 0;
      total_time = 0;
    vector<int> results(k,0);
	  for (const auto &query : queries) {
        auto start_time = chrono::high_resolution_clock::now();
        query_object->find_k_nearest_neighbors(query,k,&results);
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time-start_time);
        total_time += duration.count()/(1000.0);
        for(int i=0 ; i< k ; ++i) {
            for(int j = 0 ; j < k ; ++j) {
                if(results[i] == answers[outer_counter][j]){
                    num_matches++;
                    break;
                }
          }
    }
		++outer_counter;
	  }
	  num_probes = 2*num_probes;
	  len = queries.size();
    if(tries == 10)
      break;
	 } while (((num_matches + 0.0) / (len*k + 0.0)) < precision);
	 return make_tuple((num_matches + 0.0) / (queries.size()*k + 0.0), total_time, query_object->get_query_statistics());
}

void LSH_create_and_query(string dataFileName, string queryFile, int num_hash_tables,int num_hash_functions,float precision,int dimensions, int k,ofstream &outfile) {
  try {
  vector<Point> dataset, queries;
  vector<vector<int>> answers;

	// Reading the command line arguments
    // read the dataset
    read_dataset(dataFileName,&dataset,dimensions);

    // normalize the data points
    normalize(&dataset);
    // find the center of mass
    Point center = dataset[0];
    for (size_t i = 1; i < dataset.size(); ++i) {
      center += dataset[i];
    }
    center /= dataset.size();

    // selecting NUM_QUERIES data points as queries
	  read_dataset(queryFile, &queries,dimensions);

    // re-centering the data to make it more isotropic
    for (auto &datapoint : dataset) {
      datapoint -= center;
    }
    for (auto &query : queries) {
      query -= center;
    }
    
    // running the linear scan
    gen_answers(dataset, queries, &answers,k);
    		
    // setting parameters and constructing the table
    LSHConstructionParameters params;
    params.dimension = dataset[0].size();
    params.lsh_family = LSHFamily::CrossPolytope;
    params.l = num_hash_tables;
    params.k = num_hash_functions;
    params.distance_function = DistanceFunction::EuclideanSquared;
    compute_number_of_hash_functions<Point>(NUM_HASH_BITS, &params);
    params.num_rotations = NUM_ROTATIONS;
    params.num_setup_threads = 0;
    params.storage_hash_table = StorageHashTable::BitPackedFlatHashTable;

	  // Building the LSH from the dataset 
    auto table = construct_table<Point>(dataset, params);

    // finding the number of probes via the binary search
	  int num_probes = num_hash_tables;

    // executing the queries using the found number of probes to gather statistic 
    auto tmp = evaluate_query_time(&*table, queries, answers, num_probes,k, precision);
    auto score = get<0>(tmp);
    auto average_time = get<1>(tmp);    
    auto statistics = get<2>(tmp);   
    outfile<<(dataset.size() + 100)<<" "<<dimensions<<" "<<" "<<k<<" "<<(statistics.average_num_unique_candidates + 0.0)/100<<" "<<(average_time)/100<<endl; 
    cout<<(dataset.size() + 100)<<" "<<dimensions<<" "<<" "<<k<<" "<<(statistics.average_num_unique_candidates + 0.0)/100<<" "<<(average_time)/100<<endl; 
  } catch (runtime_error &e) {
    cerr << "Runtime error: " << e.what() << endl;
  } catch (exception &e) {
    cerr << "Exception: " << e.what() << endl;
  } catch (...) {
    cerr << "ERROR" << endl;
  }
}
