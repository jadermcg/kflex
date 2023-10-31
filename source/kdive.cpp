#include "kdive.h"
#include "utils.h"
#include "smt.h"

//'Auxiliary function to save sibligs.
//'@name save.
void save(const std::string &query, const tbb::concurrent_unordered_map<std::string, uint64_t> &hmap, const int &i) {
    std::ofstream fp("flex_data/kdive_dir/" + query + "/" + std::to_string(i));

    // Save results
    for (const auto &pair : hmap) fp << pair.first << " " << pair.second << '\n';

    fp.close();
}

//'Auxiliary Recursive kdive function.
//'@name kdive_.
void kdive_(const std::string &prefix, const std::string &sufix, const uint64_t *MT, const int &d, const int &k, std::string kmer, tbb::concurrent_unordered_map<std::string, uint64_t> &hmap, int j, int l, uint64_t node) {
  
    if (j == k) {
        uint64_t freq{MT[node * 6 + 4]};
        auto result {hmap.emplace(prefix + kmer, 0)};
        result.first->second += freq;
        return;
    }
   
    for (auto i{0}; i < 4; ++i) {
        auto next {MT[node * 6 + i]};
        if (next != 0) {
            auto c {int2char(i)};
            auto hd {(sufix[j] == c) ? 0 : 1};
            if (l + hd <= d) {
                kdive_(prefix, sufix, MT, d, k, kmer + c, hmap, j + 1, l + hd, next);
            }
        }
    }
}

//'Recursive kdive function.
//'@name kdive.
void kdive(const std::string &query, const int &prefix_size, const int &d, const std::vector<std::string> &fasta) {

    // Parameters
    const auto k{query.size()};
    const auto t{fasta[0].size()};
    
    // Extract prefix and sufix from query string
    const std::string prefix{query.substr(0, prefix_size)};
    const std::string sufix{query.substr(prefix_size, k - prefix_size)};

    // Numbers of total bins
    auto number_of_bins{static_cast<int>(std::pow(4, prefix_size))};

    // Read bins and processing
    tbb::parallel_for(0, number_of_bins, 1, [&](const auto &i) {
        auto prefix_bin{index2kmer(i, prefix_size)};
        auto bin{read_bin(prefix_bin)};
        const auto hd{hDist(prefix, prefix_bin)};
        auto node{0};
        auto l{0};
        auto j{0};
        auto n(bin.size());
        if ((d - hd) >= 0) {
            // Build MT
            uint64_t number_of_lines, number_of_cols;
            std::tie (number_of_lines, number_of_cols) = get_dimensions(n, t, k, prefix_size);
            const auto *MT {createDenseMT(fasta, k, bin, prefix_size, number_of_lines, number_of_cols)};
            
            // Compute all siblings that have until d mutations from query kmer
            tbb::concurrent_unordered_map<std::string, uint64_t> hmap;
            hmap.reserve(n);
            kdive_(prefix_bin, sufix, MT, d - hd, sufix.size(), "", hmap, 0, 0, 0);
            
            // Delete MT
            delete [] MT;

            save(query, hmap, i);
            
        }
    });
}



