#include "connection.hpp"
#include "error.hpp"
#include "request.hpp"
#include "socket-buffered-reader.hpp"

#include <iostream>
#include <regex>

namespace khdkhd::http {

using boost::asio::ip::tcp;
using boost::asio::io_context;

namespace {

RequestLine
parseRequestLine(const std::string &s) {
    std::smatch match;
    std::regex re(R"((GET|POST) (.+) HTTP\/(.*)$)");

    if (! std::regex_match(s, match, re)) {
        throw BadRequestError();
    }

    return std::make_tuple(match.str(1), match.str(2), match.str(3));
}

MessageHeader
parseMessageHeader(const std::string &s) {
    std::smatch match;
    std::regex re(R"((.*):(.*)$)");

    if (! std::regex_match(s, match, re)) {
        throw BadRequestError();
    }

    return std::make_tuple(match.str(1), match.str(2));
}

Request
parseRequest(boost::asio::ip::tcp::socket &socket) {
    using namespace std::literals;
    static const auto line_delimiter = "\r\n"sv;

    SocketBufferedReader reader(socket);

    const auto request_line = parseRequestLine(reader.readLine(line_delimiter));

    std::vector<MessageHeader> headers;
    do {
        const auto line = reader.readLine(line_delimiter);

        if (!line.empty()) {
            headers.emplace_back(parseMessageHeader(line));
        } else break; // find the empty line
    } while (true);

    return Request{
        .requestLine = request_line,
        .headers = headers,
    };
}
}

Connection::pointer Connection::create(io_context &context) {
    return pointer(new Connection(context));
}

tcp::socket &
Connection::socket() {
    return socket_;
}

void
Connection::run() {
    const auto req = parseRequest(socket_);
    const auto [method, uri, version] = req.requestLine;

    std::cout << " method: " << method  << '\n';
    std::cout << "    uri: " << uri     << '\n';
    std::cout << "version: " << version << '\n';

    std::cout << "Headers: " << '\n';
    for (const auto [key, value]: req.headers) {
        std::cout << "- " << key << ": " << value << '\n';
    }

    boost::asio::write(socket_, boost::asio::buffer("HTTP-1.0 200 OK\r\n"));
}


Connection::Connection(io_context &context)
    : socket_(context) {
}

} // namespace khdkhd::http
