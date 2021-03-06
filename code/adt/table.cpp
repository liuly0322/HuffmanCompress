#include "table.h"

void table::iterator_clear() {
    iterator_index = -1;
    iterator = nullptr;
}

h_node* table::iterator_visit() {
    do {
        if (iterator) {
            iterator = iterator->next;
        } else {
            iterator_index++;
            if (iterator_index == size)
                break;
            iterator = base[iterator_index];
        }
    } while (!iterator);
    if (iterator_index == size) {
        iterator_clear();
        return nullptr;
    }
    return iterator->data;
}

int table::hashing(bits& data) {  // 计算哈希值
    // 因为最大四个字节，按照 unsigned int 取模即可
    unsigned int key = 0;
    for (int i = 0; i < unit_num; i++) {
        if (i % 2) {  // 如果是奇数
            key += (data.data[i / 2] & 0xF) << (4 * (unit_num - i - 1));
        } else {  // 如果是偶数
            key += (((data.data[i / 2] & 0xF0)) >> 4)
                   << (4 * (unit_num - i - 1));
        }
    }
    return key % size;
}

table::table(int size, int n, int unit_num)
    : size(size), n(n), unit_num(unit_num) {
    // 每个位置初始化为 nullptr
    base = new t_node*[size]();
    // 初始计数
    ele_num = 0;
};

unsigned int& table::operator[](bits& data) {
    // 先取得地址 base[now]
    int now = hashing(data);
    // 判断是否已存在
    for (auto p = base[now]; p; p = p->next) {
        if (p->data->_data->equal(unit_num, data)) {
            return p->data->weight;
        }
    }
    // 否则需要创建
    auto table_node = new t_node();
    table_node->data = new h_node(n, (unit_num + 1) / 2, data);
    // 插入到对应地址的单链表中
    table_node->next = base[now];
    base[now] = table_node;
    ele_num++;

    return table_node->data->weight;
}

// 只提供查询功能
h_node* table::get_h_node(bits& data) {
    int now = hashing(data);
    for (auto p = base[now]; p; p = p->next) {
        if (p->data->_data->equal(unit_num, data)) {
            return p->data;
        }
    }
    return nullptr;
}

int table::get_ele_num() {
    return ele_num;
}

table::~table() {}
