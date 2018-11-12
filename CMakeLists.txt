cmake_minimum_required(VERSION 3.0)

project(tbgb)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
add_executable(tbgb animations.cpp framebuf.cpp tbgb.cpp viz.cpp)
target_link_libraries(tbgb ${GTKMM_LIBRARIES})
