#include "server/Server.h"
#include "crow/mustache.h"

Server::Server()
    : config_(AppConfig::fromEnv())
    , db_(DatabaseConnection::fromEnv())
    , userRepo_(db_)
    , chatSessionRepo_(db_)
    , chatMessageRepo_(db_)
    , personaRepo_(db_)
    , transactionRepo_(db_)
    , categoryRepo_(db_)
    , currencyRepo_(db_)
    , countryRepo_(db_)
    , employmentStatusRepo_(db_)
    , languageRepo_(db_)
    , messageSenderRepo_(db_)
    , openai_(config_.openAiApiKey(), config_.openAiModel())
    , promptBuilder_()
    , plaid_(config_.plaidClientId(), config_.plaidSecret())
    , googleOAuth_(config_.googleClientId(), config_.googleClientSecret(), "http://localhost:18080/auth/google/callback")
    , jwt_(config_.jwtSecret())
    , hasher_()
    , loginUser_(userRepo_, hasher_)
    , registerUser_(userRepo_, hasher_)
    , loginOrRegisterOAuthUser_(userRepo_, hasher_)
    , getUserProfile_(userRepo_)
    , updateUserProfile_(userRepo_)
    , updateUserPassword_(userRepo_, hasher_)
    , deleteUser_(userRepo_)
    , createChatSession_(chatSessionRepo_)
    , deleteChatSession_(chatSessionRepo_)
    , getChatHistory_(chatMessageRepo_)
    , getChatSession_(chatSessionRepo_)
    , listChatSessions_(chatSessionRepo_)
    , sendChatMessage_(chatSessionRepo_, chatMessageRepo_, personaRepo_, transactionRepo_, openai_, promptBuilder_)
    , updateChatSession_(chatSessionRepo_)
    , createTransactions_(transactionRepo_)
    , deleteTransactions_(transactionRepo_)
    , getTransactions_(transactionRepo_)
    , listTransactions_(transactionRepo_)
    , listTransactionsByCategory_(transactionRepo_)
    , updateTransactions_(transactionRepo_)
    , importPlaidTransactions_(transactionRepo_, categoryRepo_, currencyRepo_, plaid_)
    , getCountry_(countryRepo_)
    , listCountries_(countryRepo_)
    , getCurrency_(currencyRepo_)
    , listCurrencies_(currencyRepo_)
    , getEmploymentStatus_(employmentStatusRepo_)
    , listEmploymentStatuses_(employmentStatusRepo_)
    , getLanguage_(languageRepo_)
    , listLanguages_(languageRepo_)
    , getLLMPersona_(personaRepo_)
    , listLLMPersonas_(personaRepo_)
    , getMessageSender_(messageSenderRepo_)
    , listMessageSenders_(messageSenderRepo_)
    , getTransactionCategory_(categoryRepo_)
    , listTransactionCategories_(categoryRepo_)
    , homeController_()
    , authController_(loginUser_, registerUser_, loginOrRegisterOAuthUser_, googleOAuth_, jwt_)
    , userController_(getUserProfile_, updateUserProfile_, updateUserPassword_, deleteUser_)
    , userChatController_(createChatSession_, updateChatSession_, deleteChatSession_, getChatSession_, listChatSessions_, getChatHistory_, sendChatMessage_, getUserProfile_)
    , userTransactionsController_(createTransactions_, deleteTransactions_, getTransactions_, getUserProfile_, listTransactions_, listTransactionsByCategory_, updateTransactions_, importPlaidTransactions_)
    , referenceController_(getCountry_, listCountries_, getCurrency_, listCurrencies_, getEmploymentStatus_, listEmploymentStatuses_, getLanguage_, listLanguages_, getLLMPersona_, listLLMPersonas_, getMessageSender_, listMessageSenders_, getTransactionCategory_, listTransactionCategories_)
{}

void Server::run() {
    crow::logger::setHandler(&crowLogger_);
    crow::mustache::set_global_base("public");

    app_.get_middleware<AuthMiddleware>().init(jwt_);
    app_.get_middleware<AdminMiddleware>().init(jwt_);

    homeController_.registerRoutes(app_);
    authController_.registerRoutes(app_);
    userController_.registerRoutes(app_);
    userChatController_.registerRoutes(app_);
    userTransactionsController_.registerRoutes(app_);
    referenceController_.registerRoutes(app_);

    app_.bindaddr("0.0.0.0").port(18080).multithreaded().run();
}
