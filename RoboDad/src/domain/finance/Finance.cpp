#include "domain/finance/Finance.h"

Finance::Finance(uint32_t id, double amt, const std::string& desc, FinanceEnum t)
    : id(id), amount(amt), description(desc), type(t) {}

Finance::Finance(double amt, const std::string& desc, FinanceEnum t)
    : id(0), amount(amt), description(desc), type(t) {}

uint32_t Finance::getId() const { return id; }
double Finance::getAmount() const { return amount; }
std::string Finance::getDescription() const { return description; }
FinanceEnum Finance::getType() const { return type; }

void Finance::setId(uint32_t newId) { id = newId; }
void Finance::setAmount(double newAmount) { amount = newAmount; }
void Finance::setDescription(const std::string& newDescription) { description = newDescription; }
void Finance::setType(FinanceEnum newType) { type = newType; }
