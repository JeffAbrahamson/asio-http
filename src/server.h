/*
  Copyright 2019  Jellybooks

  Originally based on example code from Boost ASIO, which bore the
  following copyright notice:

  Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HTTP_SERVER_H__
#define HTTP_SERVER_H__ 1

#include <asio.hpp>
#include <string>

#include "connection.h"
#include "connection_manager.h"
#include "request_handler.h"

namespace http {
namespace server {

/// The top-level class of the HTTP server.
class Server {
   public:
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    /// Construct the server to listen on the specified TCP address and port,
    /// and
    /// serve up files from the given directory.
    explicit Server(const std::string& address, const std::string& port,
                    const std::string& doc_root);

    /// Run the server's io_service loop.
    void Run();

   private:
    /// Perform an asynchronous accept operation.
    void DoAccept();

    /// Wait for a request to stop the server.
    void DoAwaitStop();

    /// The io_service used to perform asynchronous operations.
    asio::io_service io_service_;

    /// The signal_set is used to register for process termination
    /// notifications.
    asio::signal_set signals_;

    /// Acceptor used to listen for incoming connections.
    asio::ip::tcp::acceptor acceptor_;

    /// The connection manager which owns all live connections.
    ConnectionManager connection_manager_;

    /// The next socket to be accepted.
    asio::ip::tcp::socket socket_;

    /// The handler for all incoming requests.
    request_handler request_handler_;
};

}  // namespace server
}  // namespace http

#endif  // HTTP_SERVER_H__
