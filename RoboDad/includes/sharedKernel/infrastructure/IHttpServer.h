#pragma once

#include <cstdint>

class IHttpServer {
public:
    virtual ~IHttpServer() = default;
    virtual void start(uint16_t port) = 0;
};
