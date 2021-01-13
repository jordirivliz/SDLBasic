/**
 * TODO: file header
 *
 * Author: Jorge A. Rivera Lizarralde
 */
#include <math.h>

#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
using namespace std;

/* TODO: add pseudo compression with ascii encoding and naive header
 * (checkpoint) */
void pseudoCompression(const string& inFileName, const string& outFileName) {
    HCTree tree;
    // Opening file and reading from it
    ifstream infile;
    infile.open(inFileName, ios::in);

    // Count the frequencies of each char in file and add them to vector
    vector<unsigned int> frequencies(256, 0);

    while (true) {
        int value = infile.get();
        if (value == -1) {
            break;
        }
        // Char in string
        unsigned char c = value;
        // Add one to the frequencies
        frequencies[(int)c]++;
    }

    tree.build(frequencies);
    infile.close();

    // Create the header of the file
    ofstream compressed;
    compressed.open(outFileName, ios::out);
    for (int i = 0; i < 256; i++) {
        compressed << frequencies[i] << endl;
    }

    ifstream infile1;
    infile1.open(inFileName, ios::in);
    // Traverse the chars of the file and encode all of them
    while (true) {
        int value = infile1.get();
        if (value == -1) {
            break;
        }
        tree.encode((byte)value, compressed);
    }
    // Close file
    infile1.close();
    compressed.close();
}

/* TODO: True compression with bitwise i/o and small header (final) */
void trueCompression(const string& inFileName, const string& outFileName) {
    HCTree tree;
    // Opening file and reading from it
    ifstream infile;
    infile.open(inFileName, ios::in);

    // Count the frequencies of each char in file and add them to vector
    vector<unsigned int> frequencies(256, 0);
    while (true) {
        int value = infile.get();
        if (value == -1) {
            break;
        }
        // Char in string
        unsigned char c = value;
        // Add one to the frequencies
        frequencies[(int)c]++;
    }

    tree.build(frequencies);
    infile.close();

    // Opening file and writing to it the header with 1 and 0
    ofstream compressed;
    compressed.open(outFileName, ios::out);
    BitOutputStream outputStream(compressed, 4000);

    // CREATE HEADER
    // Use for freqsHeader
    // for (int i = 0; i < 256; i++) {
    //     compressed << frequencies[i] << endl;
    // }
    // Sort frequencies to determine largest one
    vector<unsigned int> copyFreqs(256, 0);
    for (int i = 0; i < 256; i++) {
        copyFreqs[i] = frequencies[i];
    }
    sort(copyFreqs.begin(), copyFreqs.end(), greater<int>());
    int maxFreq = copyFreqs[0] + 1;
    double logBase = log2(maxFreq);
    int numBits = ceil(logBase);

    // Number of non-zero frequencies
    int nonZero = 0;
    for (int i = 0; i < 256; i++) {
        int freq = frequencies[i];
        if (freq != 0) {
            nonZero++;
        }
    }
    string bytes;
    // Set first five bits in the header to be the numInts
    bytes = bytes + bitset<5>(numBits).to_string();

    // Set next eight bits in the header to be number of nonZero freqs
    bytes = bytes + bitset<8>(nonZero).to_string();
    // for each non-zero frequency symbol, output 8 bits to represent the
    // symbol, followed by numBits bits to represent its frequency.
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] != 0) {
            int letter = i;
            int freq = frequencies[i];

            // Add the character and its frequency
            bytes = bytes + bitset<8>(letter).to_string();
            string f = bitset<31>(freq).to_string();
            f = f.substr(31 - numBits, f.length());
            bytes = bytes + f;
        }
    }
    // write to the outputStream all bits in string bytes
    for (int i = 0; i < bytes.length(); i++) {
        char a = bytes[i];
        int b = a & 1;
        outputStream.writeBit(b);
    }

    // Opening file and reading from it
    ifstream infile1;
    infile1.open(inFileName, ios::in);

    while (true) {
        int value = infile1.get();
        if (value == -1) {
            break;
        }
        tree.encode((byte)value, outputStream);
    }
    outputStream.flush();
    // Close file
    infile1.close();
    compressed.close();
}

/* Main program that runs the compression */
int main(int argc, char* argv[]) {
    cxxopts::Options options(argv[0],
                             "Compresses files using Huffman Encoding");
    options.positional_help("./path_to_input_file ./path_to_output_file");

    bool isAsciiOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Write output in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAsciiOutput))(
        "input", "", cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        return 0;
    }

    // if original file is empty, output empty file
    if (FileUtils::isEmptyFile(inFileName)) {
        ofstream outFile;
        outFile.open(outFileName, ios::out);
        outFile.close();
        return 0;
    }

    if (isAsciiOutput) {
        pseudoCompression(inFileName, outFileName);
    } else {
        trueCompression(inFileName, outFileName);
    }

    return 0;
}