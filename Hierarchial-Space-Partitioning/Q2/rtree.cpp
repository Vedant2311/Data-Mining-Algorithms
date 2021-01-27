#include "iostream"
#include "string"
#include "cstring"
#include "sstream"
#include "vector"
#include "cstdlib"
#include <cstdlib>
#include <ctime>
#include <chrono>

#include "spatialindex/SpatialIndex.h"
#include "spatialindex/capi/sidx_api.h"
#include "spatialindex/capi/sidx_impl.h"
#include "spatialindex/capi/sidx_config.h"

using namespace SpatialIndex;
using namespace std;
using namespace RTree;
/*
Compilateion command:
g++ -o rtree rtree.cpp -lspatialindex_c -lspatialindex
*/

class CountingVisitor : public IVisitor {
	public:
		int index_Count{0};
		int leaf_Count{0};
        int result_count{0};
        vector<IShape*> results;
	
		CountingVisitor() {
			index_Count = 0;
			leaf_Count = 0;
            result_count = 0;
		}

		void visitNode(const INode& n) override {
            if (n.isLeaf()) 
                ++leaf_Count;
            else
                ++index_Count;
        }

        void visitData(const IData& d) override {
            
            // Done for testing purposes
            // /*
            IShape* pS;
            d.getShape(&pS);
            results.push_back(pS);
            /*
            Region *pp = dynamic_cast<Region*>(pS);
            cout<<"Node : "<<(pp->getLow(10))<<endl;
            delete pS;
            /*
            IData* item = dynamic_cast<IData*> (const_cast<IData&>(d).clone());
            results.push_back(item);
            */
            ++result_count;
        }

        void visitData(vector<const IData*>& v) override {
        }

};

class DataStreamReader : public IDataStream {
    public:
        Data* data_pointer;
        ifstream infileStream;
        
        char *buffer;
        int64_t id;
        int dimension;

        DataStreamReader(string infile,int dim) : data_pointer(nullptr) {
            infileStream.open(infile);
            data_pointer = nullptr;
            dimension = dim;
            if(!infileStream.good()) {
                cout<<"Errror Opening File"<<endl;
                exit(0);
            }
            id = 1;
            buffer = (char *) malloc(sizeof(char)*15000);
            ReadImage();

        }
        ~DataStreamReader() override {
            if (data_pointer!= nullptr ) delete data_pointer;
        }
        // Functions for interface
        IData* getNext() override  {
            if (data_pointer == nullptr) return nullptr;
            Data* ret = data_pointer;
            data_pointer = nullptr;
            ReadImage();
            return ret;
        }
        
        bool hasNext() override {
            return (data_pointer != nullptr);
        }

        uint32_t size() override {
            throw Tools::NotSupportedException("Size not implemented.");
        }

        void rewind() override {
            if(data_pointer != nullptr) {
                delete data_pointer;
                data_pointer = nullptr;
            }
            infileStream.seekg(0,ios::beg);
            ReadImage();
        }
        //
        void ReadImage() {
            string s;
            getline(infileStream,s);
            if(!(infileStream.good()) or s.empty() or( s.length() == 0)) {
                data_pointer = nullptr;
                return;
            }
            strcpy(buffer,s.c_str());
            char *start = buffer;
            char *end;
            int j = 0;
            double image_vector[dimension];
            double image_vector2[dimension];
            image_vector[j++] = strtod(start,&end);
            while( j < dimension) {
                start = end;
                image_vector[j++] = strtod(start,&end);
                image_vector2[j - 1] = image_vector[j-1];
            }
            Region r(image_vector,image_vector2,dimension);
            data_pointer = new Data(sizeof(double),reinterpret_cast<uint8_t*>(image_vector),r,id++);
        }
};

void rtree_create_and_query(int dimension,int k,string indexFile, string queryFile,ofstream& resultfile) {
    // Creating Libspatial Index Database
    DataStreamReader stream(indexFile,dimension);

    // Loading and modifying a default property set
    Tools::PropertySet* ps = GetDefaults();
    Tools::Variant var;
    var.m_varType = Tools::VT_ULONG;
    var.m_val.ulVal = RT_Memory;
    ps->setProperty("IndexStorageType",var);
    var.m_varType = Tools::VT_ULONG;
    var.m_val.ulVal = dimension;
    ps->setProperty("Dimension",var);

    IStorageManager* memoryFile = StorageManager::createNewMemoryStorageManager();
    StorageManager::IBuffer* file = StorageManager::returnRandomEvictionsBuffer(*memoryFile,*ps);
    id_type indexIdentifier;
    
    // Creating Tree by Bulk Loading
    ISpatialIndex* tree = createAndBulkLoadNewRTree(RTree::BLM_STR,stream,*file,0.7,25,25,dimension,RTree::RV_RSTAR,indexIdentifier);

    delete ps;

    if (!tree->isIndexValid()) {
        cout<<"Error Creating index"<<endl;
        exit(0);
    }
    
    // Reading query points and query 
    ifstream infile(queryFile);
    double vc[128];
    double d;
    string s;
    char *buffer = (char *) malloc(sizeof(char) * 10000);
    char *start;
    char *end;
    int j =0;

    double num_inspections = 0;
    double time = 0;

    // Timing measurements
    for (int i = 0; i < 100 ; ++i)
    {
        getline(infile,s);

        strcpy(buffer,s.c_str());
        char* start = buffer;
        vc[j++] = strtod(start,&end);
        j = 0;
        while (j < dimension) {
            start = end;
            vc[j++] = strtod(start,&end);    
        }
        CountingVisitor* vis = new CountingVisitor;
        Point* r = new Point(&vc[0],dimension);
        auto start_time = chrono::high_resolution_clock::now();
        tree->nearestNeighborQuery(k,*r,*vis);
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time-start_time);
        time += duration.count()/(1000.0);
        num_inspections += vis->index_Count + vis->leaf_Count;
    }
    
    resultfile<<(stream.id + 99)<<" "<<dimension<<" "<<" "<<k<<" "<<num_inspections/100<<" "<<time/100<<endl;
    delete(tree);
}