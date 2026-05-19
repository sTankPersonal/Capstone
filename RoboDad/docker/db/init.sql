-- RoboDad schema
-- employment_status maps to EmploymentStatus enum: 0=Employed 1=Unemployed 2=Student 3=Retired
-- finance type maps to FinanceEnum: 0=Salary 1=Bonus 2=Investment 3=Expense 4=Tax

-- ── Users ─────────────────────────────────────────────────────────────────────
CREATE TABLE IF NOT EXISTS users (
    id                SERIAL      PRIMARY KEY,
    name              VARCHAR(255) NOT NULL,
    age               SMALLINT     NOT NULL CHECK (age BETWEEN 0 AND 150),
    employment_status SMALLINT     NOT NULL CHECK (employment_status BETWEEN 0 AND 3),
    email             VARCHAR(255) NOT NULL UNIQUE,
    password_hash     VARCHAR(255) NOT NULL
);

-- ── Finances ──────────────────────────────────────────────────────────────────
CREATE TABLE IF NOT EXISTS finances (
    id          SERIAL           PRIMARY KEY,
    user_id     INTEGER          NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    amount      DOUBLE PRECISION NOT NULL,
    description TEXT             NOT NULL,
    type        SMALLINT         NOT NULL CHECK (type BETWEEN 0 AND 4)
);

-- ── Budget goals ──────────────────────────────────────────────────────────────
CREATE TABLE IF NOT EXISTS budget_goals (
    id          SERIAL           PRIMARY KEY,
    user_id     INTEGER          NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    end_goal    DOUBLE PRECISION NOT NULL CHECK (end_goal > 0),
    amount      DOUBLE PRECISION NOT NULL CHECK (amount >= 0),
    description TEXT             NOT NULL
);

-- ── Indexes ───────────────────────────────────────────────────────────────────
CREATE INDEX IF NOT EXISTS idx_finances_user_id     ON finances(user_id);
CREATE INDEX IF NOT EXISTS idx_budget_goals_user_id ON budget_goals(user_id);
