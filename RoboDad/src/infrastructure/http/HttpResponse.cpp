#include "infrastructure/http/HttpResponse.h"

HttpResponse::HttpResponse(int statusCode) : statusCode_(statusCode) {}

HttpResponse& HttpResponse::setStatus(int code) {
    // TODO: assign code to statusCode_ and return *this
}

HttpResponse& HttpResponse::setBody(const crow::json::wvalue& json) {
    // TODO: serialize json to a string and assign to body_;
    //       add Content-Type: application/json to headers_;
    //       return *this
}

HttpResponse& HttpResponse::setHeader(const std::string& key, const std::string& value) {
    // TODO: append {key, value} to headers_; return *this
}

crow::response HttpResponse::toCrowResponse() const {
    // TODO: construct a crow::response with statusCode_ and body_;
    //       iterate headers_ and set each one on the response;
    //       return the crow::response
}

// ---- Static factory helpers ----

HttpResponse HttpResponse::ok(const crow::json::wvalue& body) {
    // TODO: construct HttpResponse(200), call setBody(body), return it
}

HttpResponse HttpResponse::created(const crow::json::wvalue& body) {
    // TODO: construct HttpResponse(201), call setBody(body), return it
}

HttpResponse HttpResponse::badRequest(const std::string& message) {
    // TODO: construct HttpResponse(400), call setBody({"error": message}), return it
}

HttpResponse HttpResponse::unauthorized(const std::string& message) {
    // TODO: construct HttpResponse(401), call setBody({"error": message}), return it
}

HttpResponse HttpResponse::notFound(const std::string& message) {
    // TODO: construct HttpResponse(404), call setBody({"error": message}), return it
}

HttpResponse HttpResponse::internalError(const std::string& message) {
    // TODO: construct HttpResponse(500), call setBody({"error": message}), return it
}
