#pragma once

#include <boost/asio.hpp>

namespace khdkhd {
class SocketBufferedReader {
public:
    SocketBufferedReader(boost::asio::ip::tcp::socket &);

    SocketBufferedReader(const SocketBufferedReader &) = delete;
    SocketBufferedReader &operator=(const SocketBufferedReader &) = delete;

public:
    std::string readLine(const std::string_view &delimiter);

private:
    boost::asio::ip::tcp::socket &socket_;
    std::string buffer_;
};
} // namespace khdkhd
