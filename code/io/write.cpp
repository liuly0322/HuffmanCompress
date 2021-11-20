#include "write.h"

write::write(char* path, bits* bits, int unit_num, int branch)
    : out(path),
      data(bits),
      unit_num(unit_num),
      branch(branch),
      buffer(0),
      buffer_size(0) {}

write::~write() {}

void write::write_num(int x) {
    static unsigned char temp[4];
    unsigned char k;
    for (k = 0; x; k++) {
        temp[k] = x % 0x100;
        x /= 0x100;
    }
    out << k;
    while (k) {
        out << temp[--k];
    }
}

void write::before_head(int zero_num, int node_num, bits* reserve) {
    out << (unsigned char)((unit_num << 4) + branch) << (unsigned char)zero_num;
    for (int i = 0; i < (unit_num + 1) / 2; i++)
        out << reserve->data[i];
    write_num(node_num);
}

void write::write_head(h_node* node) {
    if (!node) {
        out << (unsigned char)0;
        return;
    }
    write_num(node->weight);
    for (int i = 0; i < (unit_num + 1) / 2; i++)
        out << node->_data->data[i];
}

// 写入直到末尾，当标识 end 的时候（此时写入的是 nyt），补0，清空 buff
void write::write_body(int size, bool end) {
    // size 编码长度
    // buffer 和 buffer_size 是还没被写入的
    // 保证每次 buffer_size 都在 0 到 7 之间，满了就写入
    // 现在情况是可以得到 huffman 编码（0到n-1的数字序列）
}