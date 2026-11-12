#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <rmb/restricted_manhattan_ball.hpp>

PYBIND11_MODULE(_core, m) {
    m.def("bruteforce", &rmb::bruteforce);
    m.def("exact", &rmb::exact);
    m.def("approximate", &rmb::approximate);
}

