#include "mapreduce.hh"

#include <future>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace MapReduce;
int num_part = 0;
//Basically a hashtable of maps
using PART =
std::vector<std::unordered_map<std::string, std::vector<std::string>>>;
PART parts;
std::queue<std::future<void>> threadQueue;
std::mutex locked;



//Returns the hash key from hash function
unsigned long partitioner(const std::string &key, int num_partitions) {
    return MR_DefaultHashPartition(key, num_partitions);
}
//gets the value and deletes it from vector inside the map
std::string getter(const std::string &key, int part_num) {
    if (!parts[part_num].at(key).empty()) {
        std::string value = parts[part_num].at(key).back();
        parts[part_num].at(key).pop_back();
        return value;
    }
    return "";
}

/**
 * @param argc          argument count (number of files + 1)
 * @param argv          argument array (caller + filenames array)
 * @param map           map function
 * @param num_mappers   mapper thread count
 * @param reduce        reduce function
 * @param num_reducers  reducer thread count
 * @param partition     partition function
 */
void MapReduce::MR_Run(int argc, char *argv[], MapReduce::mapper_t map,
                       int num_mappers, MapReduce::reducer_t reduce,
                       int num_reducers, MapReduce::partitioner_t partition) {
    //set vector size correctly so we can put in keys
    num_part = num_reducers;
    parts.resize(num_reducers);
    //loops through files and adds map function to thread queue, if threadqueue is bigger than num_maps pop and then continue.
    for(int i = 1; i < argc; i++){
        if ((int)threadQueue.size() >= num_mappers){
            threadQueue.front().get();
            threadQueue.pop();
        }
        threadQueue.emplace(std::async(std::launch::async, map, argv[i]));

    }
    //Run rest of map
    while(!threadQueue.empty()){
        threadQueue.front().get();
        threadQueue.pop();
    }
    //We need the keys so we get them here
    std::vector<std::string> keys;
    for(int i = 0; i < (int)parts.size(); i++) {
        for (auto key: parts[i]) {
            keys.emplace_back(key.first);
        }
    }
    //For each key run reduce
    for(int i = 0; i < (int)keys.size(); i++){
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
    //Get hash key for the key
    unsigned long partKey = partitioner(key, num_part);
    //We need to lock here until insert is done
    std::lock_guard<std::mutex> lock(locked);
    parts[partKey][key].emplace_back(value);
}
//hash function
unsigned long MapReduce::MR_DefaultHashPartition(const std::string &key,
                                                 int num_partitions) {
    unsigned long hash = std::hash<std::string>{}(key);
    return hash % num_partitions;
}