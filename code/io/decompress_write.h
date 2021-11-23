#ifndef DEWRITE
#define DEWRITE

#include <fstream>
#include "../basic_structure/bits.h"

class de_write {
   private:
    std::ofstream out;

    int unit_num;
    unsigned char buffer;  // 高 4 位代表缓冲区
    bool buffer_flag;

   public:
    de_write(char* path, int unit_num);
    ~de_write();

    void write_bits(const bits* bits);
    void write_end(const bits* bits, int zero_half_byte);
};

#endif