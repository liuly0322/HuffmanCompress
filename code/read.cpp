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
    for (int i = 0; i < (unit_num + 1) / 2; i++) {
        data->data[i] = 0;
    }

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
        in >> temp[write_num];
    }

    if (unit_num % 2 && buffer_flag) {
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
    }

    if (in.peek() == EOF)
        return -1;
    return 0;
}