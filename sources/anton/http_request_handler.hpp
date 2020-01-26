#pragma once

#include "http_request.hpp"
#include "http_response.hpp"
#include "http_middleware.hpp"

#include <functional>

class http_request_handler {
public:
    explicit http_request_handler(std::vector<std::shared_ptr<http_middleware>> chain);

public:
    void handle_request(const http_request &request, http_response &response);

private:
    std::vector<std::shared_ptr<http_middleware>> chain;
};



