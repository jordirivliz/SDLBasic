#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include "BitInputStream.hpp"

using namespace std;
using namespace testing;

TEST(BitInputStreamTests, SIMPLE_TEST) {
    string byteStr = "10000000";
    char byte = (char)stoi(byteStr, nullptr, 2);

    stringstream ss;
    ss.write(&byte, 1);
    BitInputStream bis(ss, 1);

    ASSERT_EQ(1, bis.readBit());
    ASSERT_EQ(0, bis.readBit());

    string byteStr1 = "11101010";
    char byte1 = (char)stoi(byteStr1, nullptr, 2);

    stringstream ss1;
    ss1.write(&byte1, 1);
    BitInputStream bis1(ss1, 1);

    ASSERT_EQ(1, bis1.readBit());
    ASSERT_EQ(1, bis1.readBit());
    ASSERT_EQ(1, bis1.readBit());
    ASSERT_EQ(0, bis1.readBit());
    ASSERT_EQ(1, bis1.readBit());
    ASSERT_EQ(0, bis1.readBit());
    ASSERT_EQ(1, bis1.readBit());
    ASSERT_EQ(0, bis1.readBit());
    bis1.readBit();
    ASSERT_EQ(true, bis1.eof());
    ASSERT_EQ(0, bis1.getNbits());

    string byteStr2 = "10101010";
    char byte2 = (char)stoi(byteStr2, nullptr, 2);

    stringstream ss2;
    ss2.write(&byte2, 1);
    BitInputStream bis2(ss2, 1);

    ASSERT_EQ(1, bis2.readBit());
    ASSERT_EQ(0, bis2.readBit());
    ASSERT_EQ(false, bis2.eof());

    string byteStr3 = "101";
    char byte3 = (char)stoi(byteStr3, nullptr, 2);
    stringstream ss3;
    ss3.write(&byte3, 1);
    BitInputStream bis3(ss3, 1);
    ASSERT_EQ(0, bis3.readBit());
    ASSERT_EQ(0, bis3.readBit());
    ASSERT_EQ(0, bis3.readBit());
}