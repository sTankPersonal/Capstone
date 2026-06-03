#include "presentation/controllers/ReferenceController.h"
#include "application/references/countries/dtos/CountryDto.h"
#include "application/references/currencies/dtos/CurrencyDto.h"
#include "application/references/employmentStatuses/dtos/EmploymentStatusDto.h"
#include "application/references/languages/dtos/LanguageDto.h"
#include "application/references/personas/dtos/PersonaDto.h"
#include "application/references/messageSenders/dtos/MessageSenderDto.h"
#include "application/references/transactionCategories/dtos/TransactionCategoryDto.h"

#include "application/references/countries/queries/GetCountryQuery.h"
#include "application/references/countries/queries/ListCountriesQuery.h"
#include "application/references/currencies/queries/GetCurrencyQuery.h"
#include "application/references/currencies/queries/ListCurrenciesQuery.h"
#include "application/references/employmentStatuses/queries/GetEmploymentStatusQuery.h"
#include "application/references/employmentStatuses/queries/ListEmploymentStatusesQuery.h"
#include "application/references/languages/queries/GetLanguageQuery.h"
#include "application/references/languages/queries/ListLanguagesQuery.h"
#include "application/references/messageSenders/queries/GetMessageSenderQuery.h"
#include "application/references/messageSenders/queries/ListMessageSendersQuery.h"
#include "application/references/personas/queries/GetPersonaQuery.h"
#include "application/references/personas/queries/ListPersonasQuery.h"
#include "application/references/transactionCategories/queries/GetTransactionCategoryQuery.h"
#include "application/references/transactionCategories/queries/ListTransactionCategoriesQuery.h"

ReferenceController::ReferenceController(const GetCountry& getCountry, const ListCountries& listCountries, const GetCurrency& getCurrency, const ListCurrencies& listCurrencies, const GetEmploymentStatus& getEmploymentStatus, const ListEmploymentStatuses& listEmploymentStatuses, const GetLanguage& getLanguage, const ListLanguages& listLanguages, const GetPersona& getLLMPersona, const ListPersonas& listLLMPersonas, const GetMessageSender& getMessageSender, const ListMessageSenders& listMessageSenders, const GetTransactionCategory& getTransactionCategory, const ListTransactionCategories& listTransactionCategories)
    : getCountry_(getCountry), listCountries_(listCountries), getCurrency_(getCurrency), listCurrencies_(listCurrencies), getEmploymentStatus_(getEmploymentStatus), listEmploymentStatuses_(listEmploymentStatuses), getLanguage_(getLanguage), listLanguages_(listLanguages), getLLMPersona_(getLLMPersona), listLLMPersonas_(listLLMPersonas), getMessageSender_(getMessageSender), listMessageSenders_(listMessageSenders), getTransactionCategory_(getTransactionCategory), listTransactionCategories_(listTransactionCategories) {}

void ReferenceController::registerRoutes(RoboDadApp& app) {
    CROW_ROUTE(app, "/references/countries")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req){
            return getCountries(req);
        });
    CROW_ROUTE(app, "/references/countries/<string>")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req, const std::string& countryId){
            return getCountry(req, CountryId(countryId));
        });
    CROW_ROUTE(app, "/references/currencies")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req){
            return getCurrencies(req);
        });
    CROW_ROUTE(app, "/references/currencies/<string>")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req, const std::string& currencyId){
            return getCurrency(req, CurrencyId(currencyId));
        });
    CROW_ROUTE(app, "/references/employment-statuses")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req){
            return getEmploymentStatuses(req);
        });
    CROW_ROUTE(app, "/references/employment-statuses/<string>")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req, const std::string& employmentStatusId){
            return getEmploymentStatus(req, EmploymentStatusId(employmentStatusId));
        });
    CROW_ROUTE(app, "/references/languages")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req){
            return getLanguages(req);
        });
    CROW_ROUTE(app, "/references/languages/<string>")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req, const std::string& languageId){
            return getLanguage(req, LanguageId(languageId));
        });
    CROW_ROUTE(app, "/references/llm-personas")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req){
            return getLLMPersonas(req);
        });
    CROW_ROUTE(app, "/references/llm-personas/<string>")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req, const std::string& llmPersonaId){
            return getLLMPersona(req, LlmPersonaId(llmPersonaId));
        });
    CROW_ROUTE(app, "/references/message-senders")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req){
            return getMessageSenders(req);
        });
    CROW_ROUTE(app, "/references/message-senders/<string>")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req, const std::string& messageSenderId){
            return getMessageSender(req, MessageSenderId(messageSenderId));
        });
    CROW_ROUTE(app, "/references/transaction-categories")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req){
            return getTransactionCategories(req);
        });
    CROW_ROUTE(app, "/references/transaction-categories/<string>")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req, const std::string& transactionCategoryId){
            return getTransactionCategory(req, TransactionCategoryId(transactionCategoryId));
        });
}

crow::response ReferenceController::getCountries(const crow::request& req) {
    std::vector<CountryDto> countries = listCountries_.execute(ListCountriesQuery());
    crow::json::wvalue::list countryList;
    for (const CountryDto& country : countries) {
        countryList.push_back(static_cast<crow::json::wvalue>(country));
    }
    crow::json::wvalue responseBody;
    responseBody["countries"] = std::move(countryList);
    return crow::response(200, responseBody);
}

crow::response ReferenceController::getCountry(const crow::request& req, CountryId country_id) {
    std::optional<CountryDto> countryOpt = getCountry_.execute(GetCountryQuery(country_id));
    if (!countryOpt) {
        return crow::response(404, "Country not found");
    }
    return crow::response(200, static_cast<crow::json::wvalue>(*countryOpt));
}

