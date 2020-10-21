#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "mapreduce.hh"

static void count_words(const char *filename) {
    /* prepare the getline call */
    // open stream to file
    FILE *stream = fopen(filename, "r");
    if (stream == nullptr) {
        fprintf(stderr, "fopen: Couldn't open file %s: %s\n", filename,
                strerror(errno));
        exit(EXIT_FAILURE);
    }
    char *line = nullptr;  // getline will allocate memory
    size_t len = 0;        // no maximum line length

    /* for line in stream */
    while (getline(&line, &len, stream) > -1) {
        char *token;
        char *dummy = line;
        // we get the next whitespace-delimited token
        while ((token = strsep(&dummy, " \t\n\r")) != nullptr) {
            MapReduce::MR_Emit(token, "1");
        }
    }

    /* check stream for errors */
    if (ferror(stream)) {
        fprintf(stderr, "getline: Couldn't read file %s: %s\n", filename,
                strerror(errno));
        exit(EXIT_FAILURE);
    }

    free(line);      // free what getline allocated
    fclose(stream);  // stream to file
}

static void add_counts(std::string key, MapReduce::getter_t get_next,
                       int partition_number) {
    int count = 0;
    std::string value;
    while (!(value = get_next(key, partition_number)).empty()) count++;
    std::cout << key << ' ' << count << std::endl;
}

static unsigned long hash_partition(const std::string &key,
                                    unsigned long num_partitions) {
    std::hash<std::string> hash;
    return hash(key) % num_partitions;
}

int main(int argc, char *argv[]) {
    MapReduce::mapper_t map = count_words;
    int num_mappers = 10;
    MapReduce::reducer_t reduce = add_counts;
    int num_reducers = 10;
    MapReduce::partitioner_t partition = hash_partition;

    MapReduce::MR_Run(argc, argv, map, num_mappers, reduce, num_reducers,
                      partition);

    return EXIT_SUCCESS;
}
