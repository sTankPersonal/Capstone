-- RoboDad database schema
-- Apply once against an empty database: psql -d <dbname> -f schema.sql

-- ── Users ─────────────────────────────────────────────────────────────────────
-- employment_status maps to EmploymentStatus enum:
--   0=Employed  1=Unemployed  2=Student  3=Retired
CREATE TABLE users (
    id                SERIAL       PRIMARY KEY,
    name              VARCHAR(255) NOT NULL,
    age               SMALLINT     NOT NULL CHECK (age BETWEEN 0 AND 150),
    employment_status SMALLINT     NOT NULL CHECK (employment_status BETWEEN 0 AND 3)
);

-- ── Finances ──────────────────────────────────────────────────────────────────
-- type maps to FinanceEnum:
--   0=Salary  1=Bonus  2=Investment  3=Expense  4=Tax
CREATE TABLE finances (
    id          SERIAL           PRIMARY KEY,
    user_id     INTEGER          NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    amount      DOUBLE PRECISION NOT NULL,
    description TEXT             NOT NULL,
    type        SMALLINT         NOT NULL CHECK (type BETWEEN 0 AND 4)
);

-- ── Budget goals ──────────────────────────────────────────────────────────────
CREATE TABLE budget_goals (
    id          SERIAL           PRIMARY KEY,
    user_id     INTEGER          NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    end_goal    DOUBLE PRECISION NOT NULL CHECK (end_goal > 0),
    amount      DOUBLE PRECISION NOT NULL CHECK (amount >= 0),
    description TEXT             NOT NULL
);

-- ── LLM system instruction sets ───────────────────────────────────────────────
CREATE TABLE system_instructions (
    id                SERIAL PRIMARY KEY,
    personality       TEXT   NOT NULL,
    safety_guidelines TEXT   NOT NULL,
    goal              TEXT   NOT NULL
);

-- ── LLM prompt sessions ───────────────────────────────────────────────────────
-- system_instructions_id is SET NULL on delete so prompts are preserved
-- when a system_instructions row is removed.
CREATE TABLE prompts (
    id                     SERIAL   PRIMARY KEY,
    user_id                INTEGER  REFERENCES users(id) ON DELETE SET NULL,
    system_instructions_id INTEGER  NOT NULL REFERENCES system_instructions(id) ON DELETE RESTRICT,
    user_message           TEXT     NOT NULL
);

-- ── Chat history ──────────────────────────────────────────────────────────────
-- timestamp is Unix epoch milliseconds stored as a 64-bit integer.
CREATE TABLE chat_history (
    id           SERIAL  PRIMARY KEY,
    prompt_id    INTEGER NOT NULL REFERENCES prompts(id) ON DELETE CASCADE,
    timestamp    BIGINT  NOT NULL,
    user_message TEXT    NOT NULL,
    llm_response TEXT    NOT NULL
);

-- ── Indexes ───────────────────────────────────────────────────────────────────
CREATE INDEX idx_finances_user_id      ON finances(user_id);
CREATE INDEX idx_budget_goals_user_id  ON budget_goals(user_id);
CREATE INDEX idx_prompts_user_id       ON prompts(user_id);
CREATE INDEX idx_chat_history_prompt   ON chat_history(prompt_id);
CREATE INDEX idx_chat_history_ts       ON chat_history(timestamp);
