#include <gtest/gtest.h>
#include "Country.h"
#include "Currency.h"
#include "Language.h"
#include "EmploymentStatus.h"
#include "TransactionCategory.h"
#include "LlmPersona.h"
#include "MessageSender.h"
#include "CountryId.h"
#include "CurrencyId.h"
#include "LanguageId.h"
#include "EmploymentStatusId.h"
#include "TransactionCategoryId.h"
#include "LlmPersonaId.h"
#include "MessageSenderId.h"
#include <chrono>

static std::chrono::year_month_day makeDate(int y, int m, int d) {
    return {std::chrono::year{y}, std::chrono::month{static_cast<unsigned>(m)}, std::chrono::day{static_cast<unsigned>(d)}};
}

// ── Country ───────────────────────────────────────────────────────────────────

TEST(CountryTest, ConstructorPreservesAllFields) {
    auto date = makeDate(2024, 1, 1);
    Country country(CountryId{"CA"}, "Canada", date);
    EXPECT_EQ(country.getId().getId(), "CA");
    EXPECT_EQ(country.getValue(), "Canada");
    EXPECT_EQ(country.getCreatedAt(), date);
}

TEST(CountryTest, DistinctCountriesHaveDistinctIds) {
    auto date = makeDate(2024, 1, 1);
    Country ca(CountryId{"CA"}, "Canada", date);
    Country us(CountryId{"US"}, "United States", date);
    EXPECT_NE(ca.getId().getId(), us.getId().getId());
}

// ── Currency ──────────────────────────────────────────────────────────────────

TEST(CurrencyTest, ConstructorPreservesAllFields) {
    auto date = makeDate(2024, 1, 1);
    Currency currency(CurrencyId{"USD"}, "US Dollar", date);
    EXPECT_EQ(currency.getId().getId(), "USD");
    EXPECT_EQ(currency.getValue(), "US Dollar");
    EXPECT_EQ(currency.getCreatedAt(), date);
}

TEST(CurrencyTest, DistinctCurrenciesHaveDistinctIds) {
    auto date = makeDate(2024, 1, 1);
    Currency usd(CurrencyId{"USD"}, "US Dollar", date);
    Currency cad(CurrencyId{"CAD"}, "Canadian Dollar", date);
    EXPECT_NE(usd.getId().getId(), cad.getId().getId());
}

// ── Language ──────────────────────────────────────────────────────────────────

TEST(LanguageTest, ConstructorPreservesAllFields) {
    auto date = makeDate(2024, 1, 1);
    Language language(LanguageId{"en"}, "English", date);
    EXPECT_EQ(language.getId().getId(), "en");
    EXPECT_EQ(language.getValue(), "English");
    EXPECT_EQ(language.getCreatedAt(), date);
}

TEST(LanguageTest, DistinctLanguagesHaveDistinctIds) {
    auto date = makeDate(2024, 1, 1);
    Language en(LanguageId{"en"}, "English", date);
    Language fr(LanguageId{"fr"}, "French", date);
    EXPECT_NE(en.getId().getId(), fr.getId().getId());
}

// ── EmploymentStatus ──────────────────────────────────────────────────────────

TEST(EmploymentStatusTest, ConstructorPreservesAllFields) {
    auto date = makeDate(2024, 1, 1);
    EmploymentStatus status(EmploymentStatusId{"employed"}, "Employed", date);
    EXPECT_EQ(status.getId().getId(), "employed");
    EXPECT_EQ(status.getValue(), "Employed");
    EXPECT_EQ(status.getCreatedAt(), date);
}

TEST(EmploymentStatusTest, DistinctStatusesHaveDistinctIds) {
    auto date = makeDate(2024, 1, 1);
    EmploymentStatus employed(EmploymentStatusId{"employed"}, "Employed", date);
    EmploymentStatus unemployed(EmploymentStatusId{"unemployed"}, "Unemployed", date);
    EXPECT_NE(employed.getId().getId(), unemployed.getId().getId());
}

// ── TransactionCategory ───────────────────────────────────────────────────────

TEST(TransactionCategoryTest, ConstructorPreservesAllFields) {
    auto date = makeDate(2024, 1, 1);
    TransactionCategory category(TransactionCategoryId{"food"}, "Food & Dining", date);
    EXPECT_EQ(category.getId().getId(), "food");
    EXPECT_EQ(category.getValue(), "Food & Dining");
    EXPECT_EQ(category.getCreatedAt(), date);
}

TEST(TransactionCategoryTest, DistinctCategoriesHaveDistinctIds) {
    auto date = makeDate(2024, 1, 1);
    TransactionCategory food(TransactionCategoryId{"food"}, "Food & Dining", date);
    TransactionCategory transport(TransactionCategoryId{"transport"}, "Transport", date);
    EXPECT_NE(food.getId().getId(), transport.getId().getId());
}

// ── LlmPersona ────────────────────────────────────────────────────────────────

TEST(LlmPersonaTest, ConstructorPreservesAllFields) {
    auto date = makeDate(2024, 2, 14);
    LlmPersona persona(
        LlmPersonaId{"persona-1"},
        "RoboDad",
        "A helpful financial assistant",
        "You are a helpful financial assistant. Be concise and accurate.",
        date
    );
    EXPECT_EQ(persona.getId().getId(), "persona-1");
    EXPECT_EQ(persona.getName(), "RoboDad");
    EXPECT_EQ(persona.getDescription(), "A helpful financial assistant");
    EXPECT_EQ(persona.getSystemPrompt(), "You are a helpful financial assistant. Be concise and accurate.");
    EXPECT_EQ(persona.getCreatedAt(), date);
}

TEST(LlmPersonaTest, DistinctPersonasHaveDistinctIds) {
    auto date = makeDate(2024, 2, 14);
    LlmPersona a(LlmPersonaId{"persona-a"}, "PersonaA", "Desc A", "Prompt A", date);
    LlmPersona b(LlmPersonaId{"persona-b"}, "PersonaB", "Desc B", "Prompt B", date);
    EXPECT_NE(a.getId().getId(), b.getId().getId());
}

// ── MessageSender ─────────────────────────────────────────────────────────────

TEST(MessageSenderTest, ConstructorPreservesAllFields) {
    auto date = makeDate(2024, 3, 20);
    MessageSender sender(MessageSenderId{"sender-1"}, "user", date);
    EXPECT_EQ(sender.getId().getId(), "sender-1");
    EXPECT_EQ(sender.getName(), "user");
    EXPECT_EQ(sender.getCreatedAt(), date);
}

TEST(MessageSenderTest, DistinctSendersHaveDistinctIds) {
    auto date = makeDate(2024, 3, 20);
    MessageSender user(MessageSenderId{"user"}, "user", date);
    MessageSender assistant(MessageSenderId{"assistant"}, "assistant", date);
    EXPECT_NE(user.getId().getId(), assistant.getId().getId());
}
