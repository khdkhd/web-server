#pragma once

#include <boost/asio.hpp>

#include <memory>

namespace khdkhd::http {
class Connection
    : std::enable_shared_from_this<Connection> {
public:
    using pointer = std::shared_ptr<Connection>;

public:
    static pointer create(boost::asio::io_context &);

    boost::asio::ip::tcp::socket &socket();
    void run();

private:
    Connection(boost::asio::io_context &);

private:
    boost::asio::ip::tcp::tcp::socket socket_;
};
} // namespace khdkhd::http
