#pragma once

#include <crow.h>
#include <optional>
#include <string>
#include "application/user/GetUserProfile.h"
#include "application/user/UpdateUserProfile.h"
#include "application/user/UpdateUserPassword.h"
#include "application/persona/CreatePersona.h"
#include "application/persona/DeletePersona.h"
#include "application/persona/GetPersona.h"
#include "application/persona/ListPersonas.h"
#include "application/persona/UpdatePersona.h"
#include "IUserRepository.h"
#include "ILlmPersonaRepository.h"
#include "infrastructureServices/security/IJwtService.h"
#include "infrastructureServices/security/IPasswordHasher.h"

class SettingsController {
    GetUserProfile     getUserProfile_;
    UpdateUserProfile  updateUserProfile_;
    UpdateUserPassword updatePassword_;
    CreatePersona      createPersona_;
    DeletePersona      deletePersona_;
    GetPersona         getPersona_;
    ListPersonas       listPersonas_;
    UpdatePersona      updatePersona_;
    IJwtService&       jwt_;

    std::optional<std::string> extractUserId(const crow::request& req) const;

public:
    SettingsController(IUserRepository&      userRepo,
                       ILlmPersonaRepository& personaRepo,
                       IPasswordHasher&       hasher,
                       IJwtService&           jwt);

    void registerRoutes(crow::SimpleApp& app);
};
