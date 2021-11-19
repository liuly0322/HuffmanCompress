#include "read.h"

read::read(char* path, bits* bits, int unit_num)
    : in(path), data(bits), unit_num(unit_num), buffer_flag(false) {
    if (!in.is_open()) {
        std::cout << "invalid file";
        exit(0);
    }
}

read::~read() {}

// 每次读取 unit_num 个基本单位，如果需要补 0，返回补的基本单元数目
// 返回 -1 表示正常读取完毕
int read::read_to_end() {
    static unsigned char temp[4];

    int write_num, read_num;
    if (!buffer_flag && (unit_num % 2)) {
        read_num = (unit_num + 1) / 2;
    } else {
        read_num = unit_num / 2;
    }

    for (write_num = 0; write_num < read_num; write_num++) {
        if (in.peek() == EOF)
            break;
        in.get((char&)temp[write_num]);
    }

    if (unit_num % 2 && buffer_flag) {
        buffer_flag = false;
        for (int j = write_num; j; j--) {
            temp[j] = temp[j - 1];
        }
        temp[0] = buffer;
        for (int j = 0; j <= write_num; j++) {
            data->data[j] = ((temp[j] & 0xF) << 4) +
                            (j == write_num ? 0 : ((temp[j + 1] & 0xF0) >> 4));
        }
        if (write_num != read_num)
            return unit_num - (2 * write_num + 1);
    } else {
        for (int j = 0; j < write_num; j++) {
            data->data[j] = temp[j];
        }
        if (write_num != read_num)
            return unit_num - 2 * write_num;
        if (unit_num % 2) {
            data->data[read_num - 1] &= 0xF0;
            buffer = temp[read_num - 1] & 0xF;
            buffer_flag = true;
        }
    }

    if (in.peek() == EOF)
        return -1;
    return 0;
}

// 重置文件指针，用于再次读取文件
void read::clear() {
    in.clear();
    in.seekg(0, std::ios::beg);
}