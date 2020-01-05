#pragma once

#include <boost/asio.hpp>

namespace khdkhd::http {
class Server {
public:
    Server(boost::asio::io_context &, unsigned int port);
    virtual ~Server();

    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;

public:
    Server &start();
    Server &stop();

private:
    void accept_();
    
private:
    boost::asio::io_context &context_;
    boost::asio::ip::tcp::acceptor acceptor_;
};
} // namespace khdkhd
