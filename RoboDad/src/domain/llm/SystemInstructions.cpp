#include "domain/llm/SystemInstructions.h"

SystemInstructions::SystemInstructions() : Id(0) {}

SystemInstructions::SystemInstructions(uint32_t id, const std::string& personality,
                                       const std::string& safetyGuidelines, const std::string& goal)
    : Id(id), Personality(personality), SafetyGuidelines(safetyGuidelines), Goal(goal) {}

uint32_t SystemInstructions::getId() const { return Id; }
std::string SystemInstructions::getPersonality() const { return Personality; }
std::string SystemInstructions::getSafetyGuidelines() const { return SafetyGuidelines; }
std::string SystemInstructions::getGoal() const { return Goal; }

void SystemInstructions::setId(uint32_t newId) { Id = newId; }
void SystemInstructions::setPersonality(const std::string& p) { Personality = p; }
void SystemInstructions::setSafetyGuidelines(const std::string& sg) { SafetyGuidelines = sg; }
void SystemInstructions::setGoal(const std::string& g) { Goal = g; }
