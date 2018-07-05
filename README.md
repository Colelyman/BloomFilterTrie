# BFT: Bloom Filter Trie

This repository contains the source code of the Bloom Filter Trie (BFT) library. The BFT is an alignment-free, reference-free and incremental succinct data structure for colored de Bruijn graphs. It is based on the burst trie and use Bloom filters for efficient trie and graph traversals. The data structure indexes *k*-mers and their colors based on a new representation of trie vertices that
compress and index shared substrings. A typical application of the BFT is pan-genome indexing.

## Dependencies

In order to compile and use the BFT, you need a machine running a 64 bits Linux or Mac OS operating system, POSIX compliant and accepting SSE4 instructions.

The library depends on three external libraries: CMake (https://cmake.org/), Judy Arrays
(http://judy.sourceforge.net) and Jemalloc (http://www.canonware.com/jemalloc).
These libraries will automatically be downloaded and installed for you via CMake
(no super-user privileges required).

## Compilation and installation

The library compiles with GNU GCC, G++, and Clang. It successfully compiles and
runs on Ubuntu 14.04 and 15.04 and Mac OS X 10.13.5. The minimum version of
CMake that is needed is 3.1.

### Linux

On Linux, you can verify the presence of cmake, gcc and g++ on your system with:
```
cmake --version
gcc -v
g++ -v
```

If not present (unlikely), they can be installed for Ubuntu or Debian with:
```
sudo apt-get install build-essential cmake
```

### Mac OS

You can optionally install the "real" GCC and G++, or use the default Clang
interface (most likely) already installed on your system. You can install cmake
via Homebrew (https://brew.sh/):
```
brew install cmake
```

If you want to install the "real" GCC and G++, run:
```
brew install gcc-x
brew install gcc-x
```

where `x` is the latest major version of GCC and G++ (on 7/2018 `x == 8`). If
you would like to use these installed compilers to install BFT, modify the
`cmake ..` command below to `cmake .. -DCMAKE_C_COMPILER=$(which gcc-x)` (again
with `x` replaced with the version number).

### Mac OS X and Linux

Compiling the library should then be as simple as:
```
cd <BFT_directory>
mkdir build && cd build
cmake ..
make
```

The above command will result in the creation of a directory
`<BFT_directory>/bin` which will contain the binary executable `bft`, the shared
library `bft_shared.so` (on Linux or `bft_shared.dylib` on Mac OS X), and the
static library `bft_static.a`.

Make sure that the environment variables PATH, LIBRARY_PATH, LD_LIBRARY_PATH and
C_INCLUDE_PATH are set with *\<BFT_directory\>/bin* in order to use BFT outside
of `<BFT_directory>`. You can also install onto your system by running:
```
sudo make install
```


## API Usage

### C

Once the library is installed on your system, just use
```
#include <bft.h>
```
in your C code. Then, use the following flag for linking:
```
-lbft_(static|shared)
```
meaning either `-lbft_static` or `-lbft_shared` depending on if you want to use
the static or shared library.

### C++

Once the library is installed on your system, just use
```
extern "C" {
	#include <bft.h>
}
```
in your C++ code. Then, use the following flag for linking:
```
-lbft_(static|shared)
```

### Non-default installations

If you don't have super-user privileges (or you just chose not to run `sudo make
install`) then you will need to add the following argument when you compile:
```
-L<BFT_directory/bin
```

this allows the compiler to find the BFT libraries. *NOTE:* make sure that you
place this _after_ the `-lbft_(shared|static)`.

## API documentation:

Documentation for the BFT library is available in the /doc/doxygen folder (HTML version, open index.html).

The following command regenerates the documentation:
```
cd <BFT_directory>
doxygen Doxyfile
```

The documentation contains a description of all the functions and structures of the library as well as code snippets.

## Binary usage:

Installing the BFT library also produces a binary that shows what it is possible to do with the library. Therefore, the binary can perform a limited number of operations described in the following.

```
bft build k {kmers|kmers_comp} list_genome_files output_file [Options]
bft load file_bft [-add_genomes {kmers|kmers_comp} list_genome_files output_file] [Options]

Options:
[-query_sequences threshold {canonical|non_canonical} list_sequence_files]
[-query_kmers {kmers|kmers_comp} list_kmer_files]
[-query_branching {kmers|kmers_comp} list_kmer_files]
[-extract_kmers {kmers|kmers_comp} kmers_file]

Version:
bft --version
```
### Commands

Command **build** creates the BFT for the files listed in *list_genome_files* and writes the BFT in file *output_file*.

* *k*: length of *k*-mers
* *list_genome_files*: file that contains a list of files (one path and name per line) to be inserted in the BFT.
* *output_file*: file where to write the BFT.

Command **load** loads a BFT from file *file_bft*.

* *file_bft*: file that contains a BFT

### Options

* **-add_genomes** adds the genomes listed in *list_genome_files* to the BFT stored in *file_bft*, the new BFT is written in *output_file*
* **-query_sequences** queries the BFT for the sequences written in the files of *list_sequence_files*. For each file of *list_sequence_files* is output a CSV file: columns are the genomes represented in the BFT, rows are the queried sequences, the intersection of a column and a row is a binary value indicating if the sequence represented by the row is present in the genome represented by the column. *threshold* is a float (0 < *threshold* <= 1) indicating the percentage of *k*-mers from each query sequence that must occur in sample *x* to be reported present in sample *x*. *canonical* indicates that only the canonical *k*-mers of the queries are searched (the lexicographically smaller one between a k-mer and its reverse-complement). To the contrary, *non_canonical* indicates that the *k*-mers of the queries are searched as they are in the queries.
* **-query_kmers** queries the BFT for *k*-mers written in the files of *list_kmer_files*. For each file of *list_kmer_files* is output a CSV file: columns are the genomes represented in the BFT, rows are the queried *k*-mers, the intersection of a column and a row is a binary value indicating if the *k*-mer represented by the row is present in the genome represented by the column.
* **-query_branching** queries the BFT for the number of *k*-mers written in the files of *list_kmer_files* that are branching in the colored de-Bruijn graph represented by the BFT.
* **-extract_kmers** extracts the *k*-mers stored in the BFT and writes them to a *k*-mers file named *kmers_file* (see below for input file types).

New options will be available soon.

### I/O file types

* *kmers*: files are *k*-mers files. Each file contains one *k*-mer (plain text) per line, eventually followed by a count.
* *kmers_comp*: files of *list_genome_files* are compressed *k*-mers files. Each file is built as the following: First line is *k* (plain text), second line is the number of *k*-mers in the file (plain text) and third line is the concatenation of all compressed *k*-mers. A compressed *k*-mer is encoded with two bits per nucleotid (one byte for 4 nucleotids) with A=00, C=01, G=10 and T=11. A byte is always encoded from the Less Significant Bit to the Most Significant Bit. If a byte cannot be entirely filled in with nucleotids, it is padded with 0s.
Example: ACTTGTCTG -> 11110100 11011110 00000010

## Citation

If you want to cite the Bloom Filter Trie, please use:
```
@article{holley2016bft,
  title="{Bloom Filter Trie: an alignment-free and reference-free data structure for pan-genome storage}",
  author={Holley, Guillaume and Wittler, Roland and Stoye, Jens},
  journal={Algorithms Mol. Biol.},
  volume={11},
  pages={3},
  year={2016}
}
```

## Contact

For any question, feedback or problem, please contact me at gholley[At]cebitec[D0t]uni-bielefeld[D0t]de
