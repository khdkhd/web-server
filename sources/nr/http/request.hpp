#pragma once

namespace khdkhd::http {

using RequestLine = std::tuple<std::string, std::string, std::string>;
using MessageHeader = std::tuple<std::string, std::string>;

struct Request {
    RequestLine requestLine;
    std::vector<MessageHeader> headers;
};

} // namespace khdkhd::http