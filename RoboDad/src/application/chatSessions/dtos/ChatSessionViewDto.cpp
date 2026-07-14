#include "application/chatSessions/dtos/ChatSessionViewDto.h"
#include <chrono>
#include <string>

static std::string formatDate(const std::chrono::year_month_day& d) {
    return std::to_string(int(d.year())) + "-"
         + std::to_string(unsigned(d.month())) + "-"
         + std::to_string(unsigned(d.day()));
}

ChatSessionViewDto::ChatSessionViewDto(const ChatSession& session, const LlmPersona& persona)
    : id_(session.getId().getId())
    , userId_(session.getUserId().getId())
    , personaId_(session.getPersonaId().getId())
    , personaName_(persona.getName())
    , personaDescription_(persona.getDescription())
    , description_(session.getSessionDescription().getDescription())
    , createdAt_(formatDate(session.getCreatedAt()))
    , updatedAt_(formatDate(session.getUpdatedAt()))
{
    const auto& info = session.getSessionDescription().getAdditionalInfo();
    if (info) additionalInfo_ = *info;
}

std::string ChatSessionViewDto::getUserId() const { return userId_; }

ChatSessionViewDto::operator crow::json::wvalue() const {
    crow::json::wvalue result;
    result["id"]                 = id_;
    result["userId"]             = userId_;
    result["personaId"]          = personaId_;
    result["personaName"]        = personaName_;
    result["personaDescription"] = personaDescription_;
    result["description"]        = description_;
    if (additionalInfo_) result["additionalInfo"] = *additionalInfo_;
    result["createdAt"] = createdAt_;
    result["updatedAt"] = updatedAt_;
    return result;
}
