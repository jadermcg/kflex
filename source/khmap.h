#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <tbb/parallel_for.h>
#include <tbb/concurrent_unordered_map.h>

void khmap(const std::vector<std::string> &fasta, const int &prefix_size, const int &k, const int &kmax);