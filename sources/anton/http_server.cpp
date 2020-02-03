#include "http_server.hpp"

#include <utility>

using error_code = boost::system::error_code;

http_server::http_server(http_request_handler  handler, int port) :
acceptor(io_context, tcp::endpoint(tcp::v4(), port)),
handler(std::move(handler)) {
    acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
    acceptor.listen();
    accept();
}

void http_server::start() {
    io_context.run();
}

void http_server::accept() {
    const auto acceptor_behavior = [this](error_code ec, ip::tcp::socket socket) {
        if (!acceptor.is_open()) {
            return;
        }
        if (!ec) {
            connection_manager.start(std::move(socket), handler);
        }
        accept();
    };

    acceptor.async_accept(acceptor_behavior);
}
