#pragma once
#include "UserId.h"
#include "TransactionCategoryId.h"
#include <string>

// Which side of a bank statement should be turned into transactions:
//   MoneyOut -> debits / charges   (expenses)
//   MoneyIn  -> credits / deposits (earnings)
enum class CsvImportFlow { MoneyOut, MoneyIn };

struct ImportCsvTransactionsCommand {
    UserId userId;
    TransactionCategoryId categoryId;
    std::string csvContent;
    CsvImportFlow flow;

    explicit ImportCsvTransactionsCommand(UserId userId, TransactionCategoryId categoryId, std::string csvContent, CsvImportFlow flow = CsvImportFlow::MoneyOut)
        : userId(std::move(userId)), categoryId(std::move(categoryId)), csvContent(std::move(csvContent)), flow(flow) {}
};
