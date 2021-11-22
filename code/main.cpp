#include <cstdlib>
#include <iostream>
#include "adt/p_queue.h"
#include "adt/table.h"
#include "huffman_function/huffman.h"
#include "io/decompress_read.h"
#include "io/decompress_write.h"
#include "io/read.h"
#include "io/write.h"
// #define DEBUG

void compress(char* in, char* out, int branch, int unit_num, bool display) {
    int hash_size[9] = {0,      16,     256,    4096,   65536,
                        131072, 262144, 524288, 1048576};
    table table(hash_size[unit_num], branch,
                unit_num);  // 创建二叉 1字节为基本单位的表

    bits read_buffer((unit_num + 1) / 2);  // 基本储存大小为 1 字节

    read read(in, &read_buffer, unit_num);  // 对应 2 个半字节

    int zero_num = 0;  // 补 0 数
    while (!(zero_num = read.read_to_end())) {
        table[read_buffer]++;
    }
    if (zero_num == -1)
        zero_num = 0;

    int node_num = table.get_ele_num();
    int empty_node_num = (-node_num) % (branch - 1);
    if (empty_node_num < 0)
        empty_node_num += branch - 1;

    // 还要包含一个逸出码 NYT 结点
    p_queue p_queue(node_num + empty_node_num + 1);

    h_node* nyt = new h_node();
    nyt->_data = &read_buffer;
    p_queue.enqueue(nyt);

    for (int i = 0; i < empty_node_num; i++) {
        h_node* empty_node = new h_node();
        p_queue.enqueue(empty_node);
    }

    table.iterator_clear();  // 迭代器遍历表中所有结点
    h_node* temp;
    while ((temp = table.iterator_visit())) {
        p_queue.enqueue(temp);
    }

    // 此时 Huffman 树建立完毕，father 是根结点
    h_node** nodes = new h_node*[branch]();
    h_node* father;
    while (true) {
        for (int i = 0; i < branch; i++) {
            nodes[i] = p_queue.dequeue();
        }
        father = new h_node(branch, nodes);
        if (p_queue.empty()) {
            break;
        }
        p_queue.enqueue(father);
    }

    if (display)
        show_huffman(father, branch, unit_num);

    bits write_buffer(get_huffman_depth(father, branch) / 2);
    write write(out, &write_buffer, unit_num, branch);

    write.before_head(zero_num, node_num, &read_buffer);

    do {
        temp = table.iterator_visit();
        write.write_head(temp);
    } while (temp);

    read.clear();
    while (!read.read_to_end()) {
        write.write_body(
            encode_huffman(table.get_h_node(read_buffer), branch, write_buffer),
            false);
    }
    write.write_body(encode_huffman(nyt, branch, write_buffer), true);
}

void uncompress(char* in, char* out, bool display) {
    int unit_num, branch, node_num;

    int half_zero_bits;
    bits* unfinished_bits;

    de_read read(in, unit_num, branch, node_num, half_zero_bits,
                 unfinished_bits);
    // 文件头前置内容读取完毕，现在建立 Huffman 树，读取 Huffman 编码
}

int main(int argc, char** argv) {
#ifndef DEBUG
    int state = 0, in_index = 0, out_index = 0;
    bool compress_mode = false, uncompress_mode = false, display_mode = false;
    int branch = 2, unit_num = 2;  // 压缩时需要指定，解压缩不需要

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            state++;
            if (state == 1)
                in_index = i;
            if (state == 2)
                out_index = i;
            continue;
        }
        if (argv[i][1] == 'c') {
            compress_mode = true;
            continue;
        }
        if (argv[i][1] == 'u' && argv[i][2] == 'n') {
            uncompress_mode = true;
            continue;
        }
        if (argv[i][1] == 'd') {
            display_mode = true;
        }
        if (argv[i][1] == 'b') {
            branch = atoi(argv[i] + 2);
            if (branch < 2 || branch > 16) {
                std::cout << "指定 n 叉树的 n 不符合规范，按默认 2 处理";
                branch = 2;
            }
        }
        if (argv[i][1] == 'u') {
            unit_num = atoi(argv[i] + 2);
            if (unit_num < 1 || unit_num > 8) {
                std::cout
                    << "所指定的基本压缩单位半字节数不符合规范，按默认 2 处理";
                unit_num = 2;
            }
        }
    }

    if (!in_index || !out_index) {
        std::cout << "请依次输入目标文件路径和生成文件路径！";
    }

    if (compress_mode) {
        compress(argv[in_index], argv[out_index], branch, unit_num,
                 display_mode);
    } else if (uncompress_mode) {
        uncompress(argv[in_index], argv[out_index], display_mode);
    } else {
        std::cout << "请指定 -c 压缩或 -un 解压";
    }
#endif
#ifdef DEBUG
    compress("resources/1.txt", "resources/huffman_new.md", 4, 6, true);
#endif
}