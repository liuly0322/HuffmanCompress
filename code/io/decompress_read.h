#ifndef DEREAD
#define DEREAD

#include <fstream>
#include <iostream>

#include "../basic_structure/bits.h"

class de_read {
   private:
    std::ifstream in;

   public:
    de_read(char* path,
            int& unit_num,
            int& branch,
            int& node_num,
            int& half_bits,
            bits*& unfinished);
    ~de_read();
};

#endif