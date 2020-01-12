#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "http_request_parser.hpp"
#include "http_request.hpp"

using namespace boost::asio;

class http_connection {
public:
    http_connection(const http_connection&) = delete;
    http_connection& operator=(const http_connection&) = delete;
    explicit http_connection(ip::tcp::socket&& socket);

public:
    void start();

private:
    void read();

private:
    http_request_parser parser;
    http_request request;
    ip::tcp::socket socket;

    std::array<char, 4096> buffer;
};