project("anton-web-server")

SET(SOURCES
        main.cpp
        http_connection.hpp
        http_connection.cpp
        http_server.cpp
        http_server.hpp
        http_request.hpp
        http_headers.cpp
        http_headers.hpp
        http_errors.cpp
        http_errors.hpp
        http_request_parser.cpp
        http_request_parser.hpp
        http_version.hpp
        http_request_handler.cpp
        http_request_handler.hpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    PUBLIC CONAN_PKG::boost
)

target_compile_features(${PROJECT_NAME} 
    PUBLIC cxx_std_17
)