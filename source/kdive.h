#pragma once
#include <string>
#include <tbb/parallel_for.h>
#include <tbb/concurrent_hash_map.h>
#include <tbb/concurrent_unordered_map.h>
#include <cmath>

void kdive(const std::string &query, const int &prefix_size, const int &d, const std::vector<std::string> &fasta);