#include "decompress_read.h"

de_read::de_read(char* path,
                 int& unit_num,
                 int& branch,
                 int& node_num,
                 int& half_bits,
                 bits*& unfinished)
    : in(path), buffer(0), buffer_point(-1) {
    if (!in.is_open()) {
        std::cout << "invalid file";
        exit(0);
    }

    char temp;
    in.get(temp);
    unit_num = (temp & 0xF0) >> 4;
    unit = unit_num;
    branch = temp & 0xF;

    int bit_num[17] = {0, 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4};
    chunk_size = bit_num[branch];

    in.get(temp);
    half_bits = temp;

    unfinished = new bits((unit_num + 1) / 2);
    for (int i = 0; i < (unit_num + 1) / 2; i++) {
        in.get((char&)unfinished->data[i]);
    }

    in.get(temp);
    node_num = 0;
    for (char i = 0; i < temp; i++) {
        char num_to_join;
        in.get(num_to_join);
        node_num += num_to_join << 8 * (temp - i - 1);
    }
}

void de_read::read_node(h_node* node) {
    char node_num_bytes;
    in.get(node_num_bytes);
    node->weight = 0;
    for (char i = 0; i < node_num_bytes; i++) {
        char num_to_join;
        in.get(num_to_join);
        node->weight += num_to_join << 8 * (node_num_bytes - i - 1);
    }
    for (int i = 0; i < (unit + 1) / 2; i++) {
        in.get((char&)node->_data->data[i]);
    }
}

int de_read::read_branch() {
    if (buffer_point == -1) {
        in.get(buffer);
        buffer_point = 0;
    }
    if (buffer_point + chunk_size < 8) {
        // 直接截取当前的 buffer 返回
        buffer_point += chunk_size;
        return (buffer >> (8 - buffer_point)) & ((1 << chunk_size) - 1);
    }
    // 否则需要更新 buffer
    unsigned char next;
    in.get((char&)next);
    buffer_point += chunk_size - 8;
    int ret = (((buffer << 8) + next) >> (8 - buffer_point)) &
              ((1 << chunk_size) - 1);

    buffer = next;
    return ret;
}

de_read::~de_read() {}