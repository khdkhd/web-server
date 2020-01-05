#include "socket-buffered-reader.hpp"

namespace khdkhd {

SocketBufferedReader::SocketBufferedReader(boost::asio::ip::tcp::socket &socket)
    : socket_{socket} {
}

std::string
SocketBufferedReader::readLine(const std::string_view &delimiter) {
    using boost::asio::read_until;
    using boost::asio::dynamic_buffer;
    using std::search;

    auto first = buffer_.cbegin();
    auto last = buffer_.cend();
    auto it = search(first, last, delimiter.begin(), delimiter.end());

    if (it != last) {
        std::string line(first, it);
        buffer_.erase(first, it + 2);
        return line;
    }

    read_until(socket_, dynamic_buffer(buffer_), delimiter);

    return readLine(delimiter);
}
} // namespace khdkhd
