#include "domain/user/PersonalInfo.h"

PersonalInfo::PersonalInfo(const std::string& name, uint8_t age, EmploymentStatus employmentStatus)
    : name(name), age(age), employmentStatus(employmentStatus) {}

std::string PersonalInfo::getName() const { return name; }
uint8_t PersonalInfo::getAge() const { return age; }
EmploymentStatus PersonalInfo::getEmploymentStatus() const { return employmentStatus; }

void PersonalInfo::setName(const std::string& newName) { name = newName; }
void PersonalInfo::setAge(uint8_t newAge) { age = newAge; }
void PersonalInfo::setEmploymentStatus(EmploymentStatus newStatus) { employmentStatus = newStatus; }
