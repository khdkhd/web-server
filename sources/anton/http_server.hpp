#pragma once

#include "http_connection_manager.hpp"
#include "http_request_handler.hpp"

#include <boost/asio.hpp>
#include <set>

using namespace boost::asio;
using ip::tcp;

class http_server {
public:
    explicit http_server(http_request_handler  handler, int port = 80);
public:
    void start();

private:
    void accept();
private:
    io_context io_context;
    tcp::acceptor acceptor;
    http_request_handler handler;
    http_connection_manager connection_manager;
};
