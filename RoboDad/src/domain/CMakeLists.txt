# Domain layer – pure business logic, no third-party dependencies.
add_library(roboDadDomain STATIC
    budget/BudgetGoal.cpp
    finance/Finance.cpp
    llm/ChatHistory.cpp
    llm/Prompt.cpp
    llm/SystemInstructions.cpp
    user/PersonalInfo.cpp
    user/User.cpp
)

target_link_libraries(roboDadDomain PUBLIC server_headers)
