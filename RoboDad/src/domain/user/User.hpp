#pragma once
#include <vector>
#include "Finance.hpp"
#include "PersonalInfo.hpp"

class User {
	uint32_t id;
	PersonalInfo personalInfo;
	std::vector<Finance> finances;
	

public:
	User(uint32_t id, const PersonalInfo& personalInfo, const std::vector<Finance>& finances)
		: id(id), personalInfo(personalInfo), finances(finances) {
	}

	uint32_t getId() const { return id; }
	PersonalInfo getPersonalInfo() const { return personalInfo; }
	std::vector<Finance> getFinances() const { return finances; }

	void setId(uint32_t newId) { id = newId; }
	void setPersonalInfo(const PersonalInfo& newPersonalInfo) { personalInfo = newPersonalInfo; }
	void setFinances(const std::vector<Finance>& newFinances) { finances = newFinances; }
}