#ifndef TABLE
#define TABLE

#include "../basic_structure/huffman_node.h"

typedef struct table_node {
    h_node* data;
    struct table_node* next;
} t_node;

class table {
   private:
    t_node** base;
    int size;

    int ele_num;  // 有多少个不同的 Huffman 结点

    int n;         // 几叉的 Huffman
    int unit_num;  // 半个字节为一个基本单位
    int hashing(bits& data);

    int iterator_index;
    t_node* iterator;

   public:
    table(int size, int n, int unit_num);
    ~table();

    unsigned int& operator[](bits& data);  // 返回 weight 的引用
    h_node* get_h_node(bits& data);        // 返回查找的 huffman 结点

    int get_ele_num();

    void iterator_clear();
    h_node* iterator_visit();
};

#endif