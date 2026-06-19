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
#include "application/users/services/ImportCsvTransactions.h"

#include "presentation/AppType.h"

/*
    GET: /user/transactions - Loads the user transactions page
    GET: /user/transactions/<transaction_id> - Loads the transaction details page for the specified transaction
    GET: /user/transactions/<category_id> - Loads the user transactions page filtered by category
        
    GET: /user/transactions/<category_id>/new - Loads the new transaction page for the specified category
        - TransactionAmount
            - Amount
            - Currency_id
        - TransactionDescription
            - Description
        - TransactionDate
    GET: /user/transactions/<transaction_id>/edit - Loads the transaction edit page for the specified transaction
    GET: /user/transactions/<transaction_id>/delete - Loads the transaction deletion confirmation page for the specified transaction

    POST: /user/transactions/<category_id>/new - Submits the new transaction form for the specified category
    POST: /user/transactions/<transaction_id>/edit - Submits the transaction edit form
    POST: /user/transactions/<transaction_id>/delete - Submits the transaction deletion confirmation form

    GET: /user/transactions/import/plaid - Loads the Plaid transaction import page
        - PlaidAccessToken
    POST: /user/transactions/import/plaid - Submits the Plaid transaction import form

    POST: /user/transactions/category/<category_id>/import/csv - Imports transactions from
        an uploaded CSV/TSV bank export. The raw file contents are sent as the request
        body; debit rows are imported for the "expenses" category and credit rows for
        "earnings". The response is JSON: { "imported": <count> }.

    POST: /user/transactions/category/<category_id>/delete - Bulk-deletes the transactions
        whose ids are passed as a comma-separated "ids" form field. Only transactions owned
        by the requesting user are removed. The response is JSON: { "deleted": <count> }.
*/

class UserTransactionsController : public IController<RoboDadApp> {
    CreateTransaction createTransactions_;
    DeleteTransaction deleteTransactions_;
    GetTransaction getTransactions_;
    GetUserProfile getUserProfile_;
    ListTransactions listTransactions_;
    ListTransactionsByCategory listTransactionsByCategory_;
    UpdateTransaction updateTransactions_;
    ImportPlaidTransactions importPlaidTransactions_;
    ImportCsvTransactions importCsvTransactions_;
public:
    UserTransactionsController(const CreateTransaction& createTransactions, const DeleteTransaction& deleteTransactions, const GetTransaction& getTransactions, const GetUserProfile& getUserProfile, const ListTransactions& listTransactions, const ListTransactionsByCategory& listTransactionsByCategory, const UpdateTransaction& updateTransactions, const ImportPlaidTransactions& importPlaidTransactions, const ImportCsvTransactions& importCsvTransactions);
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
    crow::response postPlaidImport(const crow::request& req, UserId user_id);

    crow::response postCsvImport(const crow::request& req, UserId user_id, TransactionCategoryId category_id);
    crow::response postBulkDelete(const crow::request& req, UserId user_id, TransactionCategoryId category_id);
};