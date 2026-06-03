#pragma once
#include "UserId.h"
#include "TransactionCategoryId.h"

struct ListTransactionsByCategoryQuery {
    UserId userId;
    TransactionCategoryId categoryId;

    explicit ListTransactionsByCategoryQuery(UserId userId, TransactionCategoryId categoryId)
        : userId(std::move(userId)), categoryId(std::move(categoryId)) {}
};
