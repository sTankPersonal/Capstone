#include "EmploymentStatusId.h"

EmploymentStatusId::EmploymentStatusId(const std::string& id) : id_(id) {}

const std::string& EmploymentStatusId::getId() const noexcept { return id_; }
