#pragma once
#include "sharedKernel/presentation/IController.h"
#include "presentation/AppType.h"
#include "domain/valueObjects/ids/CountryId.h"
#include "domain/valueObjects/ids/CurrencyId.h"
#include "domain/valueObjects/ids/EmploymentStatusId.h"
#include "domain/valueObjects/ids/LanguageId.h"
#include "domain/valueObjects/ids/LlmPersonaId.h"
#include "domain/valueObjects/ids/MessageSenderId.h"
#include "domain/valueObjects/ids/TransactionCategoryId.h"

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

/*
    GET: /references/countries - Loads the list of countries
    GET: /references/countries/<country_id> - Loads the details of the specified country

    GET: /references/currencies - Loads the list of currencies
    GET: /references/currencies/<currency_id> - Loads the details of the specified currency

    GET: /references/employment-statuses - Loads the list of employment statuses
    GET: /references/employment-statuses/<employment_status_id> - Loads the details of the specified employment status

    GET: /references/languages - Loads the list of languages
    GET: /references/languages/<language_id> - Loads the details of the specified language

    GET: /references/llm-personas - Loads the list of LLM personas
    GET: /references/llm-personas/<llm_persona_id> - Loads the details of the specified LLM persona

    GET: /references/message-senders - Loads the list of message senders
    GET: /references/message-senders/<message_sender_id> - Loads the details of the specified message sender

    GET: /references/transaction-categories - Loads the list of transaction categories
    GET: /references/transaction-categories/<category_id> - Loads the details of the specified transaction category.    
*/

class ReferenceController : public IController<RoboDadApp> {
    GetCountry getCountry_;
    ListCountries listCountries_;
    GetCurrency getCurrency_;
    ListCurrencies listCurrencies_;
    GetEmploymentStatus getEmploymentStatus_;
    ListEmploymentStatuses listEmploymentStatuses_;
    GetLanguage getLanguage_;
    ListLanguages listLanguages_;
    GetPersona getLLMPersona_;
    ListPersonas listLLMPersonas_;
    GetMessageSender getMessageSender_;
    ListMessageSenders listMessageSenders_;
    GetTransactionCategory getTransactionCategory_;
    ListTransactionCategories listTransactionCategories_;
public:
    ReferenceController(const GetCountry& getCountry, const ListCountries& listCountries, const GetCurrency& getCurrency, const ListCurrencies& listCurrencies, const GetEmploymentStatus& getEmploymentStatus, const ListEmploymentStatuses& listEmploymentStatuses, const GetLanguage& getLanguage, const ListLanguages& listLanguages, const GetPersona& getLLMPersona, const ListPersonas& listLLMPersonas, const GetMessageSender& getMessageSender, const ListMessageSenders& listMessageSenders, const GetTransactionCategory& getTransactionCategory, const ListTransactionCategories& listTransactionCategories);

    void registerRoutes(RoboDadApp& app) override;

    crow::response getCountries(const crow::request& req);
    crow::response getCountry(const crow::request& req, CountryId country_id);

    crow::response getCurrencies(const crow::request& req);
    crow::response getCurrency(const crow::request& req, CurrencyId currency_id);

    crow::response getEmploymentStatuses(const crow::request& req);
    crow::response getEmploymentStatus(const crow::request& req, EmploymentStatusId employment_status_id);

    crow::response getLanguages(const crow::request& req);
    crow::response getLanguage(const crow::request& req, LanguageId language_id);

    crow::response getLLMPersonas(const crow::request& req);
    crow::response getLLMPersona(const crow::request& req, LlmPersonaId llm_persona_id);

    crow::response getMessageSenders(const crow::request& req);
    crow::response getMessageSender(const crow::request& req, MessageSenderId message_sender_id);

    crow::response getTransactionCategories(const crow::request& req);
    crow::response getTransactionCategory(const crow::request& req, TransactionCategoryId category_id);
};
