#pragma once
#include <tbb/concurrent_unordered_map.h>
#include <tbb/concurrent_queue.h>
#include<tbb/parallel_for.h>
#include<tbb/parallel_reduce.h>
#include "utils.h"
#include "smt.h"
#include <cmath>
#include <fstream>

void process_and_count(const std::vector<std::string> &fasta, const uint &number_of_bins, const uint &k, const uint &prefix_size);
