#pragma once

#include "http_version.hpp"
#include "http_header.hpp"

#include <vector>

struct http_response {
    int status;
    http_version version;
    std::vector<http_header> headers;
    std::string body;
};



