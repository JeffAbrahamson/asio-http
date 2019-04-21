/*
  Copyright 2019  Jellybooks

  Originally based on example code from Boost ASIO, which bore the
  following copyright notice:

  Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HTTP_HEADER_H__
#define HTTP_HEADER_H__ 1

#include <string>

namespace http {
namespace server {

struct Header {
    std::string name;
    std::string value;
};

}  // namespace server
}  // namespace http

#endif  // HTTP_HEADER_H__