crow::response ReferenceController::getCurrencies(const crow::request& req) {
    std::vector<CurrencyDto> currencies = listCurrencies_.execute(ListCurrenciesQuery());
    crow::json::wvalue::list currencyList;
    for (const CurrencyDto& currency : currencies) {
        currencyList.push_back(static_cast<crow::json::wvalue>(currency));
    }
    crow::json::wvalue responseBody;
    responseBody["currencies"] = std::move(currencyList);
    return crow::response(200, responseBody);
}

crow::response ReferenceController::getCurrency(const crow::request& req, CurrencyId currency_id) {
    std::optional<CurrencyDto> currencyOpt = getCurrency_.execute(GetCurrencyQuery(currency_id));
    if (!currencyOpt) {
        return crow::response(404, "Currency not found");
    }
    return crow::response(200, static_cast<crow::json::wvalue>(*currencyOpt));
}

crow::response ReferenceController::getEmploymentStatuses(const crow::request& req) {
    std::vector<EmploymentStatusDto> statuses = listEmploymentStatuses_.execute(ListEmploymentStatusesQuery());
    crow::json::wvalue::list statusList;
    for (const EmploymentStatusDto& status : statuses) {
        statusList.push_back(static_cast<crow::json::wvalue>(status));
    }
    crow::json::wvalue responseBody;
    responseBody["employmentStatuses"] = std::move(statusList);
    return crow::response(200, responseBody);
}

crow::response ReferenceController::getEmploymentStatus(const crow::request& req, EmploymentStatusId employment_status_id) {
    std::optional<EmploymentStatusDto> statusOpt = getEmploymentStatus_.execute(GetEmploymentStatusQuery(employment_status_id));
    if (!statusOpt) {
        return crow::response(404, "Employment status not found");
    }
    return crow::response(200, static_cast<crow::json::wvalue>(*statusOpt));
}

crow::response ReferenceController::getLanguages(const crow::request& req) {
    std::vector<LanguageDto> languages = listLanguages_.execute(ListLanguagesQuery());
    crow::json::wvalue::list languageList;
    for (const LanguageDto& language : languages) {
        languageList.push_back(static_cast<crow::json::wvalue>(language));
    }
    crow::json::wvalue responseBody;
    responseBody["languages"] = std::move(languageList);
    return crow::response(200, responseBody);
}

crow::response ReferenceController::getLanguage(const crow::request& req, LanguageId language_id) {
    std::optional<LanguageDto> languageOpt = getLanguage_.execute(GetLanguageQuery(language_id));
    if (!languageOpt) {
        return crow::response(404, "Language not found");
    }
    return crow::response(200, static_cast<crow::json::wvalue>(*languageOpt));
}

crow::response ReferenceController::getLLMPersonas(const crow::request& req) {
    std::vector<PersonaDto> personas = listLLMPersonas_.execute(ListPersonasQuery());
    crow::json::wvalue::list personaList;
    for (const PersonaDto& persona : personas) {
        personaList.push_back(static_cast<crow::json::wvalue>(persona));
    }
    crow::json::wvalue responseBody;
    responseBody["llmPersonas"] = std::move(personaList);
    return crow::response(200, responseBody);
}

crow::response ReferenceController::getLLMPersona(const crow::request& req, LlmPersonaId llm_persona_id) {
    std::optional<PersonaDto> personaOpt = getLLMPersona_.execute(GetPersonaQuery(llm_persona_id));
    if (!personaOpt) {
        return crow::response(404, "LLM persona not found");
    }
    return crow::response(200, static_cast<crow::json::wvalue>(*personaOpt));
}

crow::response ReferenceController::getMessageSenders(const crow::request& req) {
    std::vector<MessageSenderDto> senders = listMessageSenders_.execute(ListMessageSendersQuery());
    crow::json::wvalue::list senderList;
    for (const MessageSenderDto& sender : senders) {
        senderList.push_back(static_cast<crow::json::wvalue>(sender));
    }
    crow::json::wvalue responseBody;
    responseBody["messageSenders"] = std::move(senderList);
    return crow::response(200, responseBody);
}

crow::response ReferenceController::getMessageSender(const crow::request& req, MessageSenderId message_sender_id) {
    std::optional<MessageSenderDto> senderOpt = getMessageSender_.execute(GetMessageSenderQuery(message_sender_id));
    if (!senderOpt) {
        return crow::response(404, "Message sender not found");
    }
    return crow::response(200, static_cast<crow::json::wvalue>(*senderOpt));
}

crow::response ReferenceController::getTransactionCategories(const crow::request& req) {
    std::vector<TransactionCategoryDto> categories = listTransactionCategories_.execute(ListTransactionCategoriesQuery());
    crow::json::wvalue::list categoryList;
    for (const TransactionCategoryDto& category : categories) {
        categoryList.push_back(static_cast<crow::json::wvalue>(category));
    }
    crow::json::wvalue responseBody;
    responseBody["transactionCategories"] = std::move(categoryList);
    return crow::response(200, responseBody);
}

crow::response ReferenceController::getTransactionCategory(const crow::request& req, TransactionCategoryId category_id) {
    std::optional<TransactionCategoryDto> categoryOpt = getTransactionCategory_.execute(GetTransactionCategoryQuery(category_id));
    if (!categoryOpt) {
        return crow::response(404, "Transaction category not found");
    }
    return crow::response(200, static_cast<crow::json::wvalue>(*categoryOpt));
}
