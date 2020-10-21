#include "mapreduce.cc"

int main() {
    int argc = 0;
    char **argv = nullptr;
    MapReduce::mapper_t map = MapReduce::mapper_t();
    int num_mappers = 0;
    MapReduce::reducer_t reduce = MapReduce::reducer_t();
    int num_reducers = 0;
    MapReduce::partitioner_t partition = MapReduce::partitioner_t();

    MapReduce::MR_Run(argc, argv, map, num_mappers, reduce, num_reducers,
                      partition);

    return EXIT_SUCCESS;
}
