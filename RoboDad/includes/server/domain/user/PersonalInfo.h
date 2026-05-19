#pragma once
#include "domain/IValueObject.h"
#include "domain/user/EmploymentStatus.h"
#include <cstdint>
#include <string>

class PersonalInfo : public IValueObject {
    std::string name;
    uint8_t age;
    EmploymentStatus employmentStatus;

public:
    PersonalInfo(const std::string& name, uint8_t age, EmploymentStatus employmentStatus);

    std::string getName() const;
    uint8_t getAge() const;
    EmploymentStatus getEmploymentStatus() const;

    void setName(const std::string& newName);
    void setAge(uint8_t newAge);
    void setEmploymentStatus(EmploymentStatus newStatus);
};
