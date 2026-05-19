#pragma once
#include "domain/IEntity.h"
#include "domain/finance/FinanceEnum.h"
#include <cstdint>
#include <string>

class Finance : public IEntity<uint32_t> {
    uint32_t id;
    double amount;
    std::string description;
    FinanceEnum type;

public:
    Finance(uint32_t id, double amt, const std::string& desc, FinanceEnum t);
    Finance(double amt, const std::string& desc, FinanceEnum t);

    uint32_t getId() const override;
    double getAmount() const;
    std::string getDescription() const;
    FinanceEnum getType() const;

    void setId(uint32_t newId);
    void setAmount(double newAmount);
    void setDescription(const std::string& newDescription);
    void setType(FinanceEnum newType);
};
