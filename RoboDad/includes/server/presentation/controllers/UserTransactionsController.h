#pragma once
#include "sharedKernel/presentation/IController.h"
#include "domain/valueObjects/ids/TransactionId.h"
#include "domain/valueObjects/ids/TransactionCategoryId.h"
#include "domain/valueObjects/ids/UserId.h"

#include "application/users/services/CreateTransaction.h"
#include "application/users/services/DeleteTransaction.h"
#include "application/users/services/GetTransaction.h"
#include "application/users/services/GetUserProfile.h"
#include "application/users/services/ListTransactions.h"
#include "application/users/services/ListTransactionsByCategory.h"
#include "application/users/services/UpdateTransaction.h"
#include "application/users/services/ImportPlaidTransactions.h"
#include "application/users/services/CreatePlaidLinkToken.h"
#include "application/users/services/LinkPlaidAccount.h"

#include "presentation/AppType.h"

class UserTransactionsController : public IController<RoboDadApp> {
    CreateTransaction          createTransactions_;
    DeleteTransaction          deleteTransactions_;
    GetTransaction             getTransactions_;
    GetUserProfile             getUserProfile_;
    ListTransactions           listTransactions_;
    ListTransactionsByCategory listTransactionsByCategory_;
    UpdateTransaction          updateTransactions_;
    ImportPlaidTransactions    importPlaidTransactions_;
    CreatePlaidLinkToken       createPlaidLinkToken_;
    LinkPlaidAccount           linkPlaidAccount_;
public:
    UserTransactionsController(const CreateTransaction& createTransactions,
                               const DeleteTransaction& deleteTransactions,
                               const GetTransaction& getTransactions,
                               const GetUserProfile& getUserProfile,
                               const ListTransactions& listTransactions,
                               const ListTransactionsByCategory& listTransactionsByCategory,
                               const UpdateTransaction& updateTransactions,
                               const ImportPlaidTransactions& importPlaidTransactions,
                               const CreatePlaidLinkToken& createPlaidLinkToken,
                               const LinkPlaidAccount& linkPlaidAccount);

    void registerRoutes(RoboDadApp& app) override;

    crow::response getTransactions(const crow::request& req, UserId user_id);
    crow::response getTransactionDetails(const crow::request& req, UserId user_id, TransactionId transaction_id);
    crow::response getTransactionsByCategory(const crow::request& req, UserId user_id, TransactionCategoryId category_id);

    crow::response getNewTransactionPage(const crow::request& req, UserId user_id, TransactionCategoryId category_id);
    crow::response getEditTransactionPage(const crow::request& req, UserId user_id, TransactionId transaction_id);
    crow::response getDeleteTransactionPage(const crow::request& req, UserId user_id, TransactionId transaction_id);

    crow::response postNewTransaction(const crow::request& req, UserId user_id, TransactionCategoryId category_id);
    crow::response postEditTransaction(const crow::request& req, UserId user_id, TransactionId transaction_id);
    crow::response postDeleteTransaction(const crow::request& req, UserId user_id, TransactionId transaction_id);

    crow::response getPlaidImportPage(const crow::request& req, UserId user_id);
    crow::response getPlaidLinkToken(const crow::request& req, UserId user_id);
    crow::response postPlaidExchange(const crow::request& req, UserId user_id);
};
