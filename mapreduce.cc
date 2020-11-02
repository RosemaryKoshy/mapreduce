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

int num_part;
//Use part[partkey][key].emplace_back(value)
using PART = std::vector<std::unordered_map<std::string, std::vector<std::string>>>;
PART parts;


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


