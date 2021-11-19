#ifndef HUFFMAN_NODE
#define HUFFMAN_NODE

#include "bits.h"

typedef struct huffman_node {
    bits* _data;
    unsigned int weight;
    huffman_node* parent;
    huffman_node** children;
    huffman_node() {
        _data = nullptr;
        weight = 0;
        parent = nullptr;
        children = nullptr;
    }
    huffman_node(int n, int size, bits& data) {
        parent = nullptr;
        children = new huffman_node*[n]();
        weight = 0;
        _data = new bits(size, data);
    }
    huffman_node(int n, huffman_node** nodes) {
        _data = nullptr;
        parent = nullptr;
        weight = 0;
        children = new huffman_node*[n]();
        for (int i = 0; i < n; i++) {
            weight += nodes[i]->weight;
            children[i] = nodes[i];
            nodes[i]->parent = this;
        }
    }
} h_node;

#endif
