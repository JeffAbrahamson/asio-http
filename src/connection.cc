/*
  Copyright 2019  Jellybooks

  Originally based on example code from Boost ASIO, which bore the
  following copyright notice:

  Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include "connection.h"

#include <utility>
#include <vector>

#include "connection_manager.h"
#include "request_handler.h"

namespace http {
namespace server {

Connection::Connection(asio::ip::tcp::socket socket,
                       ConnectionManager& manager, request_handler& handler)
    : socket_(std::move(socket)),
      connection_manager_(manager),
      request_handler_(handler) {}

void Connection::Start() { DoRead(); }

void Connection::Stop() { socket_.close(); }

void Connection::DoRead() {
    auto self(shared_from_this());
    socket_.async_read_some(
        asio::buffer(buffer_),
        [this, self](std::error_code ec, std::size_t bytes_transferred) {
            if (!ec) {
                request_parser::result_type result;
                std::tie(result, std::ignore) =
                    request_parser_.parse(request_, buffer_.data(),
                                          buffer_.data() + bytes_transferred);

                if (result == request_parser::good) {
                    request_handler_.handle_request(request_, reply_);
                    DoWrite();
                } else if (result == request_parser::bad) {
                    reply_ = reply::stock_reply(reply::bad_request);
                    DoWrite();
                } else {
                    DoRead();
                }
            } else if (ec != asio::error::operation_aborted) {
                connection_manager_.Stop(shared_from_this());
            }
        });
}

void Connection::DoWrite() {
    auto self(shared_from_this());
    asio::async_write(socket_, reply_.to_buffers(), [this, self](
                                                        std::error_code ec,
                                                        std::size_t) {
        if (!ec) {
            // Initiate graceful connection closure.
            asio::error_code ignored_ec;
            socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
        }

        if (ec != asio::error::operation_aborted) {
            connection_manager_.Stop(shared_from_this());
        }
    });
}

}  // namespace server
}  // namespace http
