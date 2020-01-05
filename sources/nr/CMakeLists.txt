project("nr-web-server")

add_executable(${PROJECT_NAME} 
    http/connection.cpp
    http/connection.hpp
    http/error.cpp
    http/error.hpp
    http/request.hpp
    http/server.cpp
    http/server.hpp
    http/socket-buffered-reader.cpp
    http/socket-buffered-reader.hpp
    main.cpp
)
target_compile_features(${PROJECT_NAME} 
    PUBLIC cxx_std_17
)
target_compile_options(${PROJECT_NAME}
    PUBLIC -fvisibility=hidden 
)
target_link_libraries(${PROJECT_NAME}
    PUBLIC CONAN_PKG::boost
)
