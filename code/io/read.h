#ifndef READ
#define READ

#include <fstream>
#include <iostream>
#include "../basic_structure/bits.h"

class read {
   private:
    std::ifstream in;

    bits* data;
    int unit_num;  // 基本单元数

    unsigned char buffer;  // 低 4 位存储可能需要的半字节
    bool buffer_flag;

   public:
    read(char* path, bits* bits, int unit_num);
    ~read();
    int read_to_end();  // 读取直到末尾，返回末尾有几个是空置的
};

#endif