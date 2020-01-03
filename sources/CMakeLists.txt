project("web-server")

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
    PUBLIC CONAN_PKG::boost
)