#include "http_connection.hpp"

#include <iostream>

using error_code = boost::system::error_code;

http_connection::
http_connection(ip::tcp::socket &&socket) : socket(std::move(socket)) {}

void http_connection::start() {
    try {
        read();
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }
}

void http_connection::read() {
    static const auto read_behavior = [this](error_code ec, std::size_t bytes_transferred) {
        if (!ec) {
            try {
                parser.parse(request, buffer.data(), buffer.data() + bytes_transferred);
            } catch (const std::runtime_error &e) {

            }

            if (request.is_valid) {
                return;
            }

            read();
        }
    };

    socket.async_read_some(boost::asio::buffer(buffer), read_behavior);
}

