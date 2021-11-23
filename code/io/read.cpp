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
    unsigned char temp[4] = {0};
    // 需要读入几个字节
    int read_num = unit_num / 2;
    if (!buffer_flag && unit_num % 2)
        read_num++;

    int write_num;  // 实际写入几个字节
    for (write_num = 0; write_num < read_num; write_num++) {
        if (in.peek() == EOF)
            break;
        in.get((char&)temp[write_num]);
    }

    // 写到 data 里
    if (unit_num % 2) {
        if (buffer_flag) {
            // 需要与 buffer 合并
            data->data[0] = (buffer << 4) + (temp[0] >> 4);
            for (int i = 1; i <= read_num; i++) {
                data->data[i] = (temp[i - 1] << 4) + (temp[i] >> 4);
            }
            buffer_flag = false;
        } else {
            for (int i = 0; i < read_num - 1; i++) {
                data->data[i] = temp[i];
            }
            data->data[read_num - 1] = temp[read_num - 1] & 0xF0;
            // 最后一个的后半部分给 buffer
            buffer_flag = true;
            buffer = temp[read_num - 1] & 0xF;
        }
    } else {
        for (int i = 0; i < read_num; i++) {
            data->data[i] = temp[i];
        }
    }

    if (read_num != write_num) {
        // 计算返回值
        if (unit_num % 2) {
            if (buffer_flag) {
                return (write_num - read_num) * 2;
            } else {
                return (write_num - read_num) * 2 - 1;
            }
        } else {
            return (write_num - read_num) * 2;
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