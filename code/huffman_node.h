typedef struct huffman_node {
    unsigned int weight;
    huffman_node* parent;
    huffman_node** children;
    huffman_node(int n) {
        parent = nullptr;
        children = new huffman_node*[n]();
        weight = 0;
    }
} h_node;
