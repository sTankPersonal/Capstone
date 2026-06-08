#pragma once

#include "IMessageSenderRepository.h"
#include "DatabaseConnection.h"
#include <optional>
#include <string>
#include <vector>

class PostgresMessageSenderRepository : public IMessageSenderRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresMessageSenderRepository(DatabaseConnection& db);

    MessageSender create(const MessageSender& entity) override;
    std::optional<MessageSender> findById(MessageSenderId id) override;
    std::vector<MessageSender> findAll() override;
    bool update(const MessageSender& entity) override;
    bool remove(MessageSenderId id) override;

    std::optional<MessageSender> findByName(const std::string& name);
};
