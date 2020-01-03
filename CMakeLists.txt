cmake_minimum_required(VERSION 3.0)

project("web-server-project")

include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")

conan_basic_setup(TARGETS)

add_subdirectory(sources)

