#pragma once
#include "IUseCase.h"
#include "ImportCsvTransactionsCommand.h"
#include "ITransactionRepository.h"
#include "Transactions.h"
#include <vector>

// Imports transactions from a raw CSV/TSV export of any bank.
//
// The parser is heuristic so it can cope with arbitrary column layouts:
//   * the field delimiter (tab, comma, semicolon or pipe) is auto-detected,
//   * the date / amount / description columns are identified by inspecting the
//     data rather than by relying on a fixed header,
//   * separate debit and credit columns are supported. The command's flow
//     decides which side is kept: money-out (debit / charge) rows for expenses,
//     money-in (credit / deposit) rows for earnings.
class ImportCsvTransactions : public IUseCase<ImportCsvTransactionsCommand, std::vector<Transaction>> {
    ITransactionRepository& transactionRepo_;
public:
    explicit ImportCsvTransactions(ITransactionRepository& transactionRepo);

    std::vector<Transaction> execute(const ImportCsvTransactionsCommand& request) override;
};
