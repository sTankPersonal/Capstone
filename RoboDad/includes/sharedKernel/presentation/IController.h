#pragma once

template<typename TApp>
class IController{
public:
    virtual ~IController() = default;

    virtual void registerRoutes(TApp& app) = 0;
};