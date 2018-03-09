//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2014 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "http/server.hpp"
#include <signal.h>
#include <utility>
#include <sstream>

namespace http {
namespace server {

server::server(asio::io_context &io_context)
        : io_context_(io_context), signals_(io_context_), acceptor_(io_context_), connection_manager_(),
          socket_(io_context_) {

}

server::server(asio::io_context &io_context, const std::string &address,
               unsigned short port, int maxClient)
        : io_context_(io_context), signals_(io_context_), acceptor_(io_context_), connection_manager_(),
          socket_(io_context_) {
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address(address), port);
    // Open the acceptor with the option to reuse the address (i.e.
    // SO_REUSEADDR).
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen(maxClient);
    do_accept();
}

void server::add404(request_handler::route_handler callback) {
    request_handler_.add404(std::move(callback));
}

void
server::addRoute(const std::string &routeName, request_handler::route_handler callback) {
    request_handler_.add_route(routeName, std::move(callback));
}

void
server::handle_request(const request &req, reply &rep) {
    request_handler_.handle_request(req, rep);
}

void
server::do_accept() {
    acceptor_.async_accept(socket_, [this](asio::error_code ec) {
        // Check whether the server was stopped by a signal before this
        // completion handler had a chance to run.
        if (!acceptor_.is_open()) {
            return;
        }

        if (!ec) {
            connection_manager_.start(std::make_shared<connection>(
                    std::move(socket_), connection_manager_, request_handler_));
        }

        do_accept();
    });
}

server::~server() {
    acceptor_.close();
    connection_manager_.stop_all();
}

void server::parseParams(const std::string &params, std::map<std::string, std::string> &retMap) {
    bool buildingName = true;
    std::string name, value;
    for (auto c : params) {
        if (c == '?') {

        } else if (c == '=') {
            buildingName = false;
        } else if (c == '&') {
            buildingName = true;
            retMap[name] = value;
            name = "";
            value = "";
        } else {
            if (buildingName) name += c;
            else value += c;
        }
    }
    if (!name.empty() && !value.empty()) {
        retMap[name] = value;
    }
}

} // namespace server
} // namespace http
