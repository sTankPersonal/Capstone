#include "application/chatSessions/dtos/ChatSessionDto.h"
#include <chrono>
#include <string>

static std::string formatDate(const std::chrono::year_month_day& d) {
    return std::to_string(int(d.year())) + "-"
         + std::to_string(unsigned(d.month())) + "-"
         + std::to_string(unsigned(d.day()));
}

ChatSessionDto::ChatSessionDto(const ChatSession& session)
    : id_(session.getId().getId())
    , userId_(session.getUserId().getId())
    , personaId_(session.getPersonaId().getId())
    , description_(session.getSessionDescription().getDescription())
    , createdAt_(formatDate(session.getCreatedAt()))
    , updatedAt_(formatDate(session.getUpdatedAt()))
{
    const auto& info = session.getSessionDescription().getAdditionalInfo();
    if (info) additionalInfo_ = *info;
}

std::string ChatSessionDto::getUserId() const { return userId_; }

ChatSessionDto::operator crow::json::wvalue() const {
    crow::json::wvalue result;
    result["id"]          = id_;
    result["userId"]      = userId_;
    result["personaId"]   = personaId_;
    result["description"] = description_;
    if (additionalInfo_) result["additionalInfo"] = *additionalInfo_;
    result["createdAt"]   = createdAt_;
    result["updatedAt"]   = updatedAt_;
    return result;
}
