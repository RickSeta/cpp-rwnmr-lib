#ifndef CUSTOMMAT_H
#define CUSTOMMAT_H

#include <vector>

class CustomMat {
public:
    CustomMat() : rows(0), cols(0) {}
    CustomMat(int rows, int cols) : rows(rows), cols(cols) {
        data.resize(rows * cols, 0.0);
    }
    CustomMat(int rows, int cols, const std::vector<double>& data) : rows(rows), cols(cols), data(data) {}

    double& at(int row, int col) {
        return data[row * cols + col];
    }

    const double& at(int row, int col) const {
        return data[row * cols + col];
    }
    static std::vector<double> ones(int rows, int cols) {
        std::vector<double> ones(rows * cols, 1.0);
        return ones;
    }
    static std::vector<double> eye(int rows, int cols) {
        std::vector<double> eye(rows * cols, 0.0);
        for (int i = 0; i < std::min(rows, cols); i++) {
            eye[i * cols + i] = 1.0;
        }
        return eye;
    }

    int getRows() const { return rows; }
    int getCols() const { return cols; }
    const std::vector<double>& getData() const {
        return data;
    }

private:
    int rows, cols;
    std::vector<double> data;
};

#endif 