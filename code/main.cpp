#include <cstdlib>
#include <iostream>
#include "adt/p_queue.h"
#include "adt/table.h"
#include "huffman_function/huffman.h"
#include "io/read.h"
#include "io/write.h"

int main(int argc, char** argv) {
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

    // 哈希表的大小根据 unit_num 确定（范围 1 - 8）
    // 初始占用空间估计：一个指针变量占用 8 字节空间。一千万约需要 100 mb
    // 内存空间
    int hash_size[9] = {0,      16,     256,    4096,   65536,
                        131072, 262144, 524288, 1048576};

    table table(hash_size[unit_num], branch,
                unit_num);          // 创建二叉 1字节为基本单位的表
    bits bits((unit_num + 1) / 2);  // 基本储存大小为 1 字节
    read read(argv[in_index], &bits, unit_num);  // 对应 2 个半字节
    int zero_num = 0;                            // 补 0 数
    while (!(zero_num = read.read_to_end())) {
        table[bits]++;
    }
    // 注意还剩一个基本单位没统计

    int node_num = table.get_ele_num();  // 有多少个 Huffman 结点

    // 建立小根堆，先把空结点放入，避免无谓的调整
    // 队列元素总数 k(branch - 1) + 1, 当前有 node_num 个
    // 对 branch - 1 取模应该是 1, 假设现在是 m
    int empty_node_num = (1 - (node_num % (branch - 1))) % (branch - 1);
    p_queue p_queue(node_num + empty_node_num);
    for (int i = 0; i < empty_node_num; i++) {
        h_node* empty_node = new h_node();
        p_queue.enqueue(empty_node);
    }

    table.iterator_clear();  // 迭代器遍历表中所有结点
    h_node* temp;
    while ((temp = table.iterator_visit())) {
        p_queue.enqueue(temp);
    }

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
    // 此时 Huffman 树建立完毕，father 是根结点

    if (display_mode)
        show_huffman(father, branch, unit_num);

    // 最长编码需要的数字数
    int max_code_size = get_huffman_depth(father, branch);

    // 最后摆烂没压缩的那个字符
    std::cout << (int)bits.data[0] << ' ' << (int)bits.data[1];
    return 0;
}

// 下一步：
// 再次读取文件，这次每次的 bits 转换成对应编码，然后交给 write 类写入
// 考虑解压：解压时可以一个字节一个字节读取，所以文件头和文件末尾都可以存信息
// 文件头可以保存 huffman 编码（听上去很合理），比如说换行结束
// 文件末尾一个字节可以直接用来表示前面补了几个零
// 需要的接口：read 类的重置指针到文件头，每次从 table 中查出元素后追溯出编码，
// 追溯出编码 -- 转换成 bits 类 -- 给 write 类处理
// 交给 write 类正常写入，攒 buffer，攒够了就写
// write 类还应该有个 end，把最后不足 8 位的 buffer 写入并返回补了多少个零