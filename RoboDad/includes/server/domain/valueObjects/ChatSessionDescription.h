#pragma once

#include "IValueObject.h"
#include <string>
#include <optional>

class ChatSessionDescription : public IValueObject {
    std::string description_;
    std::optional<std::string> additionalInfo_;

public:
    ChatSessionDescription() = default;
    explicit ChatSessionDescription(const std::string& description, const std::optional<std::string>& additionalInfo);

    const std::string& getDescription() const noexcept;
    const std::optional<std::string>& getAdditionalInfo() const noexcept;

    void setDescription(const std::string& description);
    void setAdditionalInfo(const std::optional<std::string>& additionalInfo);
};
