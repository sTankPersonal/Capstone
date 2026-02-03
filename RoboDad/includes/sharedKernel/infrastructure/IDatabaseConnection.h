#pragma once

class IDatabaseConnection {
public:
    virtual ~IDatabaseConnection() = default;
    virtual bool isConnected() const noexcept = 0;
};
