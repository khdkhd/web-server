project("web-server")

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
    PUBLIC CONAN_PKG::boost
)
target_compile_features(${PROJECT_NAME} 
    PUBLIC cxx_std_17
)