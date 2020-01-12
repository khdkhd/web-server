#include "http_server.hpp"

#include <iostream>
#include <memory>

using error_code = boost::system::error_code;

http_server::http_server(int port) : acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
    acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
    acceptor.listen();
    accept();
}

void http_server::start() {
    io_context.run();
}

void http_server::accept() {
    static const auto acceptor_behavior = [this](error_code ec, ip::tcp::socket socket) {
        if (!acceptor.is_open()) {
            return;
        }
        if (!ec) {
            auto connection = std::make_shared<http_connection>(std::move(socket));
            connection->start();
        }
        accept();
    };

    acceptor.async_accept(acceptor_behavior);
}