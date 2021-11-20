#ifndef WRITE
#define WRITE

#include <fstream>
#include "../basic_structure/huffman_node.h"

class write {
   private:
    std::ofstream out;

    bits* data;
    int unit_num;
    int branch;

    unsigned char buffer;
    int buffer_size;

    void write_num(int x);

   public:
    write(char* path, bits* bits, int unit_num, int branch);
    ~write();

    void before_head(int zero_num, int node_num, bits* reserve);
    void write_head(h_node* node);
    void write_body(int size, bool end);
};

#endif