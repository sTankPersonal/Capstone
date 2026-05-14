#include "infrastructure/http/HttpResponse.h"

HttpResponse::HttpResponse(int statusCode) : statusCode_(statusCode) {}

HttpResponse& HttpResponse::setStatus(int code) {
    statusCode_ = code;
    return *this;
}

HttpResponse& HttpResponse::setBody(const crow::json::wvalue& json) {
    body_ = json.dump();  // serialize JSON to string
    headers_.emplace_back("Content-Type", "application/json");
    return *this;
}

HttpResponse& HttpResponse::setHeader(const std::string& key, const std::string& value) {
    headers_.emplace_back(key, value);
    return *this;
}

crow::response HttpResponse::toCrowResponse() const {
    crow::response res(statusCode_, body_);
    for (const auto& [key, value] : headers_) {
        res.set_header(key, value);
    }
    return res;
}

// ---- Static factory helpers ----

HttpResponse HttpResponse::ok(const crow::json::wvalue& body) {
    HttpResponse resp(200);
    resp.setBody(body);
    return resp;
}

HttpResponse HttpResponse::created(const crow::json::wvalue& body) {
    HttpResponse resp(201);
    resp.setBody(body);
    return resp;
}

HttpResponse HttpResponse::badRequest(const std::string& message) {
    HttpResponse resp(400);
    resp.setBody(crow::json::wvalue{ {"error", message} });
    return resp;
}

HttpResponse HttpResponse::unauthorized(const std::string& message) {
    HttpResponse resp(401);
    resp.setBody(crow::json::wvalue{ {"error", message} });
    return resp;
}

HttpResponse HttpResponse::notFound(const std::string& message) {
    HttpResponse resp(404);
    resp.setBody(crow::json::wvalue{ {"error", message} });
    return resp;
}

HttpResponse HttpResponse::internalError(const std::string& message) {
    HttpResponse resp(500);
    resp.setBody(crow::json::wvalue{ {"error", message} });
    return resp;
}
