# rmb: Counting lattice points in a Manhattan ball on a restricted grid
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE.txt)
[![Python](https://img.shields.io/badge/python-%E2%89%A53.9-blue.svg)](https://www.python.org/)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Status: alpha](https://img.shields.io/badge/status-alpha-orange.svg)](#project-status)

The library `rmb` counts the integer lattice points that lie inside a Manhattan (L1) ball while being restricted to a bounded grid. 
<img width="1071" height="916" alt="ie_explainer" src="https://github.com/user-attachments/assets/19601845-051e-4315-b1c5-3361cd0d0da2" />

The counting is implemented in C++ and exposed to Python through [pybind11](https://github.com/pybind/pybind11), so you get native speed with a Python interface.

## Installation
### Requirements
- Python >=3.9.
- A C++17-capable compiler (GCC, Clang, or MSVC).
- CMake >=3.18 (provided by scikit-build-core during the build).

`pybind11` and `scikit-build-core` are declared as build dependencies and are installed automatically by `pip`.

### From GitHub
```bash
pip install git+https://github.com/svilsen/rmb.git
```

### From a local clone
```bash
git clone https://github.com/svilsen/rmb.git
cd rmb
pip install .
```

For development, an editable install is convenient:
```bash
pip install -e .
```

## Usage
```python
from rmb import rmb

r  = 2
km = [3, 4, 5, 6, 7, 3]
kp = [4, 3, 4, 2, 1, 6]
c  = [2, 3, -3, -1, 0, 5]

rmb(d, r, km, kp, c, "bruteforce")
# 71

rmb(d, r, km, kp, c, "exact")
# 71

rmb(d, r, km, kp, c, "dynamic")
# 71

rmb(d, r, km, kp, c, "approximate")
# (1504, 13392.0)
```

### Using the C++ library directly
The C++ core is built as the static library `rmb_core`, with the headers in `include/`. If you use CMake, you can add the repository as a subdirectory and link against the library:

```cmake
add_subdirectory(rmb)
target_link_libraries(my_target PRIVATE rmb_core)
```

By default, Python bindings are also enabled. If you only need the C++ library, you can disable them when configuring CMake:
```bash
cmake -S . -B build -DRMB_BUILD_PYTHON=OFF
cmake --build build
```

Thus, when building the C++ library alone, neither `Python` nor `pybind11` is required.

## Building from source
The build is driven by CMake and `scikit-build-core`. Running `pip install .` is the recommended way to build and install the Python package.

For a manual build, use:
```bash
cmake -S . -B build
cmake --build build
```

This builds both the C++ library and the `Python` extension. Thus, `Python` and `pybind11` must be available to CMake. For a C++-only build, see [Using the C++ library directly](#using-the-c-library-directly).

## Project status
`rmb` is at version 0.1.0 and is marked **alpha** (`Development Status :: 3 - Alpha`). The API may change between releases.

## Contributing
Issues and pull requests are welcome. If you plan a larger change, please open an issue first so it can be discussed.

## License
Released under the [MIT License](LICENSE.txt).

## Author
Søren B. Vilsen ([svilsen@mp.aau.dk](mailto:svilsen@mp.aau.dk))

