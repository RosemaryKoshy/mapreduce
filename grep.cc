#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "mapreduce.hh"

using namespace std;
using namespace MapReduce;

void map(const char *filename) {
    FILE *stream = fopen(filename, "r");
    assert(stream != nullptr);

    char *line = nullptr;
    size_t len = 0;
    while (getline(&line, &len, stream) > -1) {
        if (strstr(line, "needle")) {
            MR_Emit(line, "1");
        }
    }

    free(line);
    fclose(stream);
}

void reduce(const string &key, getter_t get_next, int partition_number) {
    while (!(get_next(key, partition_number)).empty()) std::cout << key;
}

int main(int argc, char *argv[]) {
    MR_Run(argc, argv, map, 10, reduce, 10, MR_DefaultHashPartition);
}
