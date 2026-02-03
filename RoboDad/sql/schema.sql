-- RoboDad database schema
-- Apply once against an empty database: psql -d <dbname> -f schema.sql

-- ── Reference: Employment Statuses ────────────────────────────────────────────
CREATE TABLE employment_statuses (
    employment_status_id TEXT PRIMARY KEY,
    value                TEXT NOT NULL,
    created_at           DATE NOT NULL DEFAULT CURRENT_DATE
);

INSERT INTO employment_statuses (employment_status_id, value) VALUES
    ('employed',    'Employed'),
    ('unemployed',  'Unemployed'),
    ('student',     'Student'),
    ('retired',     'Retired');

-- ── Reference: Currencies ─────────────────────────────────────────────────────
CREATE TABLE currencies (
    currency_id TEXT PRIMARY KEY,
    value       TEXT NOT NULL,
    created_at  DATE NOT NULL DEFAULT CURRENT_DATE
);

INSERT INTO currencies (currency_id, value) VALUES
    ('USD', 'US Dollar'),
    ('EUR', 'Euro'),
    ('GBP', 'British Pound'),
    ('CAD', 'Canadian Dollar'),
    ('AUD', 'Australian Dollar'),
    ('JPY', 'Japanese Yen'),
    ('CHF', 'Swiss Franc'),
    ('CNY', 'Chinese Yuan'),
    ('INR', 'Indian Rupee'),
    ('MXN', 'Mexican Peso'),
    ('BRL', 'Brazilian Real'),
    ('KRW', 'South Korean Won'),
    ('SGD', 'Singapore Dollar'),
    ('HKD', 'Hong Kong Dollar'),
    ('SEK', 'Swedish Krona');

-- ── Reference: Languages ──────────────────────────────────────────────────────
CREATE TABLE languages (
    language_id TEXT PRIMARY KEY,
    value       TEXT NOT NULL,
    created_at  DATE NOT NULL DEFAULT CURRENT_DATE
);

INSERT INTO languages (language_id, value) VALUES
    ('en', 'English'),
    ('es', 'Spanish'),
    ('fr', 'French'),
    ('de', 'German'),
    ('zh', 'Chinese'),
    ('ar', 'Arabic'),
    ('pt', 'Portuguese'),
    ('ru', 'Russian'),
    ('ja', 'Japanese'),
    ('hi', 'Hindi'),
    ('ko', 'Korean'),
    ('it', 'Italian'),
    ('nl', 'Dutch'),
    ('pl', 'Polish'),
    ('sv', 'Swedish');

-- ── Reference: Countries ──────────────────────────────────────────────────────
CREATE TABLE countries (
    country_id TEXT PRIMARY KEY,
    value      TEXT NOT NULL,
    created_at DATE NOT NULL DEFAULT CURRENT_DATE
);

INSERT INTO countries (country_id, value) VALUES
    ('US', 'United States'),
    ('GB', 'United Kingdom'),
    ('CA', 'Canada'),
    ('AU', 'Australia'),
    ('DE', 'Germany'),
    ('FR', 'France'),
    ('JP', 'Japan'),
    ('CN', 'China'),
    ('IN', 'India'),
    ('MX', 'Mexico'),
    ('BR', 'Brazil'),
    ('IT', 'Italy'),
    ('ES', 'Spain'),
    ('KR', 'South Korea'),
    ('RU', 'Russia'),
    ('NL', 'Netherlands'),
    ('SE', 'Sweden'),
    ('CH', 'Switzerland'),
    ('SG', 'Singapore'),
    ('HK', 'Hong Kong');

-- ── Reference: Transaction Categories ────────────────────────────────────────
CREATE TABLE transaction_categories (
    transaction_category_id TEXT PRIMARY KEY,
    value                   TEXT NOT NULL,
    created_at              DATE NOT NULL DEFAULT CURRENT_DATE
);

INSERT INTO transaction_categories (transaction_category_id, value) VALUES
    ('housing',         'Housing'),
    ('food',            'Food & Dining'),
    ('transportation',  'Transportation'),
    ('utilities',       'Utilities'),
    ('healthcare',      'Healthcare'),
    ('entertainment',   'Entertainment'),
    ('education',       'Education'),
    ('clothing',        'Clothing & Apparel'),
    ('savings',         'Savings'),
    ('income',          'Income'),
    ('investments',     'Investments'),
    ('insurance',       'Insurance'),
    ('personal',        'Personal Care'),
    ('travel',          'Travel'),
    ('subscriptions',   'Subscriptions'),
    ('gifts',           'Gifts & Donations'),
    ('other',           'Other');

-- ── Reference: Message Senders ────────────────────────────────────────────────
CREATE TABLE message_senders (
    message_sender_id TEXT PRIMARY KEY,
    name              TEXT NOT NULL,
    created_at        DATE NOT NULL DEFAULT CURRENT_DATE
);

