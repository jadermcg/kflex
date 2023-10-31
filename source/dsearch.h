#pragma once
#include <map>
#include <string>
#include <vector>

std::map<std::string, uint64_t> dsearch(std::vector<std::string> &fasta, std::vector<std::string> &kmers);