#include "decompress_read.h"

de_read::de_read(char* path,
                 int& unit_num,
                 int& branch,
                 int& node_num,
                 int& half_bits,
                 bits*& unfinished)
    : in(path) {
    if (!in.is_open() || in.peek() == EOF) {
        std::cout << "invalid file";
        exit(0);
    }

    char temp;
    in.get(temp);
    unit_num = (temp & 0xF0) >> 4;
    branch = temp & 0xF;

    if (in.peek() == EOF) {
        std::cout << "invalid file";
        exit(0);
    }
    in.get(temp);
    half_bits = temp;

    unfinished = new bits((unit_num + 1) / 2);
    for (int i = 0; i < (unit_num + 1) / 2; i++) {
        if (in.peek() == EOF) {
            std::cout << "invalid file";
            exit(0);
        }
        in.get((char&)unfinished->data[i]);
    }

    if (in.peek() == EOF) {
        std::cout << "invalid file";
        exit(0);
    }
    in.get(temp);
    node_num = 0;
    for (char i = 0; i < temp; i++) {
        if (in.peek() == EOF) {
            std::cout << "invalid file";
            exit(0);
        }
        char num_to_join;
        in.get(num_to_join);
        node_num += num_to_join << 8 * (temp - i - 1);
    }
}

de_read::~de_read() {}