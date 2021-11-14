#include "p_queue.h"

p_queue::p_queue(int size) {
    base = new h_node*[size]();
    this->size = 0;
}

p_queue::~p_queue() {}

void p_queue::enqueue(h_node* node) {
    base[size] = node;
    // 新插入的下标是 size
    int son = size;
    while (son) {
        int father = (son - 1) / 2;
        // father 应该小于等于 son ，否则需要交换
        if (base[father]->weight <= base[son]->weight) {
            break;
        }
        std::swap(base[father], base[son]);
        son = father;
    }
    size++;
}

h_node* p_queue::dequeue() {
    // 弹出堆顶。考虑把最后一个元素移到第一位，再不断调整
    h_node* res = base[0];
    base[0] = base[size - 1];
    size--;
    // 第一个元素不断下沉
    int father = 0;
    while (2 * father + 1 < size) {
        if (2 * father + 1 == size - 1) {
            if (base[father]->weight > base[size - 1]->weight) {
                std::swap(base[father], base[size - 1]);
            }
            break;
        }
        // 此时左右孩子都存在
        if (base[father]->weight <= base[2 * father + 1]->weight &&
            base[father]->weight <= base[2 * father + 2]->weight)
            break;
        if (base[2 * father + 1]->weight < base[2 * father + 2]->weight) {
            std::swap(base[father], base[2 * father + 1]);
            father = 2 * father + 1;
        } else {
            std::swap(base[father], base[2 * father + 2]);
            father = 2 * father + 2;
        }
    }
    return res;
}