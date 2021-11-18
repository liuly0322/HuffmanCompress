// 此文件用于一些对 Huffman 树操作的静态函数
#ifndef HUFFMAN
#define HUFFMAN

#include <fstream>
#include "../basic_structure/huffman_node.h"

// 打印 Huffman 树
// 只支持一字节(可以是 n 叉树)
// 根结点被 id = 0 调用，自己 id = 1
void show_huffman(h_node* root,
                  int n,
                  int unit_num,
                  int id = 0,
                  int came_from = 0) {
    static int _id;
    static std::ofstream* out;

    if (!id) {  // 初始调用，重置静态变量
        _id = 0;
        out = new std::ofstream("resources/huffman.md");
        if (!out->is_open())
            return;
        *out << "```mermaid" << '\n';
        *out << "graph TD" << '\n';
    }

    if (!root)
        return;

    int id_now = ++_id;

    if (id)
        *out << id << "--" << came_from << "---";

    *out << id_now << "[\"" << root->weight;
    if (root->_data) {
        *out << "<br>[0x";
        for (int i = 0; i < unit_num; i++) {
            // 读取对应位置 16 进制值
            int hex_num = (i % 2) ? (root->_data->data[i / 2] & 0xF)
                                  : ((root->_data->data[i / 2] & 0xF0) >> 4);
            *out << (hex_num < 10 ? (char)('0' + hex_num)
                                  : (char)('A' + hex_num - 10));
        }
        *out << ']';
    }
    *out << "\"]\n";
    if (root->_data)
        return;

    for (int i = 0; i < n; i++) {
        show_huffman(root->children[i], n, unit_num, id_now, i);
    }
    if (!id) {
        *out << "```" << '\n';
        out->close();
    }
}

#endif

// 求 Huffman 树深度
int get_huffman_depth(h_node* root, int n) {
    if (!root)
        return 0;
    int max = 0;
    for (int i = 0; i < n; i++) {
        int children_depth = get_huffman_depth(root->children[i], n);
        if (children_depth > max)
            max = children_depth;
    }
    return max + 1;
}

// 求 Huffman 编码，返回编码长度
// 规定一个 unsigned char 恰好用来存两个数据
int encode_huffman(h_node* leaf, int n, bits& buffer) {
    h_node* parent = leaf->parent;
    if (!parent) {
        return 0;
    }
    int i;
    for (i = 0; i < n; i++) {
        if (parent->children[i] == leaf) {
            break;
        }
    }
    int parent_size = encode_huffman(parent, n, buffer);
    if (parent_size % 2) {
        buffer.data[parent_size / 2] += i;
    } else {
        buffer.data[parent_size / 2] = i << 4;
    }
    return parent_size + 1;
}