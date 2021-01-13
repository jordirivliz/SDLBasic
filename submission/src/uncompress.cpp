/**
 * TODO: file header
 *
 * Author: Jorge A. Rivera Lizarralde
 */
#include <stdlib.h>

#include <cmath>
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"

/*
 * Helper function that converts a binary number in decimal
 */
int binaryToInt(string n) {
    // IMPLEMENTATION 1
    // int rem, temp;
    // int decimal = 0;
    // int b = 1;
    // temp = num;
    // while (temp > 0) {
    //     rem = temp % 10;
    //     decimal = decimal + rem * b;
    //     b *= 2;
    //     temp /= 10;
    // }
    // return decimal;

    // IMPLEMENTATION 2
    // int decimalNumber = 0, i = 0, remainder;
    // while (n != 0) {
    //     remainder = n % 10;
    //     n /= 10;
    //     decimalNumber += remainder * pow(2, i);
    //     ++i;
    // }
    // return decimalNumber;

    // IMPLEMENTATION 3
    string num = n;
    int decimal = 0;
    int base = 1;
    int len = num.length();

    for (int i = len - 1; i >= 0; i--) {
        if (num[i] == '1') decimal += base;
        base = base * 2;
    }
    return decimal;
}
/* TODO: Pseudo decompression with ascii encoding and naive header (checkpoint)
 */
void pseudoDecompression(const string& inFileName, const string& outFileName) {
    HCTree tree;
    // Opening file and reading from it
    ifstream infile;
    infile.open(inFileName, ios::in);

    vector<unsigned int> frequencies(256, 0);

    for (int i = 0; i < 256; i++) {
        unsigned int value;
        infile >> value;
        frequencies[i] = value;
        infile.ignore();
    }

    tree.build(frequencies);

    // Opening file and writing to it
    ofstream decompressed;
    decompressed.open(outFileName, ios::out);

    while (infile.peek() != EOF) {
        byte value = tree.decode(infile);
        decompressed << value;
    }
    // Close file
    infile.close();
    decompressed.close();
}

/* TODO: True decompression with bitwise i/o and small header (final) */
void trueDecompression(const string& inFileName, const string& outFileName) {
    HCTree tree;
    // Opening file and reading from it
    ifstream infile;
    infile.open(inFileName, ios::in);
    BitInputStream inputStream(infile, 4000);

    vector<unsigned int> frequencies(256, 0);

    // Read numBits from compressed
    string numBits0 = "";
    for (int i = 0; i < 5; i++) {
        int bits = inputStream.readBit();
        numBits0 = numBits0 + to_string(bits);
    }
    // int numBits1 = stoi(numBits0);
    // cout << numBits0 << endl;
    int numBits = binaryToInt(numBits0);
    // cout << numBits << endl;

    // Read nonZero freqs from compressed
    string nonZero0 = "";
    for (int i = 0; i < 8; i++) {
        int bits = inputStream.readBit();
        nonZero0 = nonZero0 + to_string(bits);
    }
    // int nonZero1 = stoi(nonZero0);
    int nonZero;
    if (nonZero0 == "00000000" && numBits != 0) {
        nonZero = 256;
    } else {
        nonZero = binaryToInt(nonZero0);
    }
    // cout << nonZero0 << endl;
    // cout << nonZero << endl;
    // Read nonZero blocks of char-freq
    int read = 8 + numBits;
    for (int i = 0; i < nonZero; i++) {
        string character = "";
        string freq = "";
        for (int j = 0; j < 8; j++) {
            int bits = inputStream.readBit();
            character = character + to_string(bits);
        }
        int letter = binaryToInt(character);
        for (int l = 0; l < numBits; l++) {
            int bits = inputStream.readBit();
            freq = freq + to_string(bits);
        }
        int freqs = binaryToInt(freq);
        frequencies[letter] = freqs;
    }

    // Build tree with frequencies and get the # total freqs
    int totalFreqs = 0;
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] != 0) {
            totalFreqs = totalFreqs + frequencies[i];
        }
        // cout << "i: " << i << "; Freqs: " << frequencies[i] << endl;
    }
    tree.build(frequencies);

    // Opening file and writing to it
    ofstream decompressed;
    decompressed.open(outFileName, ios::out);

    for (int i = 0; i < totalFreqs; i++) {
        byte value = tree.decode(inputStream);
        decompressed.put(value);
    }
    // Close file
    infile.close();
    decompressed.close();
}

/* Main program that runs the decompression */
int main(int argc, char* argv[]) {
    cxxopts::Options options(argv[0],
                             "Uncompresses files using Huffman Encoding");
    options.positional_help(
        "./path_to_compressed_input_file ./path_to_output_file");

    bool isAscii = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Read input in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAscii))("input", "",
                                       cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit.");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        exit(0);
    }

    // if compressed file is empty, output empty file
    if (FileUtils::isEmptyFile(inFileName)) {
        ofstream outFile;
        outFile.open(outFileName, ios::out);
        outFile.close();
        exit(0);
    }

    if (isAscii) {
        pseudoDecompression(inFileName, outFileName);
    } else {
        trueDecompression(inFileName, outFileName);
    }

    return 0;
}
