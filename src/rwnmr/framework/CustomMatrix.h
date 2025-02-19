#ifndef CUSTOMMAT_H
#define CUSTOMMAT_H

#include <vector>

class CustomMat {
public:
    CustomMat() : rows(0), cols(0) {}
    CustomMat(int rows, int cols) : rows(rows), cols(cols) {
        data.resize(rows * cols, 0.0);
    }
    CustomMat(int rows, int cols, const std::vector<uint8_t>& data) : rows(rows), cols(cols), data(data) {}

    uint8_t& at(int row, int col) {
        return data[row * cols + col];
    }

    const uint8_t& at(int row, int col) const {
        return data[row * cols + col];
    }
    static std::vector<uint8_t> ones(int rows, int cols) {
        std::vector<uint8_t> ones(rows * cols, 1.0);
        return ones;
    }
    static std::vector<uint8_t> eye(int rows, int cols) {
        std::vector<uint8_t> eye(rows * cols, 0.0);
        for (int i = 0; i < std::min(rows, cols); i++) {
            eye[i * cols + i] = 1.0;
        }
        return eye;
    }

    int getRows() const { return rows; }
    int getCols() const { return cols; }
    const std::vector<uint8_t>& getData() const {
        return data;
    }

private:
    int rows, cols;
    std::vector<uint8_t> data;
};

#endif 