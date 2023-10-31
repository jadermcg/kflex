#pragma once
#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <tbb/concurrent_unordered_map.h>
#include <tbb/parallel_for.h>
#include <tbb/tbb.h>


void createBins(const std::vector<std::string> &fasta, const int &k, const int &prefix_size);
uint64_t* createDenseMT(const std::vector<std::string> &fasta, const int &k, const std::vector<size_t> &bin, const int &prefix_size, const size_t &number_of_lines, const size_t &number_of_cols);
