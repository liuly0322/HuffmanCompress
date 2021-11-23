#ifndef DEREAD
#define DEREAD

#include <fstream>
#include <iostream>

#include "../basic_structure/huffman_node.h"

class de_read {
   private:
    std::ifstream in;
    unsigned char buffer;  // 每次读入的字符
    int buffer_point;      // 即将读取现在字符的第几位

    int chunk_size;
    int unit;

   public:
    de_read(char* path,
            int& unit_num,
            int& branch,
            int& node_num,
            int& half_bits,
            bits*& unfinished);
    ~de_read();

    void read_node(h_node* node);
    int read_branch();
};

#endif