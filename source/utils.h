#pragma once
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <tuple>
#include <numeric>

int char2int(char c);
char int2char(int i);
uint64_t kmer2index(const std::string &kmer);
std::string index2kmer(uint64_t index, int k);
std::vector<uint64_t> read_bin(const std::string &prefix);
int hDist(const std::string &str1, const std::string &str2);
std::vector<std::string> readkmers(const std::string &path);
std::vector<std::string> readFasta(const std::string& filepath);
std::vector<std::string> getFilenames(const std::string& dirPath);
std::unordered_map<std::string, uint64_t> readhmap(const std::string &filename);
std::tuple<size_t, size_t> get_dimensions(const size_t &n, const size_t &t, const size_t &k, const size_t &prefix_size);