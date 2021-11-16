// 此文件用于一些对 Huffman 树操作的静态函数
#ifndef HUFFMAN
#define HUFFMAN

#include <fstream>
#include "huffman_node.h"

// 打印 Huffman 树
// 只支持一字节(可以是 n 叉树)
// 根结点被 id = 0 调用，自己 id = 1
void show_huffman(h_node* root, int n, int id = 0, int came_from = 0) {
    static int _id;
    static std::ofstream* out;

    if (!id) {  // 初始调用，重置静态变量
        _id = 0;
        // 此时打开文件
        out = new std::ofstream("resources/huffman.md");
        if (!out->is_open())
            return;
        // 写入一些 md 流程图需要的
        *out << "```mermaid" << '\n';
        *out << "graph TD" << '\n';
    }

    int id_now = ++_id;

    if (id)
        *out << id << "--" << came_from << "---";

    // 非叶子结点 1[weight + ]\n
    // 叶子结点 2[123  +  c-96 + ]\n
    *out << id_now << '[' << root->weight;
    if (root->_data)
        *out << " c-" << (unsigned int)root->_data->data[0];
    *out << "]\n";
    if (root->_data)
        return;

    for (int i = 0; i < n; i++) {
        show_huffman(root->children[i], n, id_now, i);
    }
    if (!id) {
        // 此时关闭文件
        *out << "```" << '\n';
        out->close();
    }
}

#endif
