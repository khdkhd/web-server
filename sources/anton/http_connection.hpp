#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "http_request_parser.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "http_errors.hpp"
#include "http_request_handler.hpp"

using namespace boost::asio;
using error_code = boost::system::error_code;

class http_connection {
public:
    http_connection(const http_connection&) = delete;
    http_connection& operator=(const http_connection&) = delete;
    explicit http_connection(ip::tcp::socket&& socket);
public:
    void start(http_request_handler const &handler);
    void stop();
public:
    void on_done(std::function<void()>&& handler);
private:
    void read(http_request_handler const &handler);
    void write();

private:
    ip::tcp::socket socket;
    std::array<char, 8192> request_buffer;
    http_request_parser parser;
    http_request request;
    http_response response;
    std::function<void()> on_done_handler;
};