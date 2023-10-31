#include "smt.h"
#include "utils.h"

//'Creates bins in disk by prefix of kmers.
//'@name createBins.
//'@param fasta The Dataset of sequences.
//'@param k The Size of kmers.
//'@param prefix_size Length of prefix.
void createBins(const std::vector<std::string> &fasta, const int &k, const int &prefix_size) {
  const auto n{ fasta.size() };
  const auto t{ fasta[0].size() };
  const auto m { t - k + 1 };
  const auto number_of_bins {static_cast<size_t>(std::pow(4, prefix_size))};

  tbb::concurrent_unordered_map<std::string, tbb::concurrent_vector<uint64_t>> bins;
  bins.reserve(number_of_bins);

  // Copy kmers into a prefix bin
  tbb::blocked_range<size_t> range(0, n);
  tbb::parallel_for(range, [&](const auto &r) {
      for (auto i{r.begin()}; i < r.end(); ++i) {
          const auto &seq { fasta[i] };
          for (auto j{0}; j < m; ++j) {
              const auto &prefix{ seq.substr(j, prefix_size) };
              bins[prefix].push_back(i * m + j);
          }
      }
  });

  // Save bins in disk
  auto ret = std::system("rm -Rf flex_data");
  ret = std::system("mkdir -p flex_data/bins");
  range = tbb::blocked_range<size_t>(0, number_of_bins);
  tbb::parallel_for(range, [&](const auto &r) {
      for (size_t i{r.begin()}; i < r.end(); ++i) {
          auto prefix { index2kmer(i, prefix_size) };
          std::vector<uint64_t> bin(bins[prefix].begin(),bins[prefix].end());
          
          std::string path = "flex_data/bins/" + prefix;
          FILE *fp = std::fopen(path.data(), "wb");
          std::fwrite(&bin.at(0), sizeof(uint64_t), bin.size(), fp);
          std::fclose(fp);
      }
  });
}

//'Creates Dense matrix from the sequences with direct memory allocation.
//'@name createDenseMT.
//'@param fasta The Dataset of sequences.
//'@param k The Size of kmers.
//'@param bin The bin of kmers to processes.
//'@return C++ tree/matrix of kmers.
uint64_t* createDenseMT(const std::vector<std::string> &fasta, const int &k, const std::vector<size_t> &bin, const int &prefix_size, const size_t &number_of_lines, const size_t &number_of_cols) {
  auto m{fasta[0].size() - k + 1};
  auto next {0};
  auto *MT = new uint64_t[number_of_lines * number_of_cols]();
 
  for (const auto &address : bin) {
    const auto &line {address / m};
    const auto &col {address % m};
    const auto &sufix = fasta[line].substr(col + prefix_size, k - prefix_size);
   
    uint64_t node = 0;
     
    for (const auto &c : sufix) {
      auto symbol {char2int(c)};
      auto current { MT[node * 6 + symbol]};
      
      if (current == 0) {
        next += 1;
        MT[node*6 + symbol] = next;
        node = next;
      }
      
      else {
        node = current;
      }
    }
     
     MT[node*6 + 4] += 1;
     MT[node*6 + 5] = address;
   
 }
 
 return MT;
}
