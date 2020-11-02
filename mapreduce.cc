/**
 * mapreduce.cc
 * Talib Pierson
 * 20 October 2020
 */
#include <string>
#include <queue>
#include <future>
#include <thread>
#include "mapreduce.hh"
using namespace MapReduce;
int num_part;
//Use part[partkey][key].emplace_back(value)
using PART = std::vector<std::unordered_map<std::string, std::vector<std::string>>>;
PART parts;

void deleteVal(const std::string& key, int part_num){
    parts[part_num].at(key).erase(parts[part_num].at(key).begin());
}
unsigned long partitioner(const std::string &key, int num_partitions){
    unsigned long hash = std::hash<std::string>{}(key);
    return hash % num_partitions;
}
std::string get(const std::string& key, int part_num){
    if(!parts[part_num].at(key).empty()){
        std::string value = parts[part_num].at(key).front();
        deleteVal(key, part_num);
        return value;
    }
    return "";
}


reducer_t reduce(const std::string &key, getter_t getter, int partition_number){

    return MapReduce::reducer_t();
}

void MapReduce::MR_Run(int argc, char *argv[], MapReduce::mapper_t map,
                       int num_mappers, MapReduce::reducer_t reduce,
                       int num_reducers, MapReduce::partitioner_t partition) {
    std::queue<std::future<PART>> threadQueue;
    num_part = num_reducers;
    parts.resize(num_part);


}
void MapReduce::MR_Emit(const std::string &key, const std::string &value) {

}

unsigned long MapReduce::MR_DefaultHashPartition(const std::string &key,
                                                 int num_partitions) {
    return 0;
}


