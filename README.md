# rmb: Counting lattice points in a Manhattan ball on a restricted grid

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE.txt)
[![Python](https://img.shields.io/badge/python-%E2%89%A53.9-blue.svg)](https://www.python.org/)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Status: alpha](https://img.shields.io/badge/status-alpha-orange.svg)](#project-status)

`rmb` counts the integer lattice points that lie inside a Manhattan (L1) ball while being restricted to a bounded grid. The counting is implemented in C++ and exposed to Python through [pybind11](https://github.com/pybind/pybind11), so you get native speed with a Python interface.

## Installation
### Requirements

- Python 3.9 or newer
- A C++17-capable compiler (GCC, Clang or MSVC)
- CMake 3.15 or newer (scikit-build-core can provide this for you during the build)

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
from rmb import restricted_manhattan_ball

d = 2
r = 2
km = [3, 4]
kp = [4, 3]
c = [2, 3]

restricted_manhattan_ball(d, r, km, kp, c, "bruteforce")
# {"count": 9, "error": 0}

restricted_manhattan_ball(d, r, km, kp, c, "exact")
# {"count": 9, "error": 0}

restricted_manhattan_ball(d, r, km, kp, c, "approximate")
# {"count": 18, "error": 12}
```

### Using the C++ library directly

The C++ core is built as the static library `rmb_core`, with the headers in `include/`. If you use CMake, you can add the repository as a subdirectory and link against the library:

```cmake
add_subdirectory(rmb)
target_link_libraries(my_target PRIVATE rmb_core)
```

Note that the current `CMakeLists.txt` calls `find_package(pybind11 CONFIG REQUIRED)` unconditionally, so pybind11 must be discoverable at configure time even when you only need the C++ library.

## Building from source

The build is driven by CMake and scikit-build-core. Running `pip install .` is the recommended route, but you can also build the pieces manually:

```bash
cmake -S . -B build
cmake --build build
```

## Project status

`rmb` is at version 0.1.0 and is marked **alpha** (`Development Status :: 3 - Alpha`). The API may change between releases.

## Contributing

Issues and pull requests are welcome. If you plan a larger change, please open an issue first so it can be discussed.

## License

Released under the [MIT License](LICENSE.txt).

## Author

Søren B. Vilsen ([svilsen@mp.aau.dk](mailto:svilsen@mp.aau.dk))

