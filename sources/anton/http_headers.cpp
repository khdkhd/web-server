#include "http_headers.hpp"
#include "http_errors.hpp"

namespace http_headers {
    std::pair<std::string, std::string> parseHeader(const std::string &line)  {
        auto separatorPos = std::find(line.begin(), line.end(), ':');
        if (separatorPos == line.end()) {
            throw http_errors::bad_request();
        }
        auto key = std::string(line.begin(), separatorPos);
        auto value = std::string(++separatorPos, line.end());
        boost::algorithm::trim_left(value);
        return std::make_pair(key, value);
    }
}