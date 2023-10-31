#include "dump.h"
#include "utils.h"

//'Save map kmer:count in disk.
//'@name save_map.
//'@param hmap Hashmap to save in disk.
//'@param prefix Prefix name to hashmap.
void save_map(const std::unordered_map<std::string, uint64_t> &hmap, const std::string &prefix) {
    std::ofstream fp("flex_data/maps/" + prefix);
    for(const auto &pair : hmap) {
        fp << pair.first << " " << pair.second << '\n';
    }

    fp.close();
}


//'Process all bins of kmers and count its frequency.
//'@name process_and_count.
//'@param fasta Dataset of sequences.
//'@param The number_of_bins Number of bins that data was divided.
//'@param k Size of kmers.
void process_and_count(const std::vector<std::string> &fasta, const uint &number_of_bins, const uint &k, const uint &prefix_size) {
    tbb::blocked_range<size_t> range(0, number_of_bins);
    tbb::parallel_for(range, [&](const auto &r) {
        for(auto i{r.begin()}; i < r.end(); ++i) {
            const auto &prefix{index2kmer(i, prefix_size)};
            const auto &bin{read_bin(prefix)};

            auto n{bin.size()};
            auto t{fasta[0].size()};
            auto m{t - k + 1};

            // Count kmers
            std::unordered_map<std::string, uint64_t> hmap;
            hmap.reserve(n);
            for (const auto &address: bin) {
                const auto line = address / m;
                const auto col = address % m;
                const std::string kmer = fasta[line].substr(col, k);
                auto result {hmap.emplace(kmer, 0)};
                ++(result.first->second);
            }

            // Save kmers in disk
            save_map(hmap, prefix);
        }
    });
}
