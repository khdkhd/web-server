#include "http_request_parser.hpp"
#include "http_errors.hpp"

#include <iostream>
#include <algorithm>
#include <regex>

using namespace boost::asio;

using error_code = boost::system::error_code;

namespace {

    bool is_char(int c) {
        return c >= 0 && c <= 127;
    }

    bool is_ctl(int c) {
        return (c >= 0 && c <= 31) || (c == 127);
    }

    bool is_special(int c) {
        switch (c) {
            case '(':
            case ')':
            case '<':
            case '>':
            case '@':
            case ',':
            case ';':
            case ':':
            case '\\':
            case '"':
            case '/':
            case '[':
            case ']':
            case '?':
            case '=':
            case '{':
            case '}':
            case ' ':
            case '\t':
                return true;
            default:
                return false;
        }
    }

    bool is_digit(int c) {
        return c >= '0' && c <= '9';
    }

    state handle_start(http_request &request, char input) {
        if (!is_char(input) || is_ctl(input) || is_special(input)) {
            throw http_errors::bad_request();
        }

        request.method.push_back(input);
        return state::method;
    }

    state handle_method(http_request &request, char input) {
        if (input == ' ') {
            return state::uri;
        }

        if (!is_char(input) || is_ctl(input) || is_special(input)) {
            throw http_errors::bad_request();
        }

        request.method.push_back(input);
        return state::method;
    }

    state handle_uri(http_request &request, char input) {
        if (is_ctl(input)) {
            throw http_errors::bad_request();
        }

        if (input == ' ') {
            return state::version;
        }

        request.location.push_back(input);
        return state::uri;
    }

    state handle_version(http_request &request, char input) {
        if (input == 'H' && request.version.text.empty()) {
            request.version.text.push_back(input);
            return state::version;
        }

        if (request.version.text.empty()) {
            throw http_errors::bad_request();
        }

        if (input == 'T' && request.version.text.back() == 'H') {
            request.version.text.push_back(input);
            return state::version;
        }

        if (input == 'T' && request.version.text.back() == 'T') {
            request.version.text.push_back(input);
            return state::version;
        }

        if (input == 'P' && request.version.text.back() == 'T') {
            request.version.text.push_back(input);
            return state::version;
        }

        if (input == '/' && request.version.text.back() == 'P') {
            request.version.text.push_back(input);
            return state::version;
        }

        if (is_digit(input) && request.version.text.back() == '/') {
            request.version.text.push_back(input);
            request.version.major = input - '0';
            return state::version;
        }

        if (input == '.' && is_digit(request.version.text.back())) {
            request.version.text.push_back(input);
            return state::version;
        }

        if (is_digit(input) && request.version.text.back() == '.') {
            request.version.text.push_back(input);
            request.version.minor = input - '0';
            return state::version;
        }

        if (input == '\r' && is_digit(request.version.text.back())) {
            return state::request_line_end;
        }

        throw http_errors::bad_request();
    }

    state handle_request_line_end(http_request &request, char input) {
        if (input != '\n') {
            throw http_errors::bad_request();
        }

        return state::header_line_start;
    }

    state handle_header_line_start(http_request &request, char input) {
        if (input == '\r') {
            return state::before_body;
        }

        request.headers.emplace_back(http_header{});
        request.headers.back().name.push_back(input);
        return state::header_name;
    }

    state handle_header_name(http_request &request, char input) {
        if (input == ':') {
            return state::header_value;
        }

        request.headers.back().name.push_back(input);
        return state::header_name;
    }

    state handle_header_value(http_request &request, char input) {
        if (input == ' ') {
            return state::header_value;
        }

        if (input == '\r') {
            return state::request_line_end;
        }

        request.headers.back().value.push_back(input);
        return state::header_value;
    }

    state handle_before_body(http_request &request, char input) {
        if (input != '\n') {
            throw http_errors::bad_request();
        }
        request.is_valid = true;
        return state::body;
    }

    state handle_body(http_request &request, char input) {
        if (input == '\r') {
            return state::end;
        }
        request.is_valid = false;
        request.body.push_back(input);
        return state::body;
    }

    void handle_end(http_request &request, char input) {
        if (input != '\n') {
            throw http_errors::bad_request();
        }
        request.is_valid = true;
    }
}

http_request_parser::http_request_parser() : current_state(state::start) {
}

void http_request_parser::consume(http_request &request, char input) {
    switch (current_state) {
        case state::start:
            current_state = handle_start(request, input);
            break;
        case state::method:
            current_state = handle_method(request, input);
            break;
        case state::uri:
            current_state = handle_uri(request, input);
            break;
        case state::version:
            current_state = handle_version(request, input);
            break;
        case state::request_line_end:
            current_state = handle_request_line_end(request, input);
            break;
        case state::header_line_start:
            current_state = handle_header_line_start(request, input);
            break;
        case state::header_name:
            current_state = handle_header_name(request, input);
            break;
        case state::header_value:
            current_state = handle_header_value(request, input);
            break;
        case state::before_body:
            current_state = handle_before_body(request, input);
            break;
        case state::body:
            current_state = handle_body(request, input);
            break;
        case state::end:
            return handle_end(request, input);
    }
}
