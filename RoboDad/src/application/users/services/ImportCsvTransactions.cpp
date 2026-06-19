#include "application/users/services/ImportCsvTransactions.h"
#include "TransactionId.h"
#include "TransactionAmount.h"
#include "TransactionDescription.h"
#include "TransactionCategoryId.h"
#include "CurrencyId.h"
#include "UuidGenerator.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <chrono>
#include <optional>
#include <string>
#include <vector>

namespace {

// ── Low-level CSV tokenising ──────────────────────────────────────────────────

// Split the file into logical, non-blank lines, tolerating both \n and \r\n.
std::vector<std::string> splitLines(const std::string& content) {
    std::vector<std::string> lines;
    std::string cur;
    for (char c : content) {
        if (c == '\n') {
            if (!cur.empty() && cur.back() == '\r') cur.pop_back();
            if (cur.find_first_not_of(" \t") != std::string::npos) lines.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    if (!cur.empty() && cur.back() == '\r') cur.pop_back();
    if (cur.find_first_not_of(" \t") != std::string::npos) lines.push_back(cur);
    return lines;
}

// Pick the delimiter that occurs most often across the sampled lines.
char detectDelimiter(const std::vector<std::string>& lines) {
    const char candidates[] = {'\t', ',', ';', '|'};
    char best = ',';
    std::size_t bestCount = 0;
    for (char cand : candidates) {
        std::size_t count = 0;
        for (std::size_t i = 0; i < lines.size() && i < 20; ++i) {
            count += static_cast<std::size_t>(std::count(lines[i].begin(), lines[i].end(), cand));
        }
        if (count > bestCount) {
            bestCount = count;
            best = cand;
        }
    }
    return best;
}

// Split a single line into fields, honouring double-quoted segments so that a
// quoted value may itself contain the delimiter (and "" escapes a quote).
std::vector<std::string> splitFields(const std::string& line, char delim) {
    std::vector<std::string> fields;
    std::string cur;
    bool inQuotes = false;
    for (std::size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (inQuotes) {
            if (c == '"') {
                if (i + 1 < line.size() && line[i + 1] == '"') { cur += '"'; ++i; }
                else inQuotes = false;
            } else {
                cur += c;
            }
        } else if (c == '"') {
            inQuotes = true;
        } else if (c == delim) {
            fields.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    fields.push_back(cur);
    return fields;
}

std::string trim(const std::string& s) {
    std::size_t b = s.find_first_not_of(" \t");
    if (b == std::string::npos) return "";
    std::size_t e = s.find_last_not_of(" \t");
    return s.substr(b, e - b + 1);
}

// ── Field interpreters ────────────────────────────────────────────────────────

// Parse a monetary amount, rejecting anything that is not purely a number.
// Handles $, thousands separators, leading sign and accounting parentheses.
std::optional<double> parseAmount(const std::string& raw) {
    std::string s = trim(raw);
    if (s.empty()) return std::nullopt;

    bool negative = false;
    std::string digits;
    bool seenDigit = false;
    for (char c : s) {
        if (c == ' ' || c == '$' || c == ',' || c == '+') continue;
        if (c == '(') { negative = true; continue; }
        if (c == ')') continue;
        if (c == '-') {
            // A sign is only valid before any digits; a '-' anywhere else
            // (e.g. an ISO date "2026-06-15") means this is not an amount.
            if (seenDigit) return std::nullopt;
            negative = true;
            continue;
        }
        if (std::isdigit(static_cast<unsigned char>(c))) { digits += c; seenDigit = true; continue; }
        if (c == '.') { digits += c; continue; }
        // Any other character (letters, '/', '#', …) disqualifies the field.
        return std::nullopt;
    }
    if (!seenDigit) return std::nullopt;

    try {
        std::size_t pos = 0;
        double value = std::stod(digits, &pos);
        if (pos != digits.size()) return std::nullopt;
        return negative ? -value : value;
    } catch (...) {
        return std::nullopt;
    }
}

// Parse a date in the common bank formats: M/D/YYYY, D/M/YYYY, YYYY-MM-DD, etc.
std::optional<std::chrono::year_month_day> parseDate(const std::string& raw) {
    std::string s = trim(raw);
    if (s.empty()) return std::nullopt;

    std::vector<int> parts;
    std::string num;
    for (char c : s) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            num += c;
        } else if (c == '/' || c == '-' || c == '.') {
            if (num.empty()) return std::nullopt;
            parts.push_back(std::stoi(num));
            num.clear();
        } else {
            return std::nullopt;  // letters / spaces -> not a bare date
        }
    }
    if (!num.empty()) parts.push_back(std::stoi(num));
    if (parts.size() != 3) return std::nullopt;

    int y, m, d;
    if (parts[0] > 31) {            // YYYY-MM-DD
        y = parts[0]; m = parts[1]; d = parts[2];
    } else {                       // assume month-first (US); swap if impossible
        m = parts[0]; d = parts[1]; y = parts[2];
        if (m > 12 && d <= 12) std::swap(m, d);
    }
    if (y < 100) y += 2000;
    if (m < 1 || m > 12 || d < 1 || d > 31) return std::nullopt;

    std::chrono::year_month_day ymd{
        std::chrono::year{y},
        std::chrono::month{static_cast<unsigned>(m)},
        std::chrono::day{static_cast<unsigned>(d)}
    };
    if (!ymd.ok()) return std::nullopt;
    return ymd;
}

}  // namespace

ImportCsvTransactions::ImportCsvTransactions(ITransactionRepository& transactionRepo)
    : transactionRepo_(transactionRepo) {}

std::vector<Transaction> ImportCsvTransactions::execute(const ImportCsvTransactionsCommand& request) {
    std::vector<Transaction> created;

    const std::vector<std::string> lines = splitLines(request.csvContent);
    if (lines.empty()) return created;

    const char delim = detectDelimiter(lines);

    std::vector<std::vector<std::string>> rows;
    std::size_t columnCount = 0;
    rows.reserve(lines.size());
    for (const std::string& line : lines) {
        std::vector<std::string> fields = splitFields(line, delim);
        columnCount = std::max(columnCount, fields.size());
        rows.push_back(std::move(fields));
    }
    if (columnCount == 0) return created;

    // ── Classify every column by inspecting the data ─────────────────────────
    std::vector<int> dateHits(columnCount, 0);
    std::vector<int> amountHits(columnCount, 0);
    std::vector<int> negativeHits(columnCount, 0);
    std::vector<std::size_t> textLength(columnCount, 0);

    for (const auto& row : rows) {
        for (std::size_t c = 0; c < row.size(); ++c) {
            const std::string& field = row[c];
            if (trim(field).empty()) continue;
            if (parseDate(field)) {
                ++dateHits[c];
            } else if (auto amt = parseAmount(field)) {
                ++amountHits[c];
                if (*amt < 0) ++negativeHits[c];
            } else {
                textLength[c] += trim(field).size();
            }
        }
    }

    // Date column: the one most rows successfully parse as a date.
    int dateCol = -1;
    for (std::size_t c = 0; c < columnCount; ++c) {
        if (dateHits[c] > 0 && (dateCol < 0 || dateHits[c] > dateHits[dateCol])) dateCol = static_cast<int>(c);
    }
    if (dateCol < 0) return created;  // no recognisable dates -> nothing to import

    // Amount columns: numeric columns that are not the date column, left to right.
    std::vector<std::size_t> amountCols;
    for (std::size_t c = 0; c < columnCount; ++c) {
        if (static_cast<int>(c) != dateCol && amountHits[c] > 0) amountCols.push_back(c);
    }
    if (amountCols.empty()) return created;

    // Description column: the most text-heavy non-date, non-amount column.
    int descCol = -1;
    for (std::size_t c = 0; c < columnCount; ++c) {
        if (static_cast<int>(c) == dateCol) continue;
        if (std::find(amountCols.begin(), amountCols.end(), c) != amountCols.end()) continue;
        if (textLength[c] > 0 && (descCol < 0 || textLength[c] > textLength[descCol])) descCol = static_cast<int>(c);
    }

    // Decide which column carries the value we want, based on the requested flow.
    //   * Two or more amount columns -> first is debit/charge, second is credit;
    //     expenses read the debit column, earnings read the credit column.
    //   * A single column -> if it ever holds negatives it is signed, so we keep
    //     only the matching side (negatives for expenses, positives for
    //     earnings); otherwise every row is imported.
    const bool wantMoneyIn = (request.flow == CsvImportFlow::MoneyIn);
    const std::size_t targetCol = (wantMoneyIn && amountCols.size() >= 2)
        ? amountCols[1]
        : amountCols.front();
    const bool singleSignedColumn = (amountCols.size() == 1) && (negativeHits[targetCol] > 0);

    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };

    for (const auto& row : rows) {
        if (targetCol >= row.size() || static_cast<std::size_t>(dateCol) >= row.size()) continue;

        auto date = parseDate(row[dateCol]);
        if (!date) continue;  // header / non-transaction row

        auto amount = parseAmount(row[targetCol]);
        if (!amount) continue;  // no value on the side we care about for this row

        double value = *amount;
        if (singleSignedColumn) {
            if (wantMoneyIn) {
                if (value <= 0) continue;  // a debit / charge, not an earning
            } else {
                if (value >= 0) continue;  // a credit / deposit, not an expense
                value = -value;
            }
        } else {
            value = std::abs(value);
        }
        if (value == 0) continue;

        std::string description = "Imported transaction";
        if (descCol >= 0 && static_cast<std::size_t>(descCol) < row.size()) {
            std::string d = trim(row[descCol]);
            if (!d.empty()) description = d.substr(0, 255);
        }

        Transaction tx(
            TransactionId(UuidGenerator::generate()),
            request.userId,
            request.categoryId,
            TransactionAmount(value, std::nullopt),
            TransactionDescription(description),
            *date,
            today
        );
        created.push_back(transactionRepo_.create(tx));
    }

    return created;
}
