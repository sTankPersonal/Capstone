#include <gtest/gtest.h>
#include "ChatSession.h"
#include "ChatMessage.h"
#include "ChatSessionDescription.h"
#include "ChatMessageContent.h"
#include "ChatSessionId.h"
#include "ChatMessageId.h"
#include "UserId.h"
#include "LlmPersonaId.h"
#include "MessageSenderId.h"
#include <chrono>

static std::chrono::year_month_day makeDate(int y, int m, int d) {
    return {std::chrono::year{y}, std::chrono::month{static_cast<unsigned>(m)}, std::chrono::day{static_cast<unsigned>(d)}};
}

// ── ChatSessionDescription ────────────────────────────────────────────────────

TEST(ChatSessionDescriptionTest, ConstructorPreservesDescription) {
    ChatSessionDescription desc("Budget planning", std::nullopt);
    EXPECT_EQ(desc.getDescription(), "Budget planning");
}

TEST(ChatSessionDescriptionTest, ConstructorPreservesAdditionalInfo) {
    ChatSessionDescription desc("Budget planning", std::optional<std::string>{"Monthly review"});
    ASSERT_TRUE(desc.getAdditionalInfo().has_value());
    EXPECT_EQ(desc.getAdditionalInfo().value(), "Monthly review");
}

TEST(ChatSessionDescriptionTest, AdditionalInfoEmptyWhenNotProvided) {
    ChatSessionDescription desc("General chat", std::nullopt);
    EXPECT_FALSE(desc.getAdditionalInfo().has_value());
}

TEST(ChatSessionDescriptionTest, SettersUpdateFields) {
    ChatSessionDescription desc("Old description", std::nullopt);
    desc.setDescription("New description");
    desc.setAdditionalInfo(std::optional<std::string>{"Some info"});
    EXPECT_EQ(desc.getDescription(), "New description");
    ASSERT_TRUE(desc.getAdditionalInfo().has_value());
    EXPECT_EQ(desc.getAdditionalInfo().value(), "Some info");
}

// ── ChatMessageContent ────────────────────────────────────────────────────────

TEST(ChatMessageContentTest, ConstructorPreservesContent) {
    ChatMessageContent content(std::optional<std::string>{"Hello, world!"});
    ASSERT_TRUE(content.getContent().has_value());
    EXPECT_EQ(content.getContent().value(), "Hello, world!");
}

TEST(ChatMessageContentTest, ContentEmptyWhenNotProvided) {
    ChatMessageContent content(std::nullopt);
    EXPECT_FALSE(content.getContent().has_value());
}

// ── ChatSession ───────────────────────────────────────────────────────────────

TEST(ChatSessionTest, ConstructorPreservesAllIds) {
    ChatSessionDescription desc("Planning", std::nullopt);
    auto date = makeDate(2024, 3, 10);
    ChatSession session(
        ChatSessionId{"session-1"}, UserId{"user-1"}, LlmPersonaId{"persona-1"},
        desc, date, date
    );
    EXPECT_EQ(session.getId().getId(), "session-1");
    EXPECT_EQ(session.getUserId().getId(), "user-1");
    EXPECT_EQ(session.getPersonaId().getId(), "persona-1");
}

TEST(ChatSessionTest, ConstructorPreservesSessionDescription) {
    ChatSessionDescription desc("Expense tracking", std::optional<std::string>{"Monthly"});
    auto date = makeDate(2024, 3, 10);
    ChatSession session(
        ChatSessionId{"session-2"}, UserId{"user-2"}, LlmPersonaId{"persona-2"},
        desc, date, date
    );
    EXPECT_EQ(session.getSessionDescription().getDescription(), "Expense tracking");
    ASSERT_TRUE(session.getSessionDescription().getAdditionalInfo().has_value());
    EXPECT_EQ(session.getSessionDescription().getAdditionalInfo().value(), "Monthly");
}

TEST(ChatSessionTest, ConstructorPreservesTimestamps) {
    ChatSessionDescription desc("Test", std::nullopt);
    auto created = makeDate(2024, 1, 1);
    auto updated = makeDate(2024, 6, 1);
    ChatSession session(
        ChatSessionId{"session-3"}, UserId{"user-3"}, LlmPersonaId{"persona-3"},
        desc, created, updated
    );
    EXPECT_EQ(session.getCreatedAt(), created);
    EXPECT_EQ(session.getUpdatedAt(), updated);
}

// ── ChatMessage ───────────────────────────────────────────────────────────────

TEST(ChatMessageTest, ConstructorPreservesAllFields) {
    ChatMessageContent content(std::optional<std::string>{"How do I save money?"});
    auto date = makeDate(2024, 4, 5);
    ChatMessage message(
        ChatMessageId{"msg-1"}, ChatSessionId{"session-1"},
        MessageSenderId{"sender-1"}, content, date
    );
    EXPECT_EQ(message.getId().getId(), "msg-1");
    EXPECT_EQ(message.getChatSessionId().getId(), "session-1");
    EXPECT_EQ(message.getMessageSenderId().getId(), "sender-1");
    ASSERT_TRUE(message.getContent().getContent().has_value());
    EXPECT_EQ(message.getContent().getContent().value(), "How do I save money?");
    EXPECT_EQ(message.getCreatedAt(), date);
}

TEST(ChatMessageTest, ContentCanBeEmpty) {
    ChatMessageContent content(std::nullopt);
    auto date = makeDate(2024, 4, 6);
    ChatMessage message(
        ChatMessageId{"msg-2"}, ChatSessionId{"session-2"},
        MessageSenderId{"sender-2"}, content, date
    );
    EXPECT_FALSE(message.getContent().getContent().has_value());
}

TEST(ChatMessageTest, DistinctMessagesHaveDistinctIds) {
    ChatMessageContent content(std::nullopt);
    auto date = makeDate(2024, 4, 7);
    ChatMessage a(ChatMessageId{"msg-a"}, ChatSessionId{"session-1"}, MessageSenderId{"sender-1"}, content, date);
    ChatMessage b(ChatMessageId{"msg-b"}, ChatSessionId{"session-1"}, MessageSenderId{"sender-1"}, content, date);
    EXPECT_NE(a.getId().getId(), b.getId().getId());
}
