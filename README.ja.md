# face_uvmap_cpp (C++17)

顔のランドマークから UV マップを生成する C++17 ライブラリです。Eigen3 による行列演算と、pybind11 による Python バインディングに対応しています。

## 特徴

- 画像と顔ランドマークから UV テクスチャマップを生成
- 最小幅・高さを下回る三角形を除外
- 純粋な C++17 実装
- Python API（pybind11 経由）にも対応
- Eigen3 による効率的な行列演算

## 必要環境

- C++17 対応コンパイラ（例: GCC 9+, Clang 10+）
- CMake >= 3.14
- pybind11
- Eigen3

## インストール手順

### pybind11 をサブモジュールとして追加

```bash
git submodule add https://github.com/pybind/pybind11.git extern/pybind11
git submodule update --init --recursive
```

### ビルド方法

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## 入力と出力

### 入力

| パラメータ  | 型                                           | 説明                                              |
| ----------- | -------------------------------------------- | ------------------------------------------------- |
| `img`       | `std::vector<std::vector<std::vector<int>>>` | RGB 画像データ（高さ x 幅 x 3 の 3 次元ベクトル） |
| `landmarks` | `Eigen::MatrixXd` (N x 2)                    | 2D 顔ランドマーク座標                             |
| `triangles` | `Eigen::MatrixXi` (M x 3)                    | メッシュを定義する三角形のインデックス            |
| `uv_coords` | `Eigen::MatrixXd` (N x 2)                    | ランドマークに対応する UV 座標                    |
| `uv_size`   | `int`                                        | 出力 UV マップのサイズ（例: 256）                 |
| `min_w`     | `double`                                     | 含める最小三角形幅の比率                          |
| `min_h`     | `double`                                     | 含める最小三角形高さの比率                        |

### 出力

- `uvmap`: 入力画像と UV 座標に基づいてマッピングされた RGB 値を含む 2D 画像（例: `std::vector<std::vector<std::vector<int>>>`）

## 使用例

### C++で使用する場合

```cpp
#include "face_uvmap_cpp.hpp"

int main() {
    std::vector<std::vector<std::vector<int>>> img = /* RGB画像データ */;
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

### Python から使う場合（ビルド後）

```python
import face_uvmap_cpp

uvmap = face_uvmap_cpp.generate_uv_map(
    img, landmarks, 256, triangles, uv_coords, 0.01, 0.01
)
```

## テスト実行方法

```bash
cd build
./test_uvmap
```

## ライセンス

MIT ライセンス。詳細は `LICENSE` ファイルをご覧ください。
