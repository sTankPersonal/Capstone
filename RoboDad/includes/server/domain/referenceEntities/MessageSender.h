#pragma once

#include "IEntity.h"
#include "MessageSenderId.h"
#include <chrono>
#include <string>

class MessageSender : public IEntity<MessageSenderId, std::chrono::year_month_day>
{
    MessageSenderId id_;
    std::string name_;
    std::chrono::year_month_day createdAt_;

public:
    explicit MessageSender(
        const MessageSenderId& id,
        const std::string& name,
        const std::chrono::year_month_day& createdAt
    );

    const MessageSenderId& getId() const noexcept override;
    const std::string& getName() const noexcept;
    const std::chrono::year_month_day& getCreatedAt() const noexcept override;

protected:
    void setName(const std::string& name);
};
