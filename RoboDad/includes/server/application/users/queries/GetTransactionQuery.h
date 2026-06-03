#pragma once
#include "TransactionId.h"

struct GetTransactionQuery {
    TransactionId id;

    explicit GetTransactionQuery(TransactionId id)
        : id(std::move(id)) {}
};
