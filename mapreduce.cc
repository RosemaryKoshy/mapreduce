#include "mapreduce.hh"

#include <future>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace MapReduce;
int num_part = 0;
// Use part[partkey][key].emplace_back(value)
using PART =
std::vector<std::unordered_map<std::string, std::vector<std::string>>>;
PART parts;
std::queue<std::future<void>> threadQueue;

std::string get_next(const std::string &key, int partition_number) {
    // if value match found for key then return it

    // so we have all the partitions and we need a string
    // we'll somehow traverse through in sorted order?
    // which may involve some c++ sort method

    // else return empty string (value not found for key)
    return "";
}

void deleteVal(const std::string &key, int part_num) {
    parts[part_num].at(key).erase(parts[part_num].at(key).begin());
}

unsigned long partitioner(const std::string &key, int num_partitions) {
    return MR_DefaultHashPartition(key, num_partitions);
}

std::string getter(const std::string &key, int part_num) {
    if (!parts[part_num].at(key).empty()) {
        std::string value = parts[part_num].at(key).front();
        deleteVal(key, part_num);
        return value;
    }
    return "";
}


void MapReduce::MR_Run(int argc, char *argv[], MapReduce::mapper_t map,
                       int num_mappers, MapReduce::reducer_t reduce,
                       int num_reducers, MapReduce::partitioner_t partition) {
    for(int i = 0; i < argc; i++){
        if((int)threadQueue.size() >= num_mappers){
            threadQueue.front().get();
            threadQueue.pop();
        }
        threadQueue.emplace(std::async(std::launch::async, map, argv[i]));
    }
    while(!threadQueue.empty()){
        threadQueue.front().get();
        threadQueue.pop();
    }

    std::vector<std::string> keys;
    for(int i = 0; i < (int)parts.size(); i++) {
        for (auto key: parts[i]){
            keys.emplace_back(key.first);
        }
    }

    for(int i = 0; i < num_part; i++){
        if((int)threadQueue.size() >= num_reducers){
            threadQueue.front().get();
            threadQueue.pop();
        }
        threadQueue.emplace(std::async(std::launch::async, reduce, keys[i], getter, partition(keys[i], num_part)));
    }

    while(!threadQueue.empty()){
        threadQueue.front().get();
        threadQueue.pop();
    }

}

void MapReduce::MR_Emit(const std::string &key, const std::string &value) {
    num_part++;
    unsigned long partKey = partitioner(key, num_part);
    int exists = parts[partKey].count(key);
    if (exists > 0){
        parts[partKey][key].emplace_back(value);
    }
    else{
        auto insert = std::pair<std::string, std::vector<std::string>>(key, {value});
        parts[partKey].insert(insert);
    }
}

unsigned long MapReduce::MR_DefaultHashPartition(const std::string &key,
                                                 int num_partitions) {
    unsigned long hash = std::hash<std::string>{}(key);
    return hash % num_partitions;
}