#pragma once
#include "IUseCase.h"
#include "CreatePlaidLinkTokenCommand.h"
#include "IPlaidClient.h"
#include <string>

class CreatePlaidLinkToken : public IUseCase<CreatePlaidLinkTokenCommand, std::string> {
    IPlaidClient& plaidClient_;
public:
    explicit CreatePlaidLinkToken(IPlaidClient& plaidClient);
    std::string execute(const CreatePlaidLinkTokenCommand& request) override;
};
