#include "khmap.h"
#include "utils.h"
#include "smt.h"

void save(const tbb::concurrent_unordered_map<std::string, uint64_t> &hmap, const std::string &prefix) {
    std::ofstream fp("flex_data/maps/" + prefix);
    for (const auto &pair : hmap) {
        fp << pair.first << " " << pair.second << '\n';
    }

    fp.close();
}

void count(const uint64_t *MT, const int &kmax, int l, const uint64_t &node, uint64_t &acc) {
    if (l == kmax) {
        acc += MT[node * 6 + 4];
        return;
    }

    for (auto i{0}; i < 4; ++i) {
        auto next{MT[node * 6 + i]};

        if (next != 0) {
            count(MT, kmax, l + 1, next, acc);
        }
    }
}

void khmap_(const uint64_t *MT, const std::string &prefix, tbb::concurrent_unordered_map<std::string, uint64_t> &hmap, const std::vector<std::string> &fasta, const int &k, const int &kmax, const uint64_t &node, int l, std::string kmer) {
    if (l == k) {
        uint64_t acc{0};
        count(MT, kmax, l, node, acc);
        const auto &r {hmap.emplace(prefix + kmer, 0)};
        r.first -> second = acc;
        return;
    }

    for(auto i{0}; i < 4; ++i) {
        auto next{MT[node * 6 + i]};

        if (next != 0) {
            khmap_(MT, prefix, hmap, fasta, k, kmax, next, l + 1, kmer + int2char(i));
        }
    }
}

void khmap(const std::vector<std::string> &fasta, const int &prefix_size, const int &k, const int &kmax) {
    auto number_of_bins{static_cast<int>(std::pow(4, prefix_size))};
    auto t{fasta[0].size()};
    tbb::blocked_range<size_t> range(0, number_of_bins);
    tbb::parallel_for(range, [&](const auto &r) {
        for (auto i{r.begin()}; i < r.end(); ++i) {
            auto prefix{index2kmer(i, prefix_size)};
            const auto &bin{ read_bin(prefix) };
            auto n{bin.size()};

            auto number_of_lines{0};
            auto number_of_cols{0};
            std::tie(number_of_lines, number_of_cols) = get_dimensions(n, t, kmax, prefix_size);
            const auto *MT {createDenseMT(fasta, kmax, bin, prefix_size, number_of_lines, number_of_cols)};
            tbb::concurrent_unordered_map<std::string, uint64_t> hmap;
            hmap.reserve(n);
            khmap_(MT, prefix, hmap, fasta, k - prefix_size, kmax - prefix_size, 0, 0, "");
            delete [] MT;
            save(hmap, prefix);
        }
    });
}



