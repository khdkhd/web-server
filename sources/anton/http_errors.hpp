#pragma once

#include <exception>
#include <string>

namespace http_errors {
   class error: public std::runtime_error {
   public:
       error(int status, const std::string& message);
   public:
       int get_status();
   private:
       int status;
       std::string message;
   };

   class bad_request: public error {
   public:
       bad_request();
   };
};



