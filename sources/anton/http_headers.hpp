#pragma once

#include <string>
#include <utility>
#include <boost/algorithm/string/trim.hpp>

namespace http_headers {
    std::pair<std::string, std::string> parseHeader(const std::string &line);
}

struct http_header {
    std::string name;
    std::string value;
};