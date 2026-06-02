#pragma once

#include <crow.h>
#include <optional>
#include <string>
#include "application/transaction/CreateTransaction.h"
#include "application/transaction/DeleteTransaction.h"
#include "application/transaction/GetTransaction.h"
#include "application/transaction/ImportPlaidTransactions.h"
#include "application/transaction/ListTransactions.h"
#include "application/transaction/UpdateTransaction.h"
#include "ITransactionRepository.h"
#include "ITransactionCategoryRepository.h"
#include "ICurrencyRepository.h"
#include "infrastructureServices/apiClient/IPlaidClient.h"
#include "infrastructureServices/security/IJwtService.h"

class ActivityController {
    CreateTransaction       createTransaction_;
    DeleteTransaction       deleteTransaction_;
    GetTransaction          getTransaction_;
    ImportPlaidTransactions importTransactions_;
    ListTransactions        listTransactions_;
    UpdateTransaction       updateTransaction_;
    IJwtService&            jwt_;

    std::optional<std::string> extractUserId(const crow::request& req) const;

public:
    ActivityController(ITransactionRepository&         transactionRepo,
                       ITransactionCategoryRepository& categoryRepo,
                       ICurrencyRepository&            currencyRepo,
                       IPlaidClient&                   plaidClient,
                       IJwtService&                    jwt);

    void registerRoutes(crow::SimpleApp& app);
};
