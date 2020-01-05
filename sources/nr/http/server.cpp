#include "server.hpp"
#include "connection.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <regex>
#include <string_view>
#include <tuple>

namespace khdkhd::http {

using boost::asio::ip::tcp;

Server::Server(boost::asio::io_context &context, unsigned int port)
    : context_{context}
    , acceptor_(context_, tcp::endpoint(tcp::v4(), port)) {
}

Server::~Server() {
}

Server &
Server::start() {
    accept_();
    return *this;
}

Server &
Server::stop() {
    return *this;
}

void
Server::accept_() {
    Connection::pointer connection = Connection::create(context_);
    acceptor_.async_accept(
        connection->socket(),
        [this, connection](const boost::system::error_code& error) {
            if (!error) {
                connection->run();
            }
            accept_();
        }
    );
}

} // namespace khdkhd
