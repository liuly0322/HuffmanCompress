#include <iostream>
#include "huffman.h"
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
    // 还剩一个基本单位没统计，我的建议是摆烂不压缩了

    // 希望已经 Hash 表统计完了？ 大概吧xd

    int node_num = table.get_ele_num();  // 有多少个 Huffman 结点

    // 先假设是二叉树，不补了，所有结点直接进入优先队列
    p_queue p_queue(node_num);

    table.iterator_clear();
    h_node* temp;
    while ((temp = table.iterator_visit())) {
        // 迭代器里遍历读出了所有的 Huffman_Node
        p_queue.enqueue(temp);
    }

    // 一次弹出 k 个，如果空了，就说明到根了
    h_node** nodes = new h_node*[2]();
    h_node* father;
    while (true) {
        for (int i = 0; i < 2; i++) {
            nodes[i] = p_queue.dequeue();
        }
        father = new h_node(2, nodes);
        if (p_queue.empty()) {
            break;
        }
        p_queue.enqueue(father);
    }
    // 此时 Huffman 树建立完毕，father 是根结点
    // 如果是 1 字节的默认 Huffman 压缩，应该支持打印树
    show_huffman(father, 2);
    // 最后摆烂没压缩的那个字符
    std::cout << (int)bits.data[0] << ' ' << (int)bits.data[1];
    return 0;
}

// 下一步：
// 统计完通过迭代器交给优先队列构造 Huffman 树
// 然后再次读取文件，这次每次的 bits 转换成对应编码，然后交给 write 类写入
