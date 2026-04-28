-- RoboDad schema
-- Matches the domain model: User, Expense, BudgetGoal

CREATE EXTENSION IF NOT EXISTS "pgcrypto";  -- for gen_random_uuid()

-- ── Users ────────────────────────────────────────────────────────────────────
CREATE TABLE IF NOT EXISTS users (
    id                UUID        PRIMARY KEY DEFAULT gen_random_uuid(),
    email             TEXT        NOT NULL UNIQUE,
    password_hash     TEXT        NOT NULL,
    first_name        TEXT        NOT NULL,
    last_name         TEXT        NOT NULL,
    employment_status TEXT        NOT NULL DEFAULT 'employed',
    created_at        TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

-- ── Budget goals ──────────────────────────────────────────────────────────────
CREATE TABLE IF NOT EXISTS budget_goals (
    id         UUID         PRIMARY KEY DEFAULT gen_random_uuid(),
    user_id    UUID         NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    category   TEXT         NOT NULL,
    amount     NUMERIC(12,2) NOT NULL CHECK (amount > 0),
    period     TEXT         NOT NULL DEFAULT 'monthly',  -- monthly | weekly | yearly
    created_at TIMESTAMPTZ  NOT NULL DEFAULT NOW(),
    UNIQUE (user_id, category, period)
);

-- ── Expenses ──────────────────────────────────────────────────────────────────
CREATE TABLE IF NOT EXISTS expenses (
    id          UUID         PRIMARY KEY DEFAULT gen_random_uuid(),
    user_id     UUID         NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    amount      NUMERIC(12,2) NOT NULL CHECK (amount > 0),
    category    TEXT         NOT NULL,
    description TEXT,
    incurred_at TIMESTAMPTZ  NOT NULL DEFAULT NOW(),
    created_at  TIMESTAMPTZ  NOT NULL DEFAULT NOW()
);

-- ── Indexes for common query patterns ────────────────────────────────────────
CREATE INDEX IF NOT EXISTS idx_expenses_user_date
    ON expenses (user_id, incurred_at DESC);

CREATE INDEX IF NOT EXISTS idx_expenses_user_category
    ON expenses (user_id, category);

CREATE INDEX IF NOT EXISTS idx_budget_goals_user
    ON budget_goals (user_id);