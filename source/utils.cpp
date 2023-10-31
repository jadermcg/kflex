#include "utils.h"

//'Computes hamming distance efficiently.
//'@name hDist.
//'@param str1 First string to compare.
//'@param str2 Second string to compare.
//'@return Hamming distance between str1 and str2.
int hDist(const std::string &str1, const std::string &str2) {
  return std::inner_product(str1.begin(), str1.end(), str2.begin(), 0, std::plus<>(), std::not_equal_to<>());
}

//'Compute the number of lines and columns of MT for correct memory allocation.
//'@name get_dimensions.
//'@param n Number of elements inside a bin.
//'@param t Length of a fasta sequence.
//'@param k Size of kmer.
//'@param prefix_size Size of prefix.
//'@return Number os lines and columns to correct MT memory allocation.
std::tuple<size_t, size_t> get_dimensions(const size_t &n, const size_t &t, const size_t &k, const size_t &prefix_size) {
  auto m{t - k + 1};
  auto number_of_lines{n * (k - prefix_size) + 1};
  auto number_of_cols{6};

  return std::make_tuple(number_of_lines, number_of_cols);
}

//'Read bin from file.
//'@name read_bin.
//'@param prefix Path to binary bin file.
//'@return C++ uint64_t Vector whith all address of kmers inside bin.
std::vector<uint64_t> read_bin(const std::string &prefix) {
  std::string path = "flex_data/bins/" + prefix;
  FILE *fp = std::fopen(path.data(), "rb");
  std::fseek(fp, 0, SEEK_END);
  const auto &fsize{std::ftell(fp)};
  std::fseek(fp, 0, SEEK_SET);

  const auto nitems { fsize / sizeof(uint64_t) };
  
  std::vector<uint64_t> bin(nitems);
  if (!std::fread(bin.data(), sizeof(uint64_t), nitems, fp)) {
      throw std::runtime_error("Cannot be possible to read file!");
  }
  std::fclose(fp);

  return bin;
}

//'Read kmers from file.
//'@name readkmers.
//'@param path Path to kmers.
//'@return C++ String Vector whith all kmers.
std::vector<std::string> readkmers(const std::string &path) {
  std::vector<std::string> kmers;
  std::ifstream file(path);

  if (!file.is_open()) {
    std::cerr << "Não foi possível abrir o arquivo!\n";
  }

  std::string kmer;
  while(file >> kmer) {
    kmers.push_back(kmer);
  }

  file.close();
  return kmers;
}

//'Read hash map in format XXXXX COUNT. 
//'@name readhmap.
//'@param filename Path to hash map.
//'@return C++ String Map.
std::unordered_map<std::string, uint64_t> readhmap(const std::string &filename) {

  std::unordered_map<std::string, uint64_t> hmap;

  std::ifstream file(filename); // Abre o arquivo especificado pelo nome

  if (!file.is_open()) {
    std::cerr << "Não foi possível abrir o arquivo!\n";
  }

  std::string kmer;
  uint64_t count;
  while (file >> kmer >> count) { // Lê o kmer e a contagem
    hmap[kmer] = count; // Insere no mapa
  }

  file.close();
  return hmap;
}

//'Read fasta dataset.
//'@name readFasta.
//'@param filepath Path to fasta dataset.
//'@return C++ vector string whth each line is a sequence in fasta dataset.
std::vector<std::string> readFasta(const std::string& filepath) {
  std::ifstream file(filepath);
  std::vector<std::string> data;
  std::string line;
  std::string seq;
  
  int i = 0;
  while (std::getline(file, line)) {
    
    if (line[0] == '>' && i != 0) {
      if (seq.find('N') == std::string::npos) data.push_back(seq);
      seq.clear();
    }
    
    else if (line[0] != '>' && i != 0) {
      seq += line;
    }
    
    ++i;
  }
  
  if (seq.find('N') == std::string::npos) data.push_back(seq);
  
  return data;
}

//'Converts char nucleotide A,C,G,T in int 0,1,2,3.
//'@name char2int.
//'@param c char to convert for.
//'@return The correspondent int or throws a invalid character.
int char2int(char c) {
  switch(c) {
    case 'A': return 0;
    case 'C': return 1;
    case 'G': return 2;
    case 'T': return 3;
    case 'X': return -1;
    default: throw std::invalid_argument("Invalid character");
  }

}

//'Converts int 0,1,2,3 to nucleotide char A,C,G,T.
//'@name int2char.
//'@param i int to convert for.
//'@return The correpondent nucleotide character or throws a invalid integer.
char int2char(int i) {
  switch(i) {
    case 0: return 'A';
    case 1: return 'C';
    case 2: return 'G';
    case 3: return 'T';
    case -1: return 'X';
    default: throw std::invalid_argument("Invalid integer");
  }
}

//'This function convert a kmer to index.
//'@name kmer2index.
//'@param kmer Kmer from convert to index.
//'@return Integer representation from kmer.
uint64_t kmer2index(const std::string &kmer) {
  int k = kmer.size();
  uint64_t index = 0;
  for (int i = 0; i < k; ++i) {
    index = index * 4 + char2int(kmer[i]);
  }
  return index;
}

//'This function convert a index to kmer.
//'@name index2kmer.
//'@param index Index from convert to kmer.
//'@return String representation from index.
std::string index2kmer(uint64_t index, int k) {
  std::string kmer(k, 'A'); // Inicializa a string kmer com tamanho k e todos os caracteres como 'A'
  
  for (int i = k - 1; i >= 0; --i) {
    int residue = index % 4;
    kmer[i] = int2char(residue);
    index /= 4;
  }
  
  return kmer;
}

//'Read all file names into a dirPath.
//'getFilenames.
//'@param dirPath Directory path.
//'@return C++ string vector with directory file names.
std::vector<std::string> getFilenames(const std::string& dirPath) {
  std::vector<std::string> filenames;
  boost::filesystem::path path(dirPath);
  
  try {
    if (boost::filesystem::exists(path) && boost::filesystem::is_directory(path)) {
      for (boost::filesystem::directory_iterator itr(path); itr != boost::filesystem::directory_iterator(); ++itr) {
        filenames.push_back(itr->path().filename().string());
      }
    } else {
      std::cerr << "Caminho não encontrado ou não é um diretório\n";
    }
  } catch (const boost::filesystem::filesystem_error& e) {
    std::cerr << "Erro ao acessar o diretório: " << e.what() << '\n';
  }
  
  return filenames;
}
