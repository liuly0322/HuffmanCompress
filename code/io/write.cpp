#include "write.h"

write::write(char* path, bits* bits, int unit_num, int branch)
    : out(path),
      data(bits),
      unit_num(unit_num),
      branch(branch),
      buffer(0),
      buffer_size(0) {
    int bit_num[17] = {0, 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4};
    chunk_size = bit_num[branch];
}

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

void write::buffer_write(unsigned char x) {
    buffer_size += chunk_size - 8;
    unsigned short int temp = (buffer << 8) + (x << (8 - buffer_size));
    out << (unsigned char)((temp & 0xFF00) >> 8);
    buffer = temp & 0xFF;
}

void write::buffer_append(unsigned char x) {
    buffer_size += chunk_size;
    buffer += (x << (8 - chunk_size));
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
    // 保证每次 buffer_size 都在 0 到 7 之间
    for (int i = 0; i < size; i++) {
        unsigned char now = data->data[i / 2];
        if (i % 2)
            now &= 0xF;
        else
            now >>= 4;
        // now 按照 chunk_size 大小写入
        if (buffer_size + chunk_size >= 8) {
            buffer_write(now);
        } else {
            buffer_append(now);
        }
    }
    if (end) {
        // 收尾
        out << buffer;
    }
}