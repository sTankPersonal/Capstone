#include "infrastructure/http/HttpRequest.h"
#include <iostream>

HttpRequest::HttpRequest(const crow::request& req) : req_(req) {}

std::string HttpRequest::getMethod() const {
    std::string method = crow::method_name(req_.method);
    std::transform(method.begin(), method.end(), method.begin(), ::toupper);
    return method;
}

std::string HttpRequest::getBody() const {
    return req_.body;
}

std::optional<std::string> HttpRequest::getHeader(const std::string& name) const {
    auto it = req_.headers.find(name);
    if (it != req_.headers.end()) {
        return std::string(it->second);
    }
    return std::nullopt;
}

std::optional<std::string> HttpRequest::getQueryParam(const std::string& key) const {
    const char* value = req_.url_params.get(key);
    if (value) {
        return std::string(value);
    }
    return std::nullopt;
}

std::optional<std::string> HttpRequest::getBearerToken() const {
    // TODO: call getHeader("Authorization");
    //       if present and starts with "Bearer ", strip the prefix and return the token;
    //       otherwise return nullopt
    return std::nullopt;
}

crow::json::rvalue HttpRequest::getJsonBody() const {
    auto json = crow::json::load(req_.body);
    if (!json) {
        throw std::runtime_error("Invalid JSON in request body");
    }
    return json;
}
