#Create an interface library for header-only domain layer
add_library(roboDadDomain INTERFACE)

#Specify include directories
target_include_directories(roboDadDomain INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

#List headers for IDE visibility
target_sources(roboDadDomain INTERFACE
    budget/BudgetGoal.hpp
    finance/FinanceEnum.hpp
    finance/Finance.hpp
    user/FinacialProfile.hpp
    user/PersonalInfo.hpp
    user/User.hpp
)