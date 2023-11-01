#include "smt.h"
#include "utils.h"
#include "ksearch.h"
#include "dump.h"
#include "dsearch.h"
#include "kdive.h"
#include "khmap.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>

int main(int argc, char **argv) {

    std::string path{""};
    auto k{0};
    auto prefix_size{0};
    
    if (argc == 1) {
        std::cout << "Use: kflex options\n";
        std::cout << "Options: " << '\n';
        std::cout << "   count <Preprocessing data and count>" << '\n';
        std::cout << "   search <Search specific kmer and its counts>" << '\n';
        std::cout << "   dump <Extract hashmap of all kmers and its counts>" << '\n';
        std::cout << "   batch <Searching for many kmers>" << '\n';
        std::cout << "   dsearch <Searching for many kmers directly>" << '\n';
        std::cout << "   kdive <Searching for many kmers with d mutations>" << '\n';
        std::cout << "   khmap <Extract hashmap for k less or equal to kmax>" << '\n';
        return -1;
    }


    const std::string &main_arg{argv[1]};
    if (main_arg == "count") { // Initialize bins and count kmers.
        
        if (argc < 8) {
            std::cerr << "Use: kflex count -i <fasta> -k <size of kmer> -p <size of prefix>\n";
            return -1;
        }

        for (int i = 2; i < argc; i = i + 2) {
            const std::string &arg {argv[i]};

            if (arg == "-i") path = argv[i + 1];

            else if (arg == "-k") k = std::stoi(argv[i + 1]);

            else if (arg == "-p") {
                prefix_size = std::stoi(argv[i + 1]);
                if (prefix_size > k) throw std::invalid_argument("p must be less than k!");
            }

            else throw std::invalid_argument("Invalid count argument!");
        }

        
        if (!std::filesystem::exists(path)) {
            std::cerr << "File not found!" << '\n';
            return -1;
        }

        if (prefix_size == 0) prefix_size = 5;
        const auto &fasta = readFasta(path);
        createBins(fasta, k, prefix_size);
        std::ofstream fp("flex_data/meta.txt");
        fp << path << '\n';
        fp << k << '\n';
        fp << prefix_size << '\n';
        fp.close();
    }

    else if (main_arg == "search") { // Searching for only one kmer.

        if (argc < 4) {
            std::cerr << "Use: kflex search -kmer <kmer to search>\n";
            return -1;
        }

        std::ifstream fp("flex_data/meta.txt");
        if (!fp) {
            std::cerr << "Use first: kflex count -i <fasta> -k <size of kmer> -p <size of prefix>\n" << '\n';
            return -1;
        }
        fp >> path >> k >> prefix_size;
        fp.close();

        std::string query{""};
        for (auto i{2}; i < argc; i = i + 2) {
            std::string arg{argv[i]};

            if (arg == "-kmer") {
                query = argv[i + 1];
            }

            else {
                throw std::invalid_argument("Invalid argument!");
            }
        }
        
        
        const auto &fasta{ readFasta(path) };
        uint64_t count = ksearch(fasta, query, prefix_size);
        std::cout << query << " " << count << '\n';
    }

    else if (main_arg == "dump") { // Extract hashmap of kmers and its counts.
        if (argc < 2) {
            std::cerr << "Use: kflex dump\n";
            return -1;
        }

        std::ifstream fp("flex_data/meta.txt");
        if (!fp) {
            std::cerr << "Use first: kflex count -i <fasta> -k <size of kmer> -p <size of prefix>\n" << '\n';
            return -1;
        }
        fp >> path >> k >> prefix_size;
        fp.close();

        int ret = std::system("rm -Rf flex_data/maps");
        ret = std::system("mkdir -p flex_data/maps");
        const auto &fasta{ readFasta(path) };
        const auto number_of_bins {static_cast<size_t>(std::pow(4, prefix_size))};
        process_and_count(fasta, number_of_bins, k, prefix_size);
        ret = std::system("rm -Rf flex_data/hmap.txt");
        ret = std::system("cat flex_data/maps/* > flex_data/hmap.txt");
    }

    else if (main_arg == "batch") { // Searching for many kmers.
        if (argc < 4) {
            std::cerr << "Use: kflex batch -f <path to kmers>\n";
            return -1;
        }

        std::ifstream fp("flex_data/meta.txt");
        if (!fp) {
            std::cerr << "Use first: kflex count -i <fasta> -k <size of kmer> -p <size of prefix>\n" << '\n';
            return -1;
        }
        fp >> path >> k >> prefix_size;
        fp.close();

        std::string path2kmers{""};
        for (auto i{2}; i < argc; i = i + 2) {
            std::string arg{argv[i]};

            if (arg == "-f") {
                path2kmers = argv[i + 1];
            }

            else throw std::invalid_argument("Invalid argument!");
                
        }
        
        
        const auto &kmers { readkmers(path2kmers) };
        const auto &fasta { readFasta(path) };

        std::stringstream ss;
        for(const auto &kmer : kmers) {
            const auto &count { ksearch(fasta, kmer, prefix_size) };
            ss << kmer << " " << count << '\n';
        }

        std::cout << ss.str();

    }

    else if (main_arg == "dsearch") {
        if (argc < 4) {
            std::cerr << "Use: kflex dsearch -i <path to fasta> -f <path to kmers>\n";
            return -1;
        }

        std::string path2kmers{""};
        for (auto i{2}; i < argc; i = i + 2) {
            std::string arg{argv[i]};

            if (arg == "-i") {
                path = argv[i + 1];
            }

            else if (arg == "-f") {
                path2kmers = argv[i + 1];
            }

            else throw std::invalid_argument("Invalid argument!");
        }

        std::vector<std::string> kmers {readkmers(path2kmers)};
        std::vector<std::string> fasta{readFasta(path)};
        k = kmers[0].size();
        const auto &counts{dsearch(fasta, kmers)};

        std::stringstream ss;
        for(const auto &kmer : counts) {
            ss << kmer.first << " " << kmer.second << '\n';
        }

        std::cout << ss.str();
    }

    else if (main_arg == "kdive") {
        if (argc < 6) {
            std::cerr << "Use: kflex kdive -f <path to kmers> -d <number of mutations>\n";
            return -1;
        }

        std::ifstream fp("flex_data/meta.txt");
        if (!fp) {
            std::cerr << "Use first: kflex count -i <fasta> -k <size of kmer> -p <size of prefix>\n" << '\n';
            return -1;
        }
        fp >> path >> k >> prefix_size;
        fp.close();

        std::string path2kmers{""};
        auto d{0};
        for (auto i{2}; i < argc; i = i + 2) {
            std::string arg{argv[i]};

            if (arg == "-f") {
                path2kmers = argv[i + 1];
            }

            else if (arg == "-d") {
                d = std::stoi(argv[i + 1]);
            }

            else throw std::invalid_argument("Invalid argument!");
        }

        // Read fasta
        const auto fasta{ readFasta(path) };

        // Read kmers
        const auto &kmers { readkmers(path2kmers) };

        // Kdive
        int ret = std::system("rm -Rf flex_data/kdive_dir");
        for (const auto &query : kmers) {
            std::string command{"mkdir -p flex_data/kdive_dir/" + query};
            ret = std::system(command.c_str());
            kdive(query, prefix_size, d, fasta);
            command = "cat flex_data/kdive_dir/" + query + "/* > flex_data/kdive_dir/" + query + ".txt ;" + "rm -Rf flex_data/kdive_dir/" + query;
            ret = system(command.c_str());
        }
    }

    else if (main_arg == "khmap") {
        if (argc < 4) {
            std::cerr << "Use: kflex khmap -k <size of kmers>\n";
            return -1;
        }

        std::ifstream fp("flex_data/meta.txt");
        if (!fp) {
            std::cerr << "Use first: kflex count -i <fasta> -k <size of kmer> -p <size of prefix>\n" << '\n';
            return -1;
        }
        auto kmax{0};
        fp >> path >> kmax >> prefix_size;
        fp.close();

        for (auto i{2}; i < argc; i = i + 2) {
            std::string arg{argv[i]};

            if (arg == "-k") {
                k = std::stoi(argv[i + 1]);
            }

            else throw std::invalid_argument("Invalid argument!");
        }

        int ret {system("rm -Rf flex_data/maps")};
        ret = system("mkdir -p flex_data/maps");
        
        // Read fasta
        const auto fasta{ readFasta(path) };
        
        khmap(fasta, prefix_size, k, kmax);

        ret = system("cat flex_data/maps/* > flex_data/khmap.txt");
    }

    else throw std::invalid_argument("Invalid argument!");

    
}