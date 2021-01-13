#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

class BitInputStream {
  private:
    istream& in;           // reference to the input stream to use
    char* buf;             // buffer of an char array
    unsigned int bufSize;  // size of buffer in byte
    unsigned int nbits;    // number of bits that have been read from buf
    bool eofBit;           // true when trying to read past the end of the file

    bool atEndOfFile();

  public:
    /**
     * TODO: Initializes member variables. In particular, the char array should
     * have size bufSize.
     */
    explicit BitInputStream(istream& is, unsigned int bufSize) : in(is) {
        buf = new char[bufSize]();
        this->bufSize = bufSize;
        nbits = bufSize * 8;
        eofBit = false;
    };

    ~BitInputStream() { delete[] buf; }

    unsigned int getNbits();

    void fill();

    bool eof();

    unsigned int readBit();
};

#endif
