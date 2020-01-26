#include "http_middleware.hpp"

void http_middleware::chain(std::shared_ptr<http_middleware> _next) {
    next = _next;
}

void http_middleware::operator()(const http_request &request, http_response &response) {
    handler(request, response, next);
}


