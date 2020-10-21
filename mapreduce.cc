/**
 * mapreduce.cc
 * Talib Pierson
 * 20 October 2020
 */
#include "mapreduce.hh"

void MapReduce::MR_Emit(const std::string &key, const std::string &value) {}

unsigned long MapReduce::MR_DefaultHashPartition(const std::string &key,
                                                 int num_partitions) {
    return 0;
}

void MapReduce::MR_Run(int argc, char *argv[], MapReduce::mapper_t map,
                       int num_mappers, MapReduce::reducer_t reduce,
                       int num_reducers, MapReduce::partitioner_t partition) {}
