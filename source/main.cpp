#include "smt.h"
#include "kdive.h"
#include "utils.h"
#include "khmap.h"
#include "dump.h"
#include <fstream>

int main(int argc, char **argv) {

    auto k{10};
    
    // Read metadata
    std::ifstream fp("flex_data/meta.txt");
    std::string path{""};
    int kmax, prefix_size;
    fp >> path >> kmax >> prefix_size;
    fp.close();

    int ret {system("rm -Rf flex_data/maps")};
    ret = system("mkdir -p flex_data/maps");
    
    // Read fasta
    const auto fasta{ readFasta(path) };
    
    khmap(fasta, prefix_size, k, kmax);

    ret = system("cat flex_data/maps/* > flex_data/khmap.txt");
    

    return 0;
}