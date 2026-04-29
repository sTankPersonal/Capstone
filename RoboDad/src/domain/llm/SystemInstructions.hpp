#pragma once
#include <cstdint>
#include <string>

class SystemInstructions {
    uint32_t Id;
    std::string Personality;
    std::string SafetyGuidelines;
    std::string Goal;

public:
    SystemInstructions() : Id(0) {}

    SystemInstructions(uint32_t id, const std::string& personality,
                       const std::string& safetyGuidelines, const std::string& goal)
        : Id(id), Personality(personality), SafetyGuidelines(safetyGuidelines), Goal(goal) {}

    uint32_t getId() const { return Id; }
    std::string getPersonality() const { return Personality; }
    std::string getSafetyGuidelines() const { return SafetyGuidelines; }
    std::string getGoal() const { return Goal; }

    void setId(uint32_t newId) { Id = newId; }
    void setPersonality(const std::string& p) { Personality = p; }
    void setSafetyGuidelines(const std::string& sg) { SafetyGuidelines = sg; }
    void setGoal(const std::string& g) { Goal = g; }
};