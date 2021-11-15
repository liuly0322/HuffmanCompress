#include <fstream>
#include <iostream>
#include "p_queue.h"
#include "table.h"

int main(int argc, char** argv) {
    // so we have to check if there is any setting made
    // 先写一个 demo 版本，压缩 1.txt，要求能输出二叉 Huffman 树的结点信息
    table table(256, 2, 2);  // 创建二叉 1字节为基本单位的表
    // 先测一下 Hash 表能不能用
    std::ifstream in("resources/1.txt");
    if (in.is_open()) {
        while (in.peek() != EOF) {
            // 开一个 char
            bits b(1);
            unsigned char temp;
            in >> temp;
            b.data[0] = temp;
            table[b]++;
        }
    }
    // 希望已经 Hash 表统计完了？ 大概吧xd
    table.iterator_clear();
    h_node* temp;
    while (temp = table.iterator_visit()) {
        // 根据 temp 返回读取的字节和权
        std::cout << temp->_data->data[0] << ' ' << temp->weight;
    }
    return 0;
}

// 下一步：
// 可以考虑拆分出读取操作，传入文件名，每次读取字节后转换成基本的bits形式
// bits 在 main.cpp 中创建，然后指针（引用？）形式，传给 read 静态类的构造函数
// 然后每次读取，交给哈希表统计
// 统计完通过迭代器交给优先队列构造 Huffman 树
// 然后再次读取文件，这次每次的 bits 转换成对应编码，然后交给 write 类写入