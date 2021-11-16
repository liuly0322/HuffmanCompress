#ifndef P_QUEUE
#define P_QUEUE

#include <algorithm>  // for std::swap
#include "huffman_node.h"

// 一个优先队列
// 目的是每次让权最小的点位于顶
class p_queue {
   private:
    h_node** base;
    int size;  // 当前队列中元素个数

   public:
    p_queue(int size);  // 容器最大容纳元素个数
    ~p_queue();

    void enqueue(h_node* node);
    h_node* dequeue();

	bool empty();
};

#endif
