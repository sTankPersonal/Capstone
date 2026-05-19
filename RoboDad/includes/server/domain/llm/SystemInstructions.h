#pragma once
#include "domain/IEntity.h"
#include <cstdint>
#include <string>

class SystemInstructions : public IEntity<uint32_t> {
    uint32_t Id;
    std::string Personality;
    std::string SafetyGuidelines;
    std::string Goal;

public:
    SystemInstructions();
    SystemInstructions(uint32_t id, const std::string& personality,
                       const std::string& safetyGuidelines, const std::string& goal);

    uint32_t getId() const override;
    std::string getPersonality() const;
    std::string getSafetyGuidelines() const;
    std::string getGoal() const;

    void setId(uint32_t newId);
    void setPersonality(const std::string& p);
    void setSafetyGuidelines(const std::string& sg);
    void setGoal(const std::string& g);
};
