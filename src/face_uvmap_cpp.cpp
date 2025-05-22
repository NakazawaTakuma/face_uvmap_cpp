#include "face_uvmap_cpp.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <Eigen/Dense>
#include <algorithm>
#include <cmath>

namespace py = pybind11;
using namespace face_uvmap_cpp;

template<typename T>
bool isEqual(const std::vector<T>& a, const std::vector<T>& b) {
    return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin());
}

std::vector<std::vector<std::vector<double>>>
generate_uv_map(
    const std::vector<std::vector<std::vector<int>>>& img,
    const MatrixXdRM& landmarks,
    int uvmap_size,
    const MatrixXiRM& triangles,
    const MatrixXdRM& uv_coords,
    double min_tri_w,
    double min_tri_h
) {
    int img_w = img.size();
    int img_h = img.empty() ? 0 : img[0].size();

    std::vector<std::vector<std::vector<double>>> uvmap(
        uvmap_size,
        std::vector<std::vector<double>>(uvmap_size, std::vector<double>(3, 0.0))
    );

    size_t n_tri = triangles.rows();
    for (size_t ti = 0; ti < n_tri; ++ti) {
        Eigen::Matrix<double, 3, 3> dst, src;
        for (int vi = 0; vi < 3; ++vi) {
            int idx = triangles(ti, vi);
            dst(0, vi) = landmarks(idx, 0) * img_w;
            dst(1, vi) = landmarks(idx, 1) * img_h;
            dst(2, vi) = 1.0;

            src(0, vi) = uv_coords(idx, 0) * uvmap_size;
            src(1, vi) = uv_coords(idx, 1) * uvmap_size;
            src(2, vi) = 1.0;
        }

        double u_min = src.row(0).minCoeff();
        double u_max = src.row(0).maxCoeff();
        double v_min = src.row(1).minCoeff();
        double v_max = src.row(1).maxCoeff();

        int u0 = std::floor(u_min), u1 = std::ceil(u_max);
        int v0 = std::floor(v_min), v1 = std::ceil(v_max);
        if ((u1 - u0) < min_tri_w * uvmap_size || (v1 - v0) < min_tri_h * uvmap_size)
            continue;

        Eigen::Matrix3d A = dst * src.inverse();
        for (int uu = u0; uu <= u1; ++uu) {
            for (int vv = v0; vv <= v1; ++vv) {
                double x_img = A(0, 0) * uu + A(0, 1) * vv + A(0, 2);
                double y_img = A(1, 0) * uu + A(1, 1) * vv + A(1, 2);
                Eigen::Vector2d p(uu, vv), p0(src(0, 0), src(1, 0));
                Eigen::Vector2d p1(src(0, 1), src(1, 1));
                Eigen::Vector2d p2(src(0, 2), src(1, 2));
                auto edge = [&](const Eigen::Vector2d& a, const Eigen::Vector2d& b) {
                    return (p.x() - a.x()) * (b.y() - a.y()) - (p.y() - a.y()) * (b.x() - a.x());
                    };
                if (!(edge(p0, p1) * edge(p, p1) >= 0 && edge(p1, p2) * edge(p, p2) >= 0 && edge(p2, p0) * edge(p, p0) >= 0))
                    continue;

                int xi = std::clamp(int(std::round(x_img)), 0, img_w - 1);
                int yi = std::clamp(int(std::round(y_img)), 0, img_h - 1);
                uvmap[vv][uu] = { double(img[xi][yi][0]), double(img[xi][yi][1]), double(img[xi][yi][2]) };
            }
        }
    }
    return uvmap;
}

PYBIND11_MODULE(face_uvmap_cpp, m) {
    m.doc() = "C++17 Face UV map generator using pybind11 and Eigen3";
    m.def("generate_uv_map", &generate_uv_map,
        py::arg("img"), py::arg("landmarks"), py::arg("uvmap_size"),
        py::arg("triangles"), py::arg("uv_coords"),
        py::arg("min_tri_w"), py::arg("min_tri_h"));
}