#pragma once

#include "presentation/middleware/AuthMiddleware.h"
#include "presentation/middleware/AdminMiddleware.h"
#include "infrastructure/config/AppConfig.h"
#include "infrastructure/persistence/DatabaseConnection.h"

#include "infrastructure/persistence/postgres/PostgresUserRepository.h"
#include "infrastructure/persistence/postgres/PostgresChatSessionRepository.h"
#include "infrastructure/persistence/postgres/PostgresChatMessageRepository.h"
#include "infrastructure/persistence/postgres/PostgresLlmPersonaRepository.h"
#include "infrastructure/persistence/postgres/PostgresTransactionRepository.h"
#include "infrastructure/persistence/postgres/PostgresTransactionCategoryRepository.h"
#include "infrastructure/persistence/postgres/PostgresCurrencyRepository.h"
#include "infrastructure/persistence/postgres/PostgresCountryRepository.h"
#include "infrastructure/persistence/postgres/PostgresEmploymentStatusRepository.h"
#include "infrastructure/persistence/postgres/PostgresLanguageRepository.h"
#include "infrastructure/persistence/postgres/PostgresMessageSenderRepository.h"
#include "infrastructure/persistence/postgres/PostgresPlaidItemRepository.h"
#include "infrastructure/persistence/postgres/PostgresPfcPrimaryCategoryRepository.h"
#include "infrastructure/persistence/postgres/PostgresPfcDetailedCategoryRepository.h"

#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"
#include "infrastructure/apiClient/openAiClient/PromptBuilder.h"
#include "infrastructure/apiClient/plaidClient/plaidClient.h"
#include "infrastructure/apiClient/googleAuthClient/GoogleOAuthService.h"
#include "infrastructure/security/JwtService.h"
#include "infrastructure/security/PasswordHasher.h"

#include "presentation/AppType.h"
#include "presentation/middleware/LoggingMiddleware.h"

#include "application/users/services/LoginUser.h"
#include "application/users/services/RegisterUser.h"
#include "application/users/services/LoginOrRegisterOAuthUser.h"
#include "application/users/services/GetUserProfile.h"
#include "application/users/services/UpdateUserProfile.h"
#include "application/users/services/UpdateUserPassword.h"
#include "application/users/services/DeleteUser.h"

#include "application/chatSessions/services/CreateChatSession.h"
#include "application/chatSessions/services/DeleteChatSession.h"
#include "application/chatSessions/services/GetChatHistory.h"
#include "application/chatSessions/services/GetChatSession.h"
#include "application/chatSessions/services/ListChatSessions.h"
#include "application/chatSessions/services/SendChatMessage.h"
#include "application/chatSessions/services/UpdateChatSession.h"

#include "application/users/services/CreateTransaction.h"
#include "application/users/services/DeleteTransaction.h"
#include "application/users/services/GetTransaction.h"
#include "application/users/services/ListTransactions.h"
#include "application/users/services/ListTransactionsByCategory.h"
#include "application/users/services/UpdateTransaction.h"
#include "application/users/services/CreatePlaidLinkToken.h"
#include "application/users/services/LinkPlaidAccount.h"

#include "application/references/pfcCategories/services/GetPfcPrimaryCategory.h"
#include "application/references/pfcCategories/services/ListPfcPrimaryCategories.h"
#include "application/references/pfcCategories/services/GetPfcDetailedCategory.h"
#include "application/references/pfcCategories/services/ListPfcDetailedCategories.h"

#include "application/references/countries/services/GetCountry.h"
#include "application/references/countries/services/ListCountries.h"
#include "application/references/currencies/services/GetCurrency.h"
#include "application/references/currencies/services/ListCurrencies.h"
#include "application/references/employmentStatuses/services/GetEmploymentStatus.h"
#include "application/references/employmentStatuses/services/ListEmploymentStatuses.h"
#include "application/references/languages/services/GetLanguage.h"
#include "application/references/languages/services/ListLanguages.h"
#include "application/references/personas/services/GetPersona.h"
#include "application/references/personas/services/ListPersonas.h"
#include "application/references/messageSenders/services/GetMessageSender.h"
#include "application/references/messageSenders/services/ListMessageSenders.h"
#include "application/references/transactionCategories/services/GetTransactionCategory.h"
#include "application/references/transactionCategories/services/ListTransactionCategories.h"

#include "presentation/controllers/HomeController.h"
#include "presentation/controllers/AuthController.h"
#include "presentation/controllers/UserController.h"
#include "presentation/controllers/UserChatController.h"
#include "presentation/controllers/UserTransactionsController.h"
#include "presentation/controllers/ReferenceController.h"

class Server {
    AppConfig          config_;
    DatabaseConnection db_;

    PostgresUserRepository                userRepo_;
    PostgresChatSessionRepository         chatSessionRepo_;
    PostgresChatMessageRepository         chatMessageRepo_;
    PostgresLlmPersonaRepository          personaRepo_;
    PostgresTransactionRepository         transactionRepo_;
    PostgresTransactionCategoryRepository categoryRepo_;
    PostgresCurrencyRepository            currencyRepo_;
    PostgresCountryRepository             countryRepo_;
    PostgresEmploymentStatusRepository    employmentStatusRepo_;
    PostgresLanguageRepository            languageRepo_;
    PostgresMessageSenderRepository       messageSenderRepo_;
    PostgresPlaidItemRepository           plaidItemRepo_;
    PostgresPfcPrimaryCategoryRepository  pfcPrimaryRepo_;
    PostgresPfcDetailedCategoryRepository pfcDetailedRepo_;

    OpenAIClient        openai_;
    PromptBuilder       promptBuilder_;
    PlaidClient         plaid_;
    GoogleOAuthService  googleOAuth_;

    JwtService     jwt_;
    PasswordHasher hasher_;

    LoginUser                  loginUser_;
    RegisterUser               registerUser_;
    LoginOrRegisterOAuthUser   loginOrRegisterOAuthUser_;

    GetUserProfile     getUserProfile_;
    UpdateUserProfile  updateUserProfile_;
    UpdateUserPassword updateUserPassword_;
    DeleteUser         deleteUser_;

    CreateChatSession  createChatSession_;
    DeleteChatSession  deleteChatSession_;
    GetChatHistory     getChatHistory_;
    GetChatSession     getChatSession_;
    ListChatSessions   listChatSessions_;
    SendChatMessage    sendChatMessage_;
    UpdateChatSession  updateChatSession_;

    CreateTransaction          createTransactions_;
    DeleteTransaction          deleteTransactions_;
    GetTransaction             getTransactions_;
    ListTransactions           listTransactions_;
    ListTransactionsByCategory listTransactionsByCategory_;
    UpdateTransaction          updateTransactions_;
    ImportPlaidTransactions    importPlaidTransactions_;
    CreatePlaidLinkToken       createPlaidLinkToken_;
    LinkPlaidAccount           linkPlaidAccount_;
    GetFinancialInsights       getFinancialInsights_;

    GetPfcPrimaryCategory      getPfcPrimaryCategory_;
    ListPfcPrimaryCategories   listPfcPrimaryCategories_;
    GetPfcDetailedCategory     getPfcDetailedCategory_;
    ListPfcDetailedCategories  listPfcDetailedCategories_;

    GetCountry             getCountry_;
    ListCountries          listCountries_;
    GetCurrency            getCurrency_;
    ListCurrencies         listCurrencies_;
    GetEmploymentStatus    getEmploymentStatus_;
    ListEmploymentStatuses listEmploymentStatuses_;
    GetLanguage            getLanguage_;
    ListLanguages          listLanguages_;
    GetPersona             getLLMPersona_;
    ListPersonas           listLLMPersonas_;
    GetMessageSender       getMessageSender_;
    ListMessageSenders     listMessageSenders_;
    GetTransactionCategory    getTransactionCategory_;
    ListTransactionCategories listTransactionCategories_;

    HomeController             homeController_;
    AuthController             authController_;
    UserController             userController_;
    UserChatController         userChatController_;
    UserTransactionsController userTransactionsController_;
    ReferenceController        referenceController_;

    FilteredCrowLogger crowLogger_;
    RoboDadApp app_;

public:
    Server();
    void run();
};
