#pragma once
#include <string>
#include <map>

namespace http {
namespace server {

struct reply;
struct request;

/// The common handler for all incoming requests.
class request_handler {
public:

    using route_handler = std::function<void(const std::string &, std::string &)>;

    request_handler(const request_handler &) = delete;

    request_handler &operator=(const request_handler &) = delete;

    /// Construct with a directory containing files to be served.
    request_handler();

    /// Handle a request and produce a reply.
    void handle_request(const request &req, reply &rep);

    void add_route(const std::string &routeName, route_handler callback);

    void add404(route_handler callback);


private:
    std::map<std::string, route_handler> mRoutes;

    /// Perform URL-decoding on a string. Returns false if the encoding was
    /// invalid.
    static bool url_decode(const std::string &in, std::string &out);
};

} // namespace server
} // namespace http


