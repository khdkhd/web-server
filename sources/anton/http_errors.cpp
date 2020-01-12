#include "http_errors.hpp"

http_errors::error::error(int status, const std::string &message): std::runtime_error(message.c_str()), status(status) {

}

int http_errors::error::get_status() {
    return status;
}

http_errors::bad_request::bad_request(): error(400, "Bad Request") {}
