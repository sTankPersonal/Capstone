#pragma once

template<typename TRequest, typename TResult>
class IUseCase {
public:
    virtual ~IUseCase() = default;
    virtual TResult execute(const TRequest& request) = 0;
};
