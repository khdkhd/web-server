#pragma once

#include "http_connection.hpp"
#include "http_request_handler.hpp"

#include <boost/asio.hpp>
#include <memory>
#include <set>

using namespace boost::asio::ip;

class http_connection_manager {
public:
    void start(tcp::socket &&socket, const http_request_handler& handler);
private:
    std::set<std::shared_ptr<http_connection>> connections;
};



