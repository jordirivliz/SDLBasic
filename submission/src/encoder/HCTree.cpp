#include "HCTree.hpp"

#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
using namespace std;

/**
 * Method that retrives the root
 */
HCNode* HCTree::getRoot() { return root; }
/**
 * Helper function for deleter
 */
void HCTree::deleteHelper(HCNode* node) {
    // Check if node is null
    if (node == nullptr) {
        return;
    }
    // Traverse c0 child if it does have one
    if (node->c0 != nullptr) {
        deleteHelper(node->c0);
    }
    // Traverse c1 child if it does have one
    if (node->c1 != nullptr) {
        deleteHelper(node->c1);
    }
    delete node;
}
HCTree::HCTree() {
    root = nullptr;
    leaves = vector<HCNode*>(256, 0);
}

/* TODO: Delete all objects on the heap to avoid memory leaks. */
HCTree::~HCTree() { deleteHelper(root); }

/**
 * TODO: Build the HCTree from the given frequency vector. You can assume the
 * vector must have size 256 and each value at index i represents the frequency
 * of char with ASCII value i. Only non-zero frequency symbols should be used to
 * build the tree. The leaves vector must be updated so that it can be used in
 * encode() to improve performance.
 *
 * When building the HCTree, you should use the following tie-breaking rules to
 * match the output from reference solution in checkpoint:
 *
 *    1. HCNode with lower count should have higher priority. If count is the
 * same, then HCNode with a larger ascii value symbol should have higher
 * priority. (This should be already defined properly in the comparator in
 * HCNode.hpp)
 *    2. When popping two highest priority nodes from PQ, the higher priority
 * node will be the ‘c0’ child of the new parent HCNode.
 *    3. The symbol of any parent node should be taken from its 'c0' child.
 */
void HCTree::build(const vector<unsigned int>& freqs) {
    // Create a forest to store frequencies with its symbols

    // Priority queue that will store nodes
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> prioQueue;
    for (int i = 0; i < 256; i++) {
        // Push all leaves to the queue and to the leaves vector
        if (freqs[i] != 0) {
            HCNode* node =
                new HCNode(freqs[i], (unsigned char)i, nullptr, nullptr);
            leaves[node->symbol] = node;
            prioQueue.push(node);
        }
    }
    if (prioQueue.size() == 1) {
        HCNode* child0 = prioQueue.top();
        // prioQueue.pop();
        root = new HCNode(child0->count, child0->symbol, nullptr, nullptr);
        return;
    }
    // While there is more than 1 tree in the forest
    while (prioQueue.size() > 1) {
        // Remove two trees (T1 and T2) with lowest frecuency from the forest
        HCNode* child0 = prioQueue.top();
        prioQueue.pop();
        HCNode* child1 = prioQueue.top();
        prioQueue.pop();

        // Create a new node with frequency = T1's +T2's to be their parent
        int newFreq = child0->count + child1->count;
        HCNode* parent = new HCNode(newFreq, child0->symbol, child0, child1);
        child0->p = parent;
        child1->p = parent;
        prioQueue.push(parent);
    }
    // Root points to the top of the queue
    root = prioQueue.top();
}

/**
 * TODO: Write the encoding bits of the given symbol to the ostream. You should
 * write each encoding bit as ascii char either '0' or '1' to the ostream. You
 * must not perform a comprehensive search to find the encoding bits of the
 * given symbol, and you should use the leaves vector instead to achieve
 * efficient encoding. For this function to work, build() must be called before
 * to create the HCTree.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
    string coded;
    // Access the leave associated to the symbol
    HCNode* node = leaves[symbol];
    // While the node is not equal to the root keep going up
    while (node != root) {
        // Access parent of node
        HCNode* parent = node->p;
        if (parent == nullptr) {
            coded = "0";
            char c = coded[0];
            int num = c - '0';
            out.writeBit(num);
            return;
        }
        // Add a 0 if the node was c0
        if (parent->c0 == node) {
            coded = "0" + coded;
        }
        // Add a 1 if node was c1
        else {
            coded = "1" + coded;
        }
        node = parent;
    }
    for (int i = 0; i < coded.length(); i++) {
        char c = coded[i];
        int num = c - '0';
        out.writeBit(num);
    }
}

/**
 * TODO: Write the encoding bits of the given symbol to ostream. You should
 * write each encoding bit as ascii char either '0' or '1' to the ostream. You
 * must not perform a comprehensive search to find the encoding bits of the
 * given symbol, and you should use the leaves vector instead to achieve
 * efficient encoding. For this function to work, build() must have been called
 * beforehand to create the HCTree.
 */
void HCTree::encode(byte symbol, ostream& out) const {
    // String that contains the coded sequence of 0 and 1
    string coded;

    // Access the leave associated to the symbol
    HCNode* node = leaves[symbol];
    if (node == nullptr) {
        return;
    }
    // While the node is not equal to the root keep going up
    while (node != root) {
        // Access parent of node
        HCNode* parent = node->p;
        if (parent == nullptr) {
            coded = "0";
            out << coded;
            return;
        }
        // Add a 0 if the node was c0
        if (parent->c0 == node) {
            coded = "0" + coded;
        }
        // Add a 1 if node was c1
        else {
            coded = "1" + coded;
        }
        node = parent;
    }
    // Write the string to the ostream
    out << coded;
}

/**
 * TODO: Decode the sequence of bits (represented as a char of either '0' or
 * '1') from the istream to return the coded symbol. For this function to work,
 * build() must have been called beforehand to create the HCTree.
 */
byte HCTree::decode(BitInputStream& in) const {
    byte result;
    // Access the root
    HCNode* node = root;
    if (node == nullptr) {
        return result;
    }
    if (node->c0 == nullptr && node->c1 == nullptr) {
        unsigned data = in.readBit();
        result = node->symbol;
    }
    // Traverse down the tree until we reach a leave
    while (node->c0 != nullptr && node->c1 != nullptr) {
        // Read data from the istream until we reach a leave
        unsigned int data = in.readBit();
        if (in.eof()) {
            break;
        }
        // If data is 0 then go left to c0
        if (data == 0) {
            node = node->c0;
        }
        // Data is 1, then traverse right to c1
        else {
            node = node->c1;
        }
    }
    result = node->symbol;
    return result;
}

/**
 * TODO: Decode the sequence of bits (represented as char of either '0' or '1')
 * from istream to return the coded symbol. For this function to work, build()
 * must have been called beforehand to create the HCTree.
 */
byte HCTree::decode(istream& in) const {
    byte result;
    // Access the root
    HCNode* node = root;
    if (node == nullptr) {
        return result;
    }
    if (node->c0 == nullptr && node->c1 == nullptr) {
        int data = in.get();
        result = node->symbol;
    }
    // Traverse down the tree until we reach a leave
    while (node->c0 != nullptr && node->c1 != nullptr) {
        // Read data from the istream until we reach a leave
        int data = in.get();
        if (data == -1) {
            break;
        }
        // If char is 0 then go left to c0
        if (data == '0') {
            node = node->c0;
        }
        // Char is 1, then traverse right to c1
        else {
            node = node->c1;
        }
    }
    result = node->symbol;
    return result;
}
