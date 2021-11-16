#include <iostream>
#include "p_queue.h"
#include "read.h"
#include "table.h"
#include "write.h"

int main(int argc, char** argv) {
    table table(256, 2, 2);  // 创建二叉 1字节为基本单位的表
    bits bits(1);            // 基本储存大小为 1 字节
    read read("resources/1.txt", &bits, 2);  // 对应 2 个半字节
    int zero_num = 0;                        // 补 0 数
    while (!(zero_num = read.read_to_end())) {
        // 此时可以处理 bits
        table[bits]++;
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
// 统计完通过迭代器交给优先队列构造 Huffman 树
// 然后再次读取文件，这次每次的 bits 转换成对应编码，然后交给 write 类写入