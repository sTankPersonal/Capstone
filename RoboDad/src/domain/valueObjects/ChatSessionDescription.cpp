#include "ChatSessionDescription.h"

ChatSessionDescription::ChatSessionDescription(const std::string& description, const std::optional<std::string>& additionalInfo)
    : description_(description), additionalInfo_(additionalInfo) {}

const std::string& ChatSessionDescription::getDescription() const noexcept { return description_; }
const std::optional<std::string>& ChatSessionDescription::getAdditionalInfo() const noexcept { return additionalInfo_; }

void ChatSessionDescription::setDescription(const std::string& description) { description_ = description; }
void ChatSessionDescription::setAdditionalInfo(const std::optional<std::string>& additionalInfo) { additionalInfo_ = additionalInfo; }
