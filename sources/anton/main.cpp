#include "http_server.hpp"

#include <iostream>

int main() {
    std::vector<std::shared_ptr<http_middleware>> middlewares;
    auto logger = [](const http_request &request, http_response &response, std::shared_ptr<http_middleware> next) {
        std::cout << request.method << " " << request.location << " " << request.version.text << std::endl;
        (*next)(request, response);
    };
    auto filer = [](const http_request &request, http_response &response, std::shared_ptr<http_middleware> next) {
        response.version = request.version;
        response.message = "OK";
        response.status = 200;
    };
    middlewares.emplace_back(std::make_shared<http_middleware>(logger));
    middlewares.emplace_back(std::make_shared<http_middleware>(filer));
    const http_request_handler handler{std::move(middlewares)};
    http_server(handler).start();
    return 0;
}