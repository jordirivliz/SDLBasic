#include "BitOutputStream.hpp"

/**
 * TODO: Write the part of the buffer that was written by the user to the output
 * stream, and then clear the buffer to allow further use. You may use fill() to
 * zero-fill the buffer.
 *
 * Note: don’t flush the ostream here, as it is an extremely slow operation that
 * may cause a timeout.
 */
void BitOutputStream::flush() {
    int num = nbits / 8;
    byte b;
    // Loop over all the buf until buf[a] without including because it's the
    // partially filled
    for (int i = 0; i <= (nbits - 1) / 8; i++) {
        out << buf[i];
    }
    fill(buf, buf + bufSize, 0);
    nbits = 0;
}

/**
 * TODO: Write the least significant bit of the given int to the bit buffer.
 * Flushes the buffer first if it is full (which means all the bits in the
 * buffer have already been set). You may assume the given int is either 0
 * or 1.
 */
void BitOutputStream::writeBit(unsigned int i) {
    // Flush if buffer is full
    if (nbits == bufSize * 8) {
        flush();
    }
    // Int that tells us in which buf[a] we are
    unsigned int a = nbits / 8;
    // Int that tells us how many times we need to shift to the left
    unsigned int shift = 7 - (nbits % 8);
    // Logical OR b AND i
    buf[a] = buf[a] | (i << shift);

    // Increment nbits
    nbits++;
}
