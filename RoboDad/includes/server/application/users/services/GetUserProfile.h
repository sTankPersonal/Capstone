#pragma once
#include "IUseCase.h"
#include "GetUserProfileQuery.h"
#include "IUserRepository.h"
#include "UserProfileDto.h"
#include <optional>

class GetUserProfile : public IUseCase<GetUserProfileQuery, std::optional<UserProfileDto>> {
    IUserRepository& repo_;
public:
    explicit GetUserProfile(IUserRepository& repo);
    std::optional<UserProfileDto> execute(const GetUserProfileQuery& request) override;
};
