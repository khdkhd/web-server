#include "http_connection.hpp"
#include "http_middleware.hpp"

http_connection::
http_connection(ip::tcp::socket &&socket) : socket(std::move(socket)) {

}

void http_connection::start(const http_request_handler &handler) {
    read(handler);
}

void http_connection::read(http_request_handler const &handler) {
    const auto read_behavior = [&](error_code ec, std::size_t bytes_transferred) {
        if (!ec) {
            try {
                parser.parse(request, request_buffer.data(), request_buffer.data() + bytes_transferred);
                if (request.is_valid) {
                    handler.handle_request(request, response);
                    return write();
                }
                read(handler);
            } catch (const std::runtime_error &e) {
                response.status = 400;
                response.message = "Bad Request";
                response.version = request.version;
                return write();
            }
        }
        if (ec != error::operation_aborted) {
            stop();
        }
    };

    socket.async_read_some(buffer(request_buffer), read_behavior);
}

void http_connection::write() {
    static const std::string colon = ": ";
    static const std::string crlf = "\r\n";
    static const std::string space = " ";

    std::vector<const_buffer> buffers;

    buffers.emplace_back(buffer(response.version.text));
    buffers.emplace_back(buffer(space));
    buffers.emplace_back(buffer(std::to_string(response.status)));
    buffers.emplace_back(buffer(space));
    buffers.emplace_back(buffer(response.message));
    buffers.emplace_back(buffer(crlf));

    auto content_length = std::to_string(response.body.size());
    response.headers.emplace_back(http_header{"content-length", content_length});

    for (auto const &header: response.headers) {
        buffers.emplace_back(buffer(header.name));
        buffers.emplace_back(buffer(colon));
        buffers.emplace_back(buffer(header.value));
        buffers.emplace_back(buffer(crlf));
    }

    buffers.emplace_back(buffer(crlf));

    if (!response.body.empty()) {
        buffers.emplace_back(buffer(response.body));
        buffers.emplace_back(buffer(crlf));
        buffers.emplace_back(buffer(crlf));
    }

    socket.async_send(buffers, [&](error_code ec, std::size_t) {
        if (!ec) {
            socket.shutdown(ip::tcp::socket::shutdown_both);
        }
        if (ec != error::operation_aborted) {
            stop();
        }
    });
}

void http_connection::on_done(std::function<void()>&& handler) {
    on_done_handler = std::move(handler);
}

void http_connection::stop() {
    socket.close();
    if (on_done_handler) {
        on_done_handler();
    }
}
