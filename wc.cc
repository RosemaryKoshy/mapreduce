// To build:
// g++ -Wall -Wextra -pedantic --std=c++17 -g -pthread mapreduce.cc wc.cc -o wc

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "mapreduce.hh"

using namespace std;
using namespace MapReduce;

static void map(const char *filename) {
    FILE *stream = fopen(filename, "r");
    assert(stream != nullptr);

    char *line = nullptr;
    size_t len = 0;
    while (getline(&line, &len, stream) > -1) {
        /// set key and str to line in stream
        char *key, *str = line;
        // while word in line not end of line
        while ((key = strsep(&str, " \t\n\r")) != nullptr) {
            MR_Emit(key, "1");
        }
    }

    free(line);
    fclose(stream);
}

static void reduce(const string &key, getter_t get_next, int partition_number) {
    int count = 0;
    // for all values that produced the same key
    for (string value; !(value = get_next(key, partition_number)).empty();) {
        count++;
    }
    std::cout << key << ' ' << count << std::endl;
}

int main(int argc, char *argv[]) {
    MR_Run(argc, argv, map, 10, reduce, 10, MR_DefaultHashPartition);
}
