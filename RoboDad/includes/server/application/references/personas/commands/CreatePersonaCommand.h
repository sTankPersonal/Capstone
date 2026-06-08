#pragma once
#include <string>

struct CreatePersonaCommand {
    std::string name;
    std::string description;
    std::string systemPrompt;

    CreatePersonaCommand(std::string name, std::string description, std::string systemPrompt)
        : name(std::move(name)), description(std::move(description)), systemPrompt(std::move(systemPrompt)) {}
};
