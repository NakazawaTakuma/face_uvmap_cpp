#pragma once

#include <vector>
#include <Eigen/Dense>

namespace face_uvmap_cpp {

    using MatrixXdRM = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
    using MatrixXiRM = Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

    /**
     * Generate a UV map from an RGB image using facial landmarks and triangle topology.
     *
     * @param img        Source image as a 3D vector [width][height][3], where each inner vector is [R,G,B].
     * @param landmarks  2D face landmarks, shape (N_points x 2) in normalized coordinates [0.0, 1.0].
     * @param uvmap_size Output UV map width and height (square).
     * @param triangles  Triangle vertex indices matrix (N_triangles x 3) referencing landmarks rows.
     * @param uv_coords  UV coordinate matrix (N_points x 2) in normalized [0.0, 1.0] for each landmark.
     * @param min_tri_w  Minimum triangle width ratio (relative to uvmap_size) for masking small tris.
     * @param min_tri_h  Minimum triangle height ratio (relative to uvmap_size) for masking small tris.
     * @return           UV map as a 3D vector [uvmap_size][uvmap_size][3] of doubles in [0.0, 255.0].
     */
    std::vector<std::vector<std::vector<double>>>
        generate_uv_map(
            const std::vector<std::vector<std::vector<int>>>& img,
            const MatrixXdRM& landmarks,
            int uvmap_size,
            const MatrixXiRM& triangles,
            const MatrixXdRM& uv_coords,
            double min_tri_w,
            double min_tri_h
        );

} // namespace face_uvmap_cpp