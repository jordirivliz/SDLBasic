#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

#include "HCTree.hpp"

using namespace std;
using namespace testing;

class SimpleHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree;
    HCTree tree1;
    HCTree tree2;
    HCTree tree3;
    HCTree tree4;
    HCTree tree5;
    HCTree tree6;
    HCTree tree7;
    HCTree tree8;

  public:
    SimpleHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        freqs['a'] = 2;
        freqs['b'] = 3;
        tree.build(freqs);

        vector<unsigned int> freqs1(256);
        freqs1['a'] = 6;
        freqs1['b'] = 4;
        freqs1['c'] = 4;
        freqs1['g'] = 1;
        freqs1['h'] = 2;
        tree1.build(freqs1);

        vector<unsigned int> freqs2(256);
        freqs2['\n'] = 2;
        freqs2['a'] = 3;
        tree2.build(freqs2);

        vector<unsigned int> freqs3(256);
        freqs3['\n'] = 8;
        tree3.build(freqs3);

        vector<unsigned int> freqs4(256);
        freqs4['a'] = 2;
        freqs4['b'] = 2;
        tree4.build(freqs4);

        vector<unsigned int> freqs5(256);
        freqs5['a'] = 2;
        freqs5['b'] = 3;
        freqs5['c'] = 4;
        freqs5['\n'] = 8;
        tree5.build(freqs5);

        vector<unsigned int> freqs6(256);
        freqs6[' '] = 4;
        freqs['a'] = 3;
        tree6.build(freqs6);

        vector<unsigned int> freqs7(256);
        freqs7['a'] = 2;
        freqs7['\n'] = 1;
        // freqs7['b'] = 3;
        tree7.build(freqs7);
    }
};

TEST_F(SimpleHCTreeFixture, TEST_GETROOT) {
    HCNode* node = tree.getRoot();
    HCNode* node1 = tree1.getRoot();
    HCNode* node2 = tree2.getRoot();
    HCNode* node3 = tree3.getRoot();
    HCNode* node4 = tree4.getRoot();
    HCNode* node5 = tree5.getRoot();

    ASSERT_EQ(node1->count, 17);
    ASSERT_EQ((node1->c0)->count, 7);

    ASSERT_EQ(node->count, 5);
    ASSERT_EQ((node->c0)->count, 2);

    ASSERT_EQ(node2->symbol, '\n');
    ASSERT_EQ((node2->c1)->symbol, 'a');
    ASSERT_EQ((node2->c0)->symbol, '\n');

    ASSERT_EQ(node3->symbol, '\n');
    ASSERT_EQ(node3->count, 8);

    ASSERT_EQ(node4->symbol, 'b');
    ASSERT_EQ(node4->count, 4);

    ASSERT_EQ(node5->symbol, '\n');
    ASSERT_EQ(node5->count, 17);
}

TEST_F(SimpleHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    tree.encode('a', os);
    ASSERT_EQ(os.str(), "0");

    ostringstream os1;
    tree1.encode('a', os1);
    ASSERT_EQ(os1.str(), "11");

    ostringstream os2;
    tree1.encode('b', os2);
    ASSERT_EQ(os2.str(), "10");

    ostringstream os3;
    tree1.encode('h', os3);
    ASSERT_EQ(os3.str(), "001");

    ostringstream os4;
    tree2.encode('\n', os4);
    ASSERT_EQ(os4.str(), "0");

    ostringstream os5;
    tree3.encode('\n', os5);
    ASSERT_EQ(os5.str(), "0");

    ostringstream os6;
    tree4.encode('a', os6);
    ASSERT_EQ(os6.str(), "1");

    ostringstream os7;
    tree5.encode('b', os7);
    ASSERT_EQ(os7.str(), "111");

    ostringstream os8;
    tree6.encode(' ', os8);
    ASSERT_EQ(os8.str(), "0");

    ostringstream os9;
    tree8.encode('a', os9);
    ASSERT_EQ(os9.str(), "");
}

