/*
  Copyright 2019  Jellybooks

  Originally based on example code from Boost ASIO, which bore the
  following copyright notice:

  Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HTTP_REQUEST_H__
#define HTTP_REQUEST_H__ 1

#include <string>
#include <vector>

#include "header.h"

namespace http {
namespace server {

/// A request received from a client.
struct Request {
    std::string method;
    std::string uri;
    int http_version_major;
    int http_version_minor;
    std::vector<Header> headers;
};

}  // namespace server
}  // namespace http

#endif  // HTTP_REQUEST_H__
