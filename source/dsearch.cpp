#include "dsearch.h"


//'Kmer direct Search.
//'@name busca_direta.
//'@param fasta Dataset of sequences.
//'@param kmers kmers to find sibligs.
//'@param d Number of mutations allowed.
//'@return C++ String HashMap of siblings of kmers.
std::map<std::string, uint64_t> dsearch(std::vector<std::string> &fasta, std::vector<std::string> &kmers) {
  int n = fasta.size();
  int t = fasta[0].size();
  int k = kmers[0].size();
  int m = t - k + 1;
  std::map<std::string, uint64_t> hmap;

  for (const auto &kmer : kmers) {
    for (auto i{0}; i < n; ++i) {
        const auto &seq{fasta[i]};
        for (auto j{0}; j < m; ++j) {
            const auto &km{seq.substr(j, k)};
            if (kmer == km) {
                ++hmap[kmer];
            }
        }
    }
  }

  return hmap;
}