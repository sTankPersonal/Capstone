#pragma once

#include <string>
#include "EmploymentStatus.hpp"

class PersonalInfo {
	std::string name;
	uint8_t age;
	EmploymentStatus employmentStatus;

public:
	PersonalInfo(const std::string& name, uint8_t age, EmploymentStatus employmentStatus)
		: name(name), age(age), employmentStatus(employmentStatus) {
	}

	std::string getName() const { return name; }
	uint8_t getAge() const { return age; }
	EmploymentStatus getEmploymentStatus() const { return employmentStatus; }

	void setName(const std::string& newName) { name = newName; }
	void setAge(uint8_t newAge) { age = newAge; }
	void setEmploymentStatus(EmploymentStatus newStatus) { employmentStatus = newStatus; }
};