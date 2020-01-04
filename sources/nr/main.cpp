#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string_view>
#include <regex>
#include <tuple>

struct BadRequestError : public std::exception {
};

class SocketBufferedReader {
    static constexpr std::string_view delimiter_ = std::string_view("\r\n");

    boost::asio::ip::tcp::socket &socket_;
    std::string buffer_;

public:
    SocketBufferedReader(boost::asio::ip::tcp::socket &socket)
        : socket_{socket} {
    }

    std::string
    readLine() {
        using boost::asio::read_until;
        using boost::asio::dynamic_buffer;
        using std::search;

        auto first = buffer_.cbegin();
        auto last = buffer_.cend();
        auto it = search(first, last, delimiter_.begin(), delimiter_.end());

        if (it != last) {
            std::string line(first, it);
            buffer_.erase(first, it + 2);
            return line;
        }

        read_until(socket_, dynamic_buffer(buffer_), delimiter_);

        return readLine();
    }
};

using HTTPRequestLine = std::tuple<std::string, std::string, std::string>;

HTTPRequestLine
parseHTTPRequestLine(const std::string &s) {
    std::smatch match;
    std::regex re(R"((GET|POST) (.+) HTTP\/(.*)$)");

    if (! std::regex_match(s, match, re)) {
        throw BadRequestError();
    }

    return std::make_tuple(match.str(1), match.str(2), match.str(3));
}

using HTTPMessageHeader = std::tuple<std::string, std::string>;

HTTPMessageHeader
parseHTTPMessageHeader(const std::string &s) {
    std::smatch match;
    std::regex re(R"((.*):(.*)$)");

    if (! std::regex_match(s, match, re)) {
        throw BadRequestError();
    }

    return std::make_tuple(match.str(1), match.str(2));
}

struct HTTPRequest {
    HTTPRequestLine requestLine;
    std::vector<HTTPMessageHeader> headers;
};

HTTPRequest
parseHTTPRequest(boost::asio::ip::tcp::socket &socket) {
    SocketBufferedReader reader(socket);

    const auto request_line = parseHTTPRequestLine(reader.readLine());

    std::vector<HTTPMessageHeader> headers;
    do {
        const auto line = reader.readLine();

        if (!line.empty()) {
            headers.emplace_back(parseHTTPMessageHeader(line));
        } else break; // find the empty line
    } while (true);

    return HTTPRequest{
        .requestLine = request_line,
        .headers = headers,
    };
}

void
handleConnection(boost::asio::ip::tcp::socket &socket) {
    const auto req = parseHTTPRequest(socket);

    const auto [method, uri, version] = req.requestLine;
    
    std::cout << " method: " << method  << '\n';
    std::cout << "    uri: " << uri     << '\n';
    std::cout << "version: " << version << '\n';

    std::cout << "Headers: " << '\n';
    for (const auto [key, value]: req.headers) {
        std::cout << "- " << key << ": " << value << '\n';
    }

    boost::asio::write(socket, boost::asio::buffer("HTTP-1.0 200 OK\r\n"));
}

int
main(int argc, char **argv) {
    using boost::asio::ip::tcp;

    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

    do {
        auto socket = std::make_shared<tcp::socket>(io_context);
        acceptor.accept(*socket);

        boost::thread([socket]() {
            handleConnection(*socket);
        });
    } while (true);

    return 0;
}