#ifndef HUFFMAN_NODE
#define HUFFMAN_NODE

#include "bits.h"

typedef struct huffman_node {
    bits* _data;
    unsigned int weight;
    huffman_node* parent;
    huffman_node** children;
    huffman_node(int n, int size, bits& data) {
        parent = nullptr;
        children = new huffman_node*[n]();
        weight = 0;
        _data = new bits(size, data);
    }
} h_node;

#endif