#include "BitInputStream.hpp"

/*
 * Helper fucntion to get Nbits
 */
unsigned int BitInputStream::getNbits() { return nbits; }

/**
 * TODO: Fill the buffer of size bufSize from the input stream.
 */
void BitInputStream::fill() {
    // Read all lines in input stream
    in.read(buf, bufSize);
    nbits = 0;
}

/**
 * TODO: Return true when the istream tried to read more bytes than the
 * remaining bytes in the file and the buffer was also fully read. Otherwise,
 * return false.
 *
 * Note that eof() is different from this function. eof() in most other istreams
 * is only true when we are trying to read past the end of the file. Whereas
 * this function should be only true when we are at the end of the file, but
 * have not read past it yet. For example:
 *
 * // some code to read all the data in the file
 * atEndOfFile(); // atEndOfFile() = true. But eof() is still false for now.
 * readBit();     // trying to read past the end of the file
 * eof();         // eof is true now
 */
bool BitInputStream::atEndOfFile() {
    if (!in) {
        // number of bytes successfully read to buffer
        int numBytesRead = in.gcount();
        // Number of bytes successfully read to the buffer = 0.
        if (numBytesRead == 0) {
            return true;
        }
        // buffer was fully read.
        if (nbits == (numBytesRead * 8)) {
            return true;
        }
    }
    return false;
}

/* Returns true when trying to read past the end of the file, false otherwise */
bool BitInputStream::eof() { return eofBit; }

/**
 * TODO: Read the next bit from the bit buffer. Fills the buffer from the input
 * stream if all the bits have already been read. It should return 0 if the bit
 * read is 0, and return 1 if the bit read is 1.
 *
 * Before trying to read a bit from the buffer, if atEndOFfile() is true, set
 * the eofBit and directly return 0 (You can return any number here, since after
 * setting eofBit to true, eof() will return true and the program won’t be using
 * the returned value from readBit).
 */
unsigned int BitInputStream::readBit() {
    // Fill buffer if nbits is bufSize*8
    if (nbits == bufSize * 8) {
        fill();
    }
    if (atEndOfFile()) {
        eofBit = true;
        return 0;
    }

    // Int that tells us in which buf[a] we are
    int a = nbits / 8;
    // Char located at int a in the array
    byte b = buf[a];
    // Int that tells us how many times we need to shift to the right
    int shift = 7 - (nbits % 8);

    byte bit = (b >> shift) & 1;
    nbits++;

    return (unsigned int)bit;
}
