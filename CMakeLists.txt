##
#
cmake_minimum_required(VERSION 3.15)
project(rmb LANGUAGES CXX)

#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#
find_package(pybind11 CONFIG REQUIRED)

##
#
add_library(rmb_core STATIC
    src/cpp/restricted_manhattan_ball.cpp
)

#
target_include_directories(rmb_core PUBLIC include)

##
#
pybind11_add_module(_core src/cpp/bindings.cpp)
target_link_libraries(_core PRIVATE rmb_core)

#
install(TARGETS _core DESTINATION rmb)
