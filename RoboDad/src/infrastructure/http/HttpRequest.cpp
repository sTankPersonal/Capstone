#include "infrastructure/http/HttpRequest.h"

HttpRequest::HttpRequest(const crow::request& req) : req_(req) {}

std::string HttpRequest::getMethod() const {
    // TODO: return crow::method_name(req_.method) converted to uppercase string
}

std::string HttpRequest::getBody() const {
    // TODO: return req_.body
}

std::optional<std::string> HttpRequest::getHeader(const std::string& name) const {
    // TODO: look up name in req_.headers (case-insensitive);
    //       return the value, or nullopt if the header is not present
}

std::optional<std::string> HttpRequest::getQueryParam(const std::string& key) const {
    // TODO: parse req_.url_params; return the value for key, or nullopt if absent
}

std::optional<std::string> HttpRequest::getBearerToken() const {
    // TODO: call getHeader("Authorization");
    //       if present and starts with "Bearer ", strip the prefix and return the token;
    //       otherwise return nullopt
}

crow::json::rvalue HttpRequest::getJsonBody() const {
    // TODO: call crow::json::load(req_.body);
    //       if the result is invalid JSON throw std::runtime_error with a descriptive message;
    //       otherwise return the parsed rvalue
}
