#include "http_request_handler.hpp"

http_request_handler::http_request_handler(std::vector<std::shared_ptr<http_middleware>> chain): chain(chain) {
    for (auto begin = chain.begin(); begin < chain.end() - 1; ++begin) {
        (*begin)->chain(*(begin + 1));
    }
}

void
http_request_handler::handle_request(const http_request &request, http_response &response) {
    (**chain.begin())(request, response);
}
