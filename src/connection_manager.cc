/*
  Copyright 2019  Jellybooks

  Originally based on example code from Boost ASIO, which bore the
  following copyright notice:

  Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include "connection_manager.h"

namespace http {
namespace server {

ConnectionManager::ConnectionManager() {}

void ConnectionManager::Start(connection_ptr c) {
    connections_.insert(c);
    c->Start();
}

void ConnectionManager::Stop(connection_ptr c) {
    connections_.erase(c);
    c->Stop();
}

void ConnectionManager::StopAll() {
    for (auto c : connections_) c->Stop();
    connections_.clear();
}

}  // namespace server
}  // namespace http
