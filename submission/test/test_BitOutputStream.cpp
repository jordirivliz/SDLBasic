#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include "BitOutputStream.hpp"

using namespace std;
using namespace testing;

TEST(BitOutputStreamTests, SIMPLE_TEST) {
    stringstream ss;
    BitOutputStream bos(ss, 1);
    bos.writeBit(1);
    bos.flush();

    stringstream ss1;
    BitOutputStream bos1(ss1, 1);
    bos1.writeBit(1);
    bos1.writeBit(0);
    bos1.writeBit(1);
    bos1.writeBit(0);
    bos1.flush();

    string bitsStr = "10000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);

    string bitsStr1 = "10100000";
    unsigned int asciiVal1 = stoi(bitsStr1, nullptr, 2);
    ASSERT_EQ(ss1.get(), asciiVal1);
}
