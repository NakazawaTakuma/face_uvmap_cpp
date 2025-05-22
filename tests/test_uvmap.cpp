#include <iostream>
#include <vector>
#include <cassert>
#include <Eigen/Dense>
#include "face_uvmap_cpp.hpp"

std::vector<std::vector<std::vector<int>>> make_test_image(int w, int h) {
    std::vector<std::vector<std::vector<int>>> img(w, std::vector<std::vector<int>>(h, std::vector<int>(3));
    for (int x = 0; x < w; ++x) for (int y = 0; y < h; ++y) {
        img[x][y] = { x, y, (x + y) % 256 };
    }
    return img;
}

int main() {
    int img_w = 4, img_h = 4, uv = 8;
    auto img = make_test_image(img_w, img_h);
    Eigen::MatrixXd lm(4, 2);
    lm << 0, 0, 1, 0, 1, 1, 0, 1;
    Eigen::MatrixXi tri(2, 3);
    tri << 0, 1, 2, 0, 2, 3;
    Eigen::MatrixXd uv_coords = lm;
    auto uvmap = face_uvmap_cpp::generate_uv_map(img, lm, uv, tri, uv_coords, 0.1, 0.1);
    assert((int)uvmap.size() == uv);
    std::cout << "Tests passed" << std::endl;
    return 0;
}