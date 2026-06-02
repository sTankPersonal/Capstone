#include <gtest/gtest.h>
#include <cstdlib>
#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"
#include "ChatMessage.h"
#include "ChatMessageId.h"
#include "ChatSessionId.h"
#include "MessageSenderId.h"
#include "ChatMessageContent.h"
#include <chrono>

static std::chrono::year_month_day today() {
    return std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())};
}

class OpenAIClientTest : public ::testing::Test {
protected:
    std::string apiKey_;
    std::string model_;

    void SetUp() override {
        const char* key = std::getenv("OPENAI_API_KEY");
        apiKey_ = key ? key : "";
        model_  = "gpt-4o-mini";

        if (apiKey_.empty())
            GTEST_SKIP() << "OPENAI_API_KEY not set; skipping integration tests.";
    }
};

TEST_F(OpenAIClientTest, GenerateReturnsNonEmptyResponse) {
    OpenAIClient client(apiKey_, model_);
    const std::string response = client.generate(
        "You are a helpful assistant.",
        {},
        "Say hello in one word."
    );
    EXPECT_FALSE(response.empty());
}

TEST_F(OpenAIClientTest, GenerateWithChatHistoryDoesNotCrash) {
    OpenAIClient client(apiKey_, model_);
    ChatMessage prior(
        ChatMessageId{"msg-1"}, ChatSessionId{"session-1"},
        MessageSenderId{"user"},
        ChatMessageContent(std::optional<std::string>{"My name is Alice."}),
        today()
    );
    EXPECT_NO_THROW({
        const std::string response = client.generate(
            "You are a helpful assistant.",
            {prior},
            "Repeat my name."
        );
        EXPECT_FALSE(response.empty());
    });
}

TEST_F(OpenAIClientTest, GenerateWithEmptySystemPromptDoesNotCrash) {
    OpenAIClient client(apiKey_, model_);
    EXPECT_NO_THROW({
        const std::string response = client.generate("", {}, "What is 2 + 2?");
        EXPECT_FALSE(response.empty());
    });
}
