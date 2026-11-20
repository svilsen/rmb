#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <rmb/restricted_manhattan_ball.hpp>

PYBIND11_MODULE(_core, m) {
    m.def("bruteforce", &rmb::bruteforce);
    m.def("delannoy", &rmb::delannoy);
    m.def("exact", &rmb::exact);
    m.def("exact_grp", &rmb::exact_grp);
    m.def("dp", &rmb::dp);
    m.def("dp_grp", &rmb::dp_grp);
    m.def("approximate", &rmb::approximate);
    m.def("approximate_grp", &rmb::approximate_grp);
}

