#pragma once

#include "http_connection.hpp"

#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

class http_server {
public:
    explicit http_server(int port = 80);
public:
    void start();

private:
    void accept();
private:
    io_context io_context;
    tcp::acceptor acceptor;
};
