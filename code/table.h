#include "bits.h"
#include "huffman_node.h"

typedef struct table_node {
    h_node* data;
    struct table_node* next;
} t_node;

class table {
   private:
    t_node** base;
    int size;

   public:
    table(int size);
    ~table();
    unsigned int& operator[](bits* data);
};

table::table(int size) : size(size) {
    // 每个位置初始化为 nullptr
    base = new t_node*[size]();
};

unsigned int& table::operator[](bits* data) {
    // 根据传入的 data 找寻表中有没有目标
    // 如果没有，创建，再返回
}

table::~table() {}
