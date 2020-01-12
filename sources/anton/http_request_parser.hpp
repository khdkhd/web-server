#pragma once

#include "http_request.hpp"

#include <string>
#include <utility>
#include <boost/asio.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <functional>

enum class state {
    start,
    method,
    uri,
    version,
    request_line_end,
    header_line_start,
    header_name,
    header_value,
    before_body,
    body,
    end,
};

class http_request_parser {
public:
    http_request_parser();

public:
    template <typename InputIterator>
    void parse(http_request& request, InputIterator begin, InputIterator end) {
        request.is_valid = false;
        while (begin != end) {
            consume(request, *begin++);
        }
    }

private:
    void consume(http_request &request, char c);

private:
    state current_state;
};
