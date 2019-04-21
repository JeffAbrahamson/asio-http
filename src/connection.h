/*
  Copyright 2019  Jellybooks

  Originally based on example code from Boost ASIO, which bore the
  following copyright notice:

  Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HTTP_CONNECTION_H__
#define HTTP_CONNECTION_H__ 1

#include <array>
#include <asio.hpp>
#include <memory>

#include "reply.h"
#include "request.h"
#include "request_handler.h"
#include "request_parser.h"

namespace http {
namespace server {

class ConnectionManager;

/// Represents a single connection from a client.
class Connection : public std::enable_shared_from_this<Connection> {
   public:
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    /// Construct a connection with the given socket.
    explicit Connection(asio::ip::tcp::socket socket,
                        ConnectionManager& manager, request_handler& handler);

    /// Start the first asynchronous operation for the connection.
    void Start();

    /// Stop all asynchronous operations associated with the connection.
    void Stop();

   private:
    /// Perform an asynchronous read operation.
    void DoRead();

    /// Perform an asynchronous write operation.
    void DoWrite();

    /// Socket for the connection.
    asio::ip::tcp::socket socket_;

    /// The manager for this connection.
    ConnectionManager& connection_manager_;

    /// The handler used to process the incoming request.
    request_handler& request_handler_;

    /// Buffer for incoming data.
    std::array<char, 8192> buffer_;

    /// The incoming request.
    Request request_;

    /// The parser for the incoming request.
    RequestParser request_parser_;

    /// The reply to be sent back to the client.
    reply reply_;
};

typedef std::shared_ptr<Connection> connection_ptr;

}  // namespace server
}  // namespace http

#endif  // HTTP_CONNECTION_H__