TEST_F(SimpleHCTreeFixture, TEST_DECODE) {
    istringstream is("1");
    ASSERT_EQ(tree.decode(is), 'b');

    istringstream is1("01");
    ASSERT_EQ(tree1.decode(is1), 'c');

    istringstream is2("000");
    ASSERT_EQ(tree1.decode(is2), 'g');

    istringstream is3("10");
    ASSERT_EQ(tree1.decode(is3), 'b');

    istringstream is4("0");
    ASSERT_EQ(tree2.decode(is4), '\n');

    istringstream is5("0");
    ASSERT_EQ(tree3.decode(is5), '\n');

    istringstream is6("0");
    ASSERT_EQ(tree4.decode(is6), 'b');

    istringstream is7("0");
    ASSERT_EQ(tree5.decode(is7), '\n');

    istringstream is8("0");
    ASSERT_EQ(tree6.decode(is8), ' ');

    istringstream is9("0");
    ASSERT_EQ(tree8.decode(is9), '\0');
}
TEST_F(SimpleHCTreeFixture, TEST_ENCODEBIT) {
    stringstream os;
    BitOutputStream outputStream(os, 1);
    tree7.encode('a', outputStream);
    tree7.encode('\n', outputStream);
    tree7.encode('a', outputStream);
    outputStream.flush();

    BitInputStream bis(os, 1);
    os.seekg(0, std::ios::beg);

    ASSERT_EQ(1, bis.readBit());
    ASSERT_EQ(0, bis.readBit());
    ASSERT_EQ(1, bis.readBit());
}
TEST_F(SimpleHCTreeFixture, TEST_ENCODEBIT1) {
    vector<unsigned int> freqs(256);
    freqs['a'] = 6;
    freqs['b'] = 3;
    freqs['c'] = 4;

    HCTree tree;
    tree.build(freqs);

    stringstream os;
    BitOutputStream outputStream(os, 1);

    tree.encode('a', outputStream);
    tree.encode('b', outputStream);
    tree.encode('c', outputStream);

    outputStream.flush();

    BitInputStream bis(os, 1);
    os.seekg(0, std::ios::beg);

    ASSERT_EQ(0, bis.readBit());
    ASSERT_EQ(1, bis.readBit());
    ASSERT_EQ(0, bis.readBit());
    ASSERT_EQ(1, bis.readBit());
    ASSERT_EQ(1, bis.readBit());
}

TEST_F(SimpleHCTreeFixture, TEST_ENCODEBIT2) {
    vector<unsigned int> freqs(256);
    freqs['\n'] = 4;

    HCTree tree;
    tree.build(freqs);

    stringstream os;
    BitOutputStream outputStream(os, 1);

    tree.encode('\n', outputStream);
    outputStream.flush();

    BitInputStream bis(os, 1);
    os.seekg(0, std::ios::beg);

    ASSERT_EQ(0, bis.readBit());
}

TEST_F(SimpleHCTreeFixture, TEST_DECODEBIT) {
    istringstream is("0");
    BitInputStream inputStream(is, 1);
    ASSERT_EQ(tree7.decode(inputStream), '\n');
}

TEST_F(SimpleHCTreeFixture, TEST_DECODEBIT1) {
    vector<unsigned int> freqs(256);
    freqs['a'] = 1;
    freqs['b'] = 2;
    freqs['c'] = 4;

    HCTree tree;
    tree.build(freqs);

    // d = 0110 0100
    istringstream is1("d");
    BitInputStream inputStream1(is1, 1);
    ASSERT_EQ(tree.decode(inputStream1), 'b');
    ASSERT_EQ(tree.decode(inputStream1), 'c');
    ASSERT_EQ(tree.decode(inputStream1), 'a');
    ASSERT_EQ(tree.decode(inputStream1), 'c');
    ASSERT_EQ(tree.decode(inputStream1), 'a');
    tree.decode(inputStream1);
    ASSERT_EQ(inputStream1.eof(), true);
}

TEST_F(SimpleHCTreeFixture, TEST_DECODEBIT2) {
    vector<unsigned int> freqs(256);
    freqs['a'] = 4;

    HCTree tree;
    tree.build(freqs);

    // d = 0110 0100
    istringstream is1("d");
    BitInputStream inputStream1(is1, 1);
    ASSERT_EQ(tree.decode(inputStream1), 'a');
}

TEST_F(SimpleHCTreeFixture, TEST_DECODEBIT3) {
    vector<unsigned int> freqs(256);
    freqs['\n'] = 4;

    HCTree tree;
    tree.build(freqs);

    // d = 0110 0100
    istringstream is1("d");
    BitInputStream inputStream1(is1, 1);
    ASSERT_EQ(tree.decode(inputStream1), '\n');
}

TEST_F(SimpleHCTreeFixture, TEST_DECODEBIT4) {
    istringstream is("0");
    BitInputStream inputStream(is, 1);
    ASSERT_EQ(tree8.decode(inputStream), '\0');
}