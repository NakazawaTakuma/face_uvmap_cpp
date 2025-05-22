# face_uvmap_cpp (C++17)

A C++17 library with Python bindings (via pybind11) to generate UV maps from facial landmarks using Eigen3.

📄 日本語版はこちら → [README.ja.md](README.ja.md)

## Features

- Generates a UV texture map from a source image and 2D facial landmarks.
- Filters out triangles smaller than specified width/height ratios.
- Written in C++17, with optional Python API using pybind11.
- Uses Eigen3 for efficient matrix computation.

## Requirements

- C++17 compatible compiler (e.g., GCC 9+, Clang 10+)
- CMake >= 3.14
- pybind11
- Eigen3

## Installation

### Add pybind11 as a Submodule

```bash
git submodule add https://github.com/pybind/pybind11.git extern/pybind11
git submodule update --init --recursive
```

### Build

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Input and Output

### Input

| Parameter   | Type                                         | Description                                        |
| ----------- | -------------------------------------------- | -------------------------------------------------- |
| `img`       | `std::vector<std::vector<std::vector<int>>>` | RGB image data as a 3D vector (height x width x 3) |
| `landmarks` | `Eigen::MatrixXd` (N x 2)                    | 2D facial landmarks                                |
| `triangles` | `Eigen::MatrixXi` (M x 3)                    | Triangle indices defining the mesh                 |
| `uv_coords` | `Eigen::MatrixXd` (N x 2)                    | UV coordinates corresponding to landmarks          |
| `uv_size`   | `int`                                        | Size of the output UV map (e.g., 256)              |
| `min_w`     | `double`                                     | Minimum triangle width ratio to include            |
| `min_h`     | `double`                                     | Minimum triangle height ratio to include           |

### Output

- `uvmap`: A 2D image (e.g., `std::vector<std::vector<std::vector<int>>>`) representing the UV texture map, where each pixel contains RGB values mapped from the input image based on the UV coordinates.

## Usage

### In C++

```cpp
#include "face_uvmap_cpp.hpp"

int main() {
    std::vector<std::vector<std::vector<int>>> img = /* RGB image data */;
    Eigen::MatrixXd landmarks;    // (N x 2)
    Eigen::MatrixXi triangles;    // (M x 3)
    Eigen::MatrixXd uv_coords;    // (N x 2)

    int uv_size = 256;
    double min_w = 0.01;
    double min_h = 0.01;

    auto uvmap = face_uvmap_cpp::generate_uv_map(
        img, landmarks, uv_size, triangles, uv_coords, min_w, min_h
    );
}
```

### In Python (after building)

```python
import face_uvmap_cpp

uvmap = face_uvmap_cpp.generate_uv_map(
    img, landmarks, 256, triangles, uv_coords, 0.01, 0.01
)
```

## Testing

```bash
cd build
./test_uvmap
```

## License

MIT License. See `LICENSE` for details.
