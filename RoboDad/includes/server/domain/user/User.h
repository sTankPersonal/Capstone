#pragma once
#include "domain/IAggregateRoot.h"
#include "domain/user/PersonalInfo.h"
#include "domain/finance/Finance.h"
#include <cstdint>
#include <vector>

class User : public IAggregateRoot<uint32_t> {
    uint32_t id;
    PersonalInfo personalInfo;
    std::vector<Finance> finances;

public:
    User(uint32_t id, const PersonalInfo& personalInfo, const std::vector<Finance>& finances);

    uint32_t getId() const override;
    PersonalInfo getPersonalInfo() const;
    std::vector<Finance> getFinances() const;

    void setId(uint32_t newId);
    void setPersonalInfo(const PersonalInfo& newPersonalInfo);
    void setFinances(const std::vector<Finance>& newFinances);
};
