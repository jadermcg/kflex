#include "ksearch.h"
#include "utils.h"
#include "smt.h"

//'Search a specific kmer.
//'@name ksearch.
//'@param fasta Dataset of sequences.
//'@param query Query kmer.
//'@param prefix_size Size of prefix.
//@return A frequency of kmer.
uint64_t ksearch(const std::vector<std::string> &fasta, const std::string query, const int &prefix_size) {
    int k = query.size();
    std::string prefix{query.substr(0, prefix_size)}; 
    std::string sufix{query.substr(prefix_size, k - prefix_size)};
    std::vector<uint64_t> bin = read_bin(prefix);
    
    auto n{bin.size()};
    auto t{fasta[0].size()};

    size_t number_of_lines, number_of_cols;
    std::tie(number_of_lines, number_of_cols) = get_dimensions(n, t, k, prefix_size);

    const auto *M = createDenseMT(fasta, k, bin, prefix_size, number_of_lines, number_of_cols);
    
    auto node {0};
    for (const auto &c : sufix) {
        const auto &col{ char2int(c) };
        auto next{ M[node * 6 + col] };
        if (next == 0) break;
        node = next;
    }

    uint64_t count = M[node * 6 + 4];

    delete [] M;

    return count;
}