/*
  Copyright 2019  Jellybooks

  Originally based on example code from Boost ASIO, which bore the
  following copyright notice:

  Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HTTP_REQUEST_PARSER_H__
#define HTTP_REQUEST_PARSER_H__ 1

#include <tuple>

namespace http {
namespace server {

struct Request;

/// Parser for incoming requests.
class RequestParser {
   public:
    // Construct ready to parse the request method.
    RequestParser();

    // Reset to initial parser state.
    void Reset();

    // Result of parse.
    enum result_type { good, bad, indeterminate };

    /*
      Parse some data. The enum return value is good when a complete
      request has been parsed, bad if the data is invalid,
      indeterminate when more data is required. The InputIterator
      return value indicates how much of the input has been consumed.
    */
    template <typename InputIterator>
    std::tuple<result_type, InputIterator> parse(Request& req,
                                                 InputIterator begin,
                                                 InputIterator end) {
        while (begin != end) {
            result_type result = Consume(req, *begin++);
            if (result == good || result == bad)
                return std::make_tuple(result, begin);
        }
        return std::make_tuple(indeterminate, begin);
    }

   private:
    // Handle the next character of input.
    result_type Consume(Request& req, char input);

    // Check if a byte is an HTTP character.
    static bool IsChar(int c);

    // Check if a byte is an HTTP control character.
    static bool IsCtl(int c);

    // Check if a byte is defined as an HTTP tspecial character.
    static bool IsTspecial(int c);

    // Check if a byte is a digit.
    static bool IsDigit(int c);

    // The current state of the parser.
    enum state {
        method_start,
        method,
        uri,
        http_version_h,
        http_version_t_1,
        http_version_t_2,
        http_version_p,
        http_version_slash,
        http_version_major_start,
        http_version_major,
        http_version_minor_start,
        http_version_minor,
        expecting_newline_1,
        header_line_start,
        header_lws,
        header_name,
        space_before_header_value,
        header_value,
        expecting_newline_2,
        expecting_newline_3
    } state_;
};

}  // namespace server
}  // namespace http

#endif  // HTTP_REQUEST_PARSER_H__