INSERT INTO message_senders (message_sender_id, name) VALUES
    ('user',      'User'),
    ('assistant', 'Assistant');

-- ── Reference: LLM Personas ───────────────────────────────────────────────────
CREATE TABLE llm_personas (
    llm_persona_id TEXT PRIMARY KEY,
    name           TEXT NOT NULL,
    description    TEXT NOT NULL,
    system_prompt  TEXT NOT NULL,
    created_at     DATE NOT NULL DEFAULT CURRENT_DATE
);

INSERT INTO llm_personas (llm_persona_id, name, description, system_prompt) VALUES
    (
        'robodad',
        'RoboDad',
        'A warm, encouraging father figure who guides you toward financial independence.',
        'You are RoboDad, a caring and knowledgeable AI financial advisor who speaks like a supportive father. Your goal is to help users build healthy financial habits, understand their spending, and work toward their financial goals. You give practical, actionable advice in plain language. You celebrate wins, gently correct mistakes, and always encourage long-term thinking. You never judge the user for past financial decisions — you focus on what they can do today to improve their future.'
    ),
    (
        'budget_coach',
        'Budget Coach',
        'A no-nonsense accountability partner laser-focused on sticking to the budget.',
        'You are Budget Coach, a direct and disciplined financial accountability partner. Your role is to help users track their spending, identify budget overruns, and stay on target with their financial goals. You are honest and to-the-point — if the user is overspending, you say so clearly and suggest specific corrections. You do not sugarcoat bad financial behavior, but you always provide a concrete next step to get back on track.'
    ),
    (
        'investment_mentor',
        'Investment Mentor',
        'A patient teacher who explains investing concepts and helps build long-term wealth.',
        'You are Investment Mentor, a patient and knowledgeable guide focused on long-term wealth building. Your role is to help users understand investment concepts, evaluate their risk tolerance, and develop strategies for growing their savings over time. You explain financial concepts clearly without jargon, use relatable examples, and always tie advice back to the user''s specific financial situation and goals. You emphasize diversification, patience, and compound growth.'
    );

-- ── Users ─────────────────────────────────────────────────────────────────────
CREATE TABLE users (
    user_id              TEXT PRIMARY KEY,
    email                TEXT NOT NULL UNIQUE,
    password_hash        TEXT,
    first_name           TEXT,
    last_name            TEXT,
    date_of_birth        DATE,
    country_id           TEXT REFERENCES countries(country_id),
    currency_id          TEXT REFERENCES currencies(currency_id),
    language_id          TEXT REFERENCES languages(language_id),
    employment_status_id TEXT REFERENCES employment_statuses(employment_status_id),
    created_at           DATE NOT NULL DEFAULT CURRENT_DATE,
    updated_at           DATE NOT NULL DEFAULT CURRENT_DATE
);

-- ── Chat Sessions ─────────────────────────────────────────────────────────────
CREATE TABLE chat_sessions (
    chat_session_id TEXT PRIMARY KEY,
    user_id         TEXT NOT NULL REFERENCES users(user_id) ON DELETE CASCADE,
    llm_persona_id  TEXT NOT NULL REFERENCES llm_personas(llm_persona_id),
    description     TEXT NOT NULL,
    additional_info TEXT,
    created_at      DATE NOT NULL DEFAULT CURRENT_DATE,
    updated_at      DATE NOT NULL DEFAULT CURRENT_DATE
);

-- ── Chat Messages ─────────────────────────────────────────────────────────────
CREATE TABLE chat_messages (
    chat_message_id   TEXT PRIMARY KEY,
    chat_session_id   TEXT NOT NULL REFERENCES chat_sessions(chat_session_id) ON DELETE CASCADE,
    message_sender_id TEXT NOT NULL REFERENCES message_senders(message_sender_id),
    content           TEXT,
    created_at        DATE NOT NULL DEFAULT CURRENT_DATE
);

-- ── Transactions ──────────────────────────────────────────────────────────────
CREATE TABLE transactions (
    transaction_id          TEXT PRIMARY KEY,
    user_id                 TEXT NOT NULL REFERENCES users(user_id) ON DELETE CASCADE,
    transaction_category_id TEXT NOT NULL REFERENCES transaction_categories(transaction_category_id),
    amount                  DOUBLE PRECISION,
    currency_id             TEXT REFERENCES currencies(currency_id),
    description             TEXT,
    transaction_date        DATE NOT NULL DEFAULT CURRENT_DATE,
    created_at              DATE NOT NULL DEFAULT CURRENT_DATE
);

-- ── Indexes ───────────────────────────────────────────────────────────────────
CREATE INDEX idx_chat_sessions_user_id    ON chat_sessions(user_id);
CREATE INDEX idx_chat_messages_session_id ON chat_messages(chat_session_id);
CREATE INDEX idx_transactions_user_id     ON transactions(user_id);
