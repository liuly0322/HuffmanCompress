struct bits {
    unsigned char* data;
    bits() {}
    bits(int size) { data = new unsigned char[size]; }
    bits(int size, const bits& origin) {
        data = new unsigned char[size];
        for (int i = 0; i < size; i++) {
            data[i] = origin.data[i];
        }
    }
    bool equal(int unit_num, bits& other) {
        for (int i = 0; i < (unit_num + 1) / 2; i++) {
            if (data[i] != other.data[i]) {
                return false;
            }
        }
        return true;
    }
};
