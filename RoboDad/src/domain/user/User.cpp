#include "domain/user/User.h"

User::User(uint32_t id, const PersonalInfo& personalInfo, const std::vector<Finance>& finances)
    : id(id), personalInfo(personalInfo), finances(finances) {}

uint32_t User::getId() const { return id; }
PersonalInfo User::getPersonalInfo() const { return personalInfo; }
std::vector<Finance> User::getFinances() const { return finances; }

void User::setId(uint32_t newId) { id = newId; }
void User::setPersonalInfo(const PersonalInfo& newPersonalInfo) { personalInfo = newPersonalInfo; }
void User::setFinances(const std::vector<Finance>& newFinances) { finances = newFinances; }
