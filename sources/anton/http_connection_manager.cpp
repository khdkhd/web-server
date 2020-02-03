#include "http_connection_manager.hpp"

void http_connection_manager::start(tcp::socket &&socket, const http_request_handler& handler) {
    auto connection = std::make_shared<http_connection>(std::move(socket));
    connections.insert(connection);
    connection->on_done([&connections = connections, connection]() {
        connections.erase(connection);
    });
    connection->start(handler);
}
