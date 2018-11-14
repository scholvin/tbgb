cmake_minimum_required(VERSION 3.0)

project(tbgb)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)
pkg_check_modules(LIBOLA libola)

include_directories(${GTKMM_INCLUDE_DIRS} ${LIBOLA_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS} ${LIBOLA_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
add_executable(tbgb animations.cpp framebuf.cpp tbgb.cpp viz.cpp dmx.cpp global.cpp)
target_link_libraries(tbgb ${GTKMM_LIBRARIES} ${LIBOLA_LIBRARIES})
