#include "decompress_write.h"

de_write::de_write(char* path, int unit_num)
    : out(path), unit_num(unit_num), buffer(0), buffer_flag(false) {}

void de_write::write_bits(const bits* bits) {
    if (unit_num % 2) {
        if (!buffer_flag) {
            // 最后一个计入 buffer 等待
            for (int i = 0; i < unit_num / 2; i++) {
                out << bits->data[i];
            }
            buffer = bits->data[unit_num / 2];
            buffer_flag = true;
        } else {
            out << (unsigned char)(buffer + (bits->data[0] >> 4));
            for (int i = 1; i <= unit_num / 2; i++) {
                out << (unsigned char)((bits->data[i] >> 4) +
                                       (bits->data[i - 1] << 4));
            }
            buffer_flag = false;
        }
    } else {
        for (int i = 0; i < unit_num / 2; i++) {
            out << bits->data[i];
        }
    }
}

void de_write::write_end(const bits* bits, int zero_half_byte) {
    if (unit_num % 2) {
        if (!buffer_flag) {
            for (int i = 0; i < (unit_num - zero_half_byte) / 2; i++) {
                out << bits->data[i];
            }
        } else {
            // 拼接后再输出
            out << (unsigned char)(buffer + (bits->data[0] >> 4));
            for (int i = 1; i <= (unit_num - zero_half_byte) / 2; i++) {
                out << (unsigned char)((bits->data[i] >> 4) +
                                       (bits->data[i - 1] << 4));
            }
        }
    } else {
        for (int i = 0; i < (unit_num - zero_half_byte) / 2; i++) {
            out << bits->data[i];
        }
    }
    out.close();
}

de_write::~de_write() {}