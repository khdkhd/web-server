#include "http/server.hpp"

#include <iostream>

int
main(int argc, char **argv) {
    boost::asio::io_context context;

    try {
        khdkhd::http::Server server(context, 8080);
        server.start();
        context.run();
    } catch (std::exception &err) {
        std::cerr << err.what() << std::endl;
    }

    return 0;
}