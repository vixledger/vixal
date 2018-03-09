

#include <ios>
#include <sstream>
#include "http/reply.hpp"
#include "http/request.hpp"
#include "http/request_handler.h"

namespace http {
namespace server {

request_handler::request_handler() {
}

void
request_handler::add_route(const std::string &routeName, route_handler callback) {
    mRoutes[routeName] = std::move(callback);
}

void
request_handler::add404(route_handler callback) {
   add_route("404", callback);
}

void request_handler::handle_request(const request &req, reply &rep) {
    // Decode url to path.
    std::string request_path;
    if (!url_decode(req.uri, request_path)) {
        rep = reply::stock_reply(reply::bad_request);
        return;
    }

    if (!request_path.empty() && request_path[0] == '/') {
        request_path = request_path.substr(1);
    }

    std::string command;
    std::string params;
    auto pos = request_path.find('?');
    if (pos == std::string::npos)
        command = request_path;
    else {
        command = request_path.substr(0, pos);
        params = request_path.substr(pos);
    }

    if (mRoutes.find(command) != mRoutes.end()) {
        mRoutes[command](params, rep.content);

        rep.status = reply::ok;
        rep.headers.resize(2);
        rep.headers[0].name = "Content-Length";
        rep.headers[0].value = std::to_string(rep.content.size());
        rep.headers[1].name = "Content-Type";
        rep.headers[1].value = "application/json";
    } else {
        if (mRoutes.find("404") != mRoutes.end()) {
            mRoutes["404"](params, rep.content);

            rep.status = reply::ok;
            rep.headers.resize(2);
            rep.headers[0].name = "Content-Length";
            rep.headers[0].value = std::to_string(rep.content.size());
            rep.headers[1].name = "Content-Type";
            rep.headers[1].value = "text/html";
        } else {
            rep = reply::stock_reply(reply::not_found);
            return;
        }
    }
}

bool request_handler::url_decode(const std::string &in, std::string &out) {
    out.clear();
    out.reserve(in.size());
    for (std::size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '%') {
            if (i + 3 <= in.size()) {
                int value = 0;
                std::istringstream is(in.substr(i + 1, 2));
                if (is >> std::hex >> value) {
                    out += static_cast<char>(value);
                    i += 2;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else if (in[i] == '+') {
            out += ' ';
        } else {
            out += in[i];
        }
    }
    return true;
}

}
}