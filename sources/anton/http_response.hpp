#pragma once

#include "http_version.hpp"
#include "http_header.hpp"

#include <vector>

struct http_response {
    int status;
    std::string message;
    http_version version;
    std::vector<http_header> headers;
    std::string body;
};



