#pragma once

#include <string>
#include <vector>
#include <utility>
#include "http_version.hpp"
#include "http_header.hpp"

using str_pair = std::pair<std::string, std::string>;

struct http_request {
    std::string method;
    std::string location;
    std::vector<http_header> headers;
    std::string body;
    http_version version;
    bool is_valid;
};


