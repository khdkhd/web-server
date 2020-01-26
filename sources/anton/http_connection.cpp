#include "http_connection.hpp"
#include "http_middleware.hpp"

using error_code = boost::system::error_code;

http_connection::
http_connection(ip::tcp::socket &&socket): socket(std::move(socket)) {

}

void http_connection::start(const http_request_handler &handler) {
    read(handler);
}

void http_connection::read(http_request_handler const &handler) {
    const auto read_behavior = [&](error_code ec, std::size_t bytes_transferred) {
        if (!ec) {
            try {
                parser.parse(request, buffer.data(), buffer.data() + bytes_transferred);
                handler.handle_request(request, response);
                write();
            } catch (const std::runtime_error &e) {
                response.status = 400;
                response.body = e.what();
                return write();
            }
            read(handler);
        }
    };

    socket.async_read_some(boost::asio::buffer(buffer), read_behavior);
}

void http_connection::write() {
//    socket.close();
}
