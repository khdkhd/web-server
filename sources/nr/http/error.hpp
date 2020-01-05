#pragma once

#include <exception>

namespace khdkhd::http {
class Error: public std::exception {
public:
    Error(unsigned int status, const char *msg);

    virtual const char *what() const noexcept override;

private:
    const char *message_;
    unsigned int status_;
};

struct BadRequestError: public Error {
    BadRequestError();
};

} // namespace khdkhd::http
