#pragma once
#include "TransactionCategoryId.h"

struct GetTransactionCategoryQuery {
    TransactionCategoryId id;

    explicit GetTransactionCategoryQuery(TransactionCategoryId id)
        : id(std::move(id)) {}
};
