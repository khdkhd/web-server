#pragma once

#include "http_request.hpp"
#include "http_response.hpp"

#include <functional>
#include <vector>

static int n = 0;

class http_middleware {
public:
    http_middleware(const std::function<void(const http_request&, http_response&, std::shared_ptr<http_middleware>)> handler): handler(handler), id(++n) {}
public:
    void chain(std::shared_ptr<http_middleware> _next);
public:
    void operator()(const http_request&, http_response&);
private:
    std::function<void(const http_request&, http_response&, std::shared_ptr<http_middleware>)> handler;
    std::shared_ptr<http_middleware> next;
    int id;
};



