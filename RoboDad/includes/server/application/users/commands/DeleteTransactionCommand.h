#pragma once
#include "TransactionId.h"

struct DeleteTransactionCommand {
    TransactionId id;

    explicit DeleteTransactionCommand(TransactionId id)
        : id(std::move(id)) {}
};
