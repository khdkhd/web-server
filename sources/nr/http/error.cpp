#include "error.hpp"

namespace khdkhd::http {

Error::Error(unsigned int status, const char *msg)
    : message_{msg}
    , status_{status} {
}

const char *
Error::what() const noexcept {
    return message_;
}

BadRequestError::BadRequestError()
    : Error(400, "Bad request") {
}

} // namespace khdkhd::http
