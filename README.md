![kflex](https://github.com/jadermcg/kflex/blob/4812a42c994811125cb2acc704c88c3b1096222f/logo.png)
# KFLEX: A Fast and Efficient Framework for Counting and Manipulating kmers
## Introduction
KFLEX is an advanced Linux/C++ framework designed for efficiently handling kmers in large genomic datasets. Using **Sparse Motif Tree (SMT)** (for more information about what is SMT, click [here](https://github.com/jadermcg/smt)) as its basic infrastructure, kflex was designed to optimize both RAM memory usage and processing time.

One of KFLEX's most notable features is its ability to work with large volumes of data without sacrificing efficiency. This is achieved through the use of kmer minimizers, which allow for a compact and effective representation of kmers. Additionally, the SMT structure contributes to rapid storage and retrieval of kmers, making KFLEX a robust solution for applications that require high performance and efficiency.

Whether you are a researcher, a bioinformatician or a developer, KFLEX offers a reliable and efficient solution for all your kmer-related needs.

## Instalation
The following steps must be followed for the correct installation of KFLEX.

### Prerequisites and dependencies
The `kflex` program is designed to run on the Linux operating system and has the following dependencies:

- **GLIBCXX_3.4.29** e **GLIBC_2.34**: These are specific versions of the libstdc++ (part of the C++ standard library) and glibc (standard C library on Linux) libraries. Make sure your Linux distribution has the appropriate versions installed.
- **Boost Filesystem**: A Boost library for file system operations.
- **Intel TBB**: Threading Building Blocks, a library that supports parallelization and efficient program execution.
- **Linux standard libraries**: libstdc++.so.6, libm.so.6, libgcc_s.so.1, e libc.so.6.

### Installation Steps
KFLEX can be used directly through the binary file present in the ```bin``` directory or compiled through the source code.

## Installation using binary file
The commands below were tested on the Ubuntu 22.04 LTS 64-bit operating system. For other systems, please consult the corresponding documentation.
1) First, install the dependencies with the following instruction: ``` sudo apt update && sudo apt install libstdc++6 libgcc1 libc6 libboost-filesystem-dev libtbb-dev```.
2) Create a directory to store the KFLEX binary, for example: ```mkdir /path/to/klex```.
3) Download the kflex binary from the GitHub bin directory and save it in the directory created above.
4) Place the above directory in the path so that it can be executed from any part of the system: ``` echo "export PATH=$PATH:/path/to/kflex" ```
  
## Installation using source code compilation
To install KFLEX via source code, download the files in the ```source``` directory to a local directory on your computer. After that, just run the ``` make ``` command. This will generate an executable called ```kflex```. After that, just follow 1 to 4, as explained above.

## Example of use
Below is an example that shows how to use the program in a basic way.
```
Use: kflex options
Options: 
   count <Preprocessing data and count>
   search <Search specific kmer and its counts>
   dump <Extract hashmap of all kmers and its counts>
   batch <Searching for many kmers>
   dsearch <Searching for many kmers directly>
   kdive <Searching for many kmers with d mutations>
   khmap <Extract hashmap for k less or equal to kmax>
```

First, we need to create the database using the command: ``` kflex count -i <fasta file> -k <kmer size> -p <sufix size> ```.
After this, a directory called *flex_data* will be created in the same directory in which KFLEX was run. After that, we can extract the kmers using the instruction: ``` kflex dump ```. This will create a file called *hmap.txt* inside *flex_data* with the count of all kmers. Additionally, it is possible to perform searches with ``` kflex search -kmer <kmer to search> ```. The command ```kflex batch -f <path to kmer> ``` does the same as the previous command, but it can perform the search on more than one kmer at the same time. The command ``` kflex kdive -f <path to kmers> -d <umber of mutations> ``` is particularly useful for the analysis and discovery of biological motifs, as it allows searching for kmers with up to *d* mutations. Finally, ```khmap -k <size to kmer> ``` allows hashmaps of sizes smaller than the value of *k* initially used to create the kmer database to be extracted.

For example, consider the example dataset 200000.fasta. This dataset has a total of 200 thousand sequences with a width of 100bp each. To create the initial database, we can run:
```
kflex count -i 200000.fasta -k 30 -p 5
```
This will create a *flex_data* directory in which the kmer database will be processed. After that, we can extract the map for all kmers of size 30:

```
kflex dump
```
The above command will create a file called *hmap.txt* with all kmers and their respective counts. We can also extract maps with *k* values between 5 and 29. The command below gives the example for *k = 18*:

```
kflex khmap -k 18
```

We can also check whether a kmer is in the database. To do this, we use:

```
kflex search AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
```
In this case, it will be queried whether the 30-mer *AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA* is present in the dataset. If yes, the program will return the kmer and its count. Otherwise, the returned value will be zero.

We can also query several kmers at the same time. To do this, we first need to create a file with the kmers to be searched. Consider the instruction below:

```
sort -k2nr < flex_data/hmap.txt | head -n 10 | awk '{print $1}' > flex_data/kmers.txt
```
This command will extract the 10 most frequent kmers from the *hmap.txt* map and create a file in *flex_data* called *kmers.txt*. We can use the *kmers.txt* file as input for the batch search command:

```
kflex batch -f flex_data/kmers.txt
```

This command will return the kmers and their respective counts.

Finally, mainly for the area of analysis and discovery of biological motifs, we can search for all kmers with up to *d* mutations. This command also performs a batch search, so we need to create a file with the kmers to be searched. Let's use the *flex_data/kmers.txt* file created previously. The command to search considering *d* mutations is the following:

```
kflex kdive -f flex_data/kmers.txt -d 2
```

The command above will search for all kmers with up to *d* mutations considering all kmers belonging to *kmers.txt*. The algorithm will create a directory inside *flex_data* called *kdive_dir*. Within this directory, several files will be created, one for each kmer within *kmers.txt*. Within this file, all kmers with up to *d* mutations will be placed. This data can later be used to create probabilistic models and thus help in the search for biologically relevant motifs.

## When to use KFLEX?
KFLEX was built with the main objective of assisting in the analysis of biological motifs. However, it can be used in any situation in which it is necessary to count kmers or perform operations on kmers, such as knowing which are the most frequent or which are the most frequent considering a certain number of mutations.
