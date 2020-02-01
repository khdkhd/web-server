#include "http_connection_manager.hpp"

#include <memory>
#include <utility>

#include <iostream>

void http_connection_manager::start(tcp::socket &&socket, const http_request_handler& handler) {
    auto connection = std::make_shared<http_connection>(std::move(socket));
    connections.insert(connection);
    connection->on_Shutdown([&]() {
        connections.erase(connection);
    });
    connection->start(handler);
}
