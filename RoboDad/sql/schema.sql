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
    ('earnings', 'Earnings'),
    ('expenses', 'Expenses');

-- ── Reference: PFC Primary Categories ────────────────────────────────────────
CREATE TABLE pfc_primary_categories (
    pfc_primary_category_id TEXT PRIMARY KEY,
    value                   TEXT NOT NULL,
    created_at              DATE NOT NULL DEFAULT CURRENT_DATE
);

INSERT INTO pfc_primary_categories (pfc_primary_category_id, value) VALUES
    ('BANK_FEES',               'Bank Fees'),
    ('ENTERTAINMENT',           'Entertainment'),
    ('FOOD_AND_DRINK',          'Food and Drink'),
    ('GENERAL_MERCHANDISE',     'General Merchandise'),
    ('GENERAL_SERVICES',        'General Services'),
    ('GOVERNMENT_AND_NON_PROFIT','Government and Non-Profit'),
    ('HOME_IMPROVEMENT',        'Home Improvement'),
    ('INCOME',                  'Income'),
    ('LOAN_DISBURSEMENTS',      'Loan Disbursements'),
    ('LOAN_PAYMENTS',           'Loan Payments'),
    ('MEDICAL',                 'Medical'),
    ('OTHER',                   'Other'),
    ('PERSONAL_CARE',           'Personal Care'),
    ('RENT_AND_UTILITIES',      'Rent and Utilities'),
    ('TRANSFER_IN',             'Transfer In'),
    ('TRANSFER_OUT',            'Transfer Out'),
    ('TRANSPORTATION',          'Transportation'),
    ('TRAVEL',                  'Travel');

-- ── Reference: PFC Detailed Categories ───────────────────────────────────────
CREATE TABLE pfc_detailed_categories (
    pfc_detailed_category_id TEXT PRIMARY KEY,
    pfc_primary_category_id  TEXT NOT NULL REFERENCES pfc_primary_categories(pfc_primary_category_id),
    value                    TEXT NOT NULL,
    created_at               DATE NOT NULL DEFAULT CURRENT_DATE
);

INSERT INTO pfc_detailed_categories (pfc_detailed_category_id, pfc_primary_category_id, value) VALUES
    -- INCOME
    ('INCOME_CHILD_SUPPORT',           'INCOME', 'Child Support'),
    ('INCOME_CONTRACTOR',              'INCOME', 'Contractor'),
    ('INCOME_DIVIDENDS',               'INCOME', 'Dividends'),
    ('INCOME_GIG_ECONOMY',             'INCOME', 'Gig Economy'),
    ('INCOME_INTEREST_EARNED',         'INCOME', 'Interest Earned'),
    ('INCOME_LONG_TERM_DISABILITY',    'INCOME', 'Long Term Disability'),
    ('INCOME_MILITARY',                'INCOME', 'Military'),
    ('INCOME_RENTAL',                  'INCOME', 'Rental'),
    ('INCOME_RETIREMENT_PENSION',      'INCOME', 'Retirement Pension'),
    ('INCOME_SALARY',                  'INCOME', 'Salary'),
    ('INCOME_TAX_REFUND',              'INCOME', 'Tax Refund'),
    ('INCOME_UNEMPLOYMENT',            'INCOME', 'Unemployment'),
    ('INCOME_OTHER',                   'INCOME', 'Other Income'),
    -- LOAN_DISBURSEMENTS
    ('LOAN_DISBURSEMENTS_AUTO',                  'LOAN_DISBURSEMENTS', 'Auto Loan Disbursement'),
    ('LOAN_DISBURSEMENTS_CASH_ADVANCES',         'LOAN_DISBURSEMENTS', 'Cash Advances'),
    ('LOAN_DISBURSEMENTS_EWA',                   'LOAN_DISBURSEMENTS', 'Earned Wage Access'),
    ('LOAN_DISBURSEMENTS_MORTGAGE',              'LOAN_DISBURSEMENTS', 'Mortgage Disbursement'),
    ('LOAN_DISBURSEMENTS_PERSONAL',              'LOAN_DISBURSEMENTS', 'Personal Loan Disbursement'),
    ('LOAN_DISBURSEMENTS_STUDENT',               'LOAN_DISBURSEMENTS', 'Student Loan Disbursement'),
    ('LOAN_DISBURSEMENTS_OTHER_DISBURSEMENT',    'LOAN_DISBURSEMENTS', 'Other Disbursement'),
    -- LOAN_PAYMENTS
    ('LOAN_PAYMENTS_BNPL',                       'LOAN_PAYMENTS', 'Buy Now Pay Later'),
    ('LOAN_PAYMENTS_CAR_PAYMENT',                'LOAN_PAYMENTS', 'Car Payment'),
    ('LOAN_PAYMENTS_CASH_ADVANCES',              'LOAN_PAYMENTS', 'Cash Advances'),
    ('LOAN_PAYMENTS_CREDIT_CARD_PAYMENT',        'LOAN_PAYMENTS', 'Credit Card Payment'),
    ('LOAN_PAYMENTS_EWA',                        'LOAN_PAYMENTS', 'Earned Wage Access'),
    ('LOAN_PAYMENTS_MORTGAGE_PAYMENT',           'LOAN_PAYMENTS', 'Mortgage Payment'),
    ('LOAN_PAYMENTS_PERSONAL_LOAN_PAYMENT',      'LOAN_PAYMENTS', 'Personal Loan Payment'),
    ('LOAN_PAYMENTS_STUDENT_LOAN_PAYMENT',       'LOAN_PAYMENTS', 'Student Loan Payment'),
    ('LOAN_PAYMENTS_OTHER_PAYMENT',              'LOAN_PAYMENTS', 'Other Payment'),
    -- TRANSFER_IN
    ('TRANSFER_IN_ACCOUNT_TRANSFER',             'TRANSFER_IN', 'Account Transfer In'),
    ('TRANSFER_IN_DEPOSIT',                      'TRANSFER_IN', 'Deposit'),
    ('TRANSFER_IN_INVESTMENT_AND_RETIREMENT_FUNDS','TRANSFER_IN', 'Investment and Retirement Funds In'),
    ('TRANSFER_IN_SAVINGS',                      'TRANSFER_IN', 'Savings Transfer In'),
    ('TRANSFER_IN_TRANSFER_IN_FROM_APPS',        'TRANSFER_IN', 'Transfer In from Apps'),
    ('TRANSFER_IN_WIRE',                         'TRANSFER_IN', 'Wire Transfer In'),
    ('TRANSFER_IN_OTHER_TRANSFER_IN',            'TRANSFER_IN', 'Other Transfer In'),
    -- TRANSFER_OUT
    ('TRANSFER_OUT_ACCOUNT_TRANSFER',            'TRANSFER_OUT', 'Account Transfer Out'),
    ('TRANSFER_OUT_CRYPTO',                      'TRANSFER_OUT', 'Crypto'),
    ('TRANSFER_OUT_INVESTMENT_AND_RETIREMENT_FUNDS','TRANSFER_OUT', 'Investment and Retirement Funds Out'),
    ('TRANSFER_OUT_SAVINGS',                     'TRANSFER_OUT', 'Savings Transfer Out'),
    ('TRANSFER_OUT_TRANSFER_OUT_FROM_APPS',      'TRANSFER_OUT', 'Transfer Out from Apps'),
    ('TRANSFER_OUT_WIRE',                        'TRANSFER_OUT', 'Wire Transfer Out'),
    ('TRANSFER_OUT_WITHDRAWAL',                  'TRANSFER_OUT', 'Withdrawal'),
    ('TRANSFER_OUT_OTHER_TRANSFER_OUT',          'TRANSFER_OUT', 'Other Transfer Out'),
    -- BANK_FEES
    ('BANK_FEES_ATM_FEES',                       'BANK_FEES', 'ATM Fees'),
    ('BANK_FEES_INSUFFICIENT_FUNDS',             'BANK_FEES', 'Insufficient Funds'),
    ('BANK_FEES_INTEREST_CHARGE',                'BANK_FEES', 'Interest Charge'),
    ('BANK_FEES_FOREIGN_TRANSACTION_FEES',       'BANK_FEES', 'Foreign Transaction Fees'),
    ('BANK_FEES_OVERDRAFT_FEES',                 'BANK_FEES', 'Overdraft Fees'),
    ('BANK_FEES_LATE_FEES',                      'BANK_FEES', 'Late Fees'),
    ('BANK_FEES_CASH_ADVANCE',                   'BANK_FEES', 'Cash Advance Fee'),
    ('BANK_FEES_OTHER_BANK_FEES',                'BANK_FEES', 'Other Bank Fees'),
    -- ENTERTAINMENT
    ('ENTERTAINMENT_CASINOS_AND_GAMBLING',                       'ENTERTAINMENT', 'Casinos and Gambling'),
    ('ENTERTAINMENT_MUSIC_AND_AUDIO',                            'ENTERTAINMENT', 'Music and Audio'),
    ('ENTERTAINMENT_SPORTING_EVENTS_AMUSEMENT_PARKS_AND_MUSEUMS','ENTERTAINMENT', 'Sporting Events, Amusement Parks and Museums'),
    ('ENTERTAINMENT_TV_AND_MOVIES',                              'ENTERTAINMENT', 'TV and Movies'),
    ('ENTERTAINMENT_VIDEO_GAMES',                                'ENTERTAINMENT', 'Video Games'),
    ('ENTERTAINMENT_OTHER_ENTERTAINMENT',                        'ENTERTAINMENT', 'Other Entertainment'),
    -- FOOD_AND_DRINK
    ('FOOD_AND_DRINK_BEER_WINE_AND_LIQUOR',      'FOOD_AND_DRINK', 'Beer, Wine and Liquor'),
    ('FOOD_AND_DRINK_COFFEE',                    'FOOD_AND_DRINK', 'Coffee'),
    ('FOOD_AND_DRINK_FAST_FOOD',                 'FOOD_AND_DRINK', 'Fast Food'),
    ('FOOD_AND_DRINK_GROCERIES',                 'FOOD_AND_DRINK', 'Groceries'),
    ('FOOD_AND_DRINK_RESTAURANT',                'FOOD_AND_DRINK', 'Restaurant'),
    ('FOOD_AND_DRINK_VENDING_MACHINES',          'FOOD_AND_DRINK', 'Vending Machines'),
    ('FOOD_AND_DRINK_OTHER_FOOD_AND_DRINK',      'FOOD_AND_DRINK', 'Other Food and Drink'),
    -- GENERAL_MERCHANDISE
    ('GENERAL_MERCHANDISE_BOOKSTORES_AND_NEWSSTANDS', 'GENERAL_MERCHANDISE', 'Bookstores and Newsstands'),
    ('GENERAL_MERCHANDISE_CLOTHING_AND_ACCESSORIES',  'GENERAL_MERCHANDISE', 'Clothing and Accessories'),
    ('GENERAL_MERCHANDISE_CONVENIENCE_STORES',        'GENERAL_MERCHANDISE', 'Convenience Stores'),
    ('GENERAL_MERCHANDISE_DEPARTMENT_STORES',         'GENERAL_MERCHANDISE', 'Department Stores'),
    ('GENERAL_MERCHANDISE_DISCOUNT_STORES',           'GENERAL_MERCHANDISE', 'Discount Stores'),
    ('GENERAL_MERCHANDISE_ELECTRONICS',               'GENERAL_MERCHANDISE', 'Electronics'),
    ('GENERAL_MERCHANDISE_GIFTS_AND_NOVELTIES',       'GENERAL_MERCHANDISE', 'Gifts and Novelties'),
    ('GENERAL_MERCHANDISE_OFFICE_SUPPLIES',           'GENERAL_MERCHANDISE', 'Office Supplies'),
    ('GENERAL_MERCHANDISE_ONLINE_MARKETPLACES',       'GENERAL_MERCHANDISE', 'Online Marketplaces'),
    ('GENERAL_MERCHANDISE_PET_SUPPLIES',              'GENERAL_MERCHANDISE', 'Pet Supplies'),
    ('GENERAL_MERCHANDISE_SPORTING_GOODS',            'GENERAL_MERCHANDISE', 'Sporting Goods'),
    ('GENERAL_MERCHANDISE_SUPERSTORES',               'GENERAL_MERCHANDISE', 'Superstores'),
    ('GENERAL_MERCHANDISE_TOBACCO_AND_VAPE',          'GENERAL_MERCHANDISE', 'Tobacco and Vape'),
    ('GENERAL_MERCHANDISE_OTHER_GENERAL_MERCHANDISE', 'GENERAL_MERCHANDISE', 'Other General Merchandise'),
    -- HOME_IMPROVEMENT
    ('HOME_IMPROVEMENT_FURNITURE',               'HOME_IMPROVEMENT', 'Furniture'),
    ('HOME_IMPROVEMENT_HARDWARE',                'HOME_IMPROVEMENT', 'Hardware'),
    ('HOME_IMPROVEMENT_REPAIR_AND_MAINTENANCE',  'HOME_IMPROVEMENT', 'Repair and Maintenance'),
    ('HOME_IMPROVEMENT_SECURITY',                'HOME_IMPROVEMENT', 'Security'),
    ('HOME_IMPROVEMENT_OTHER_HOME_IMPROVEMENT',  'HOME_IMPROVEMENT', 'Other Home Improvement'),
    -- MEDICAL
    ('MEDICAL_DENTAL_CARE',                      'MEDICAL', 'Dental Care'),
    ('MEDICAL_EYE_CARE',                         'MEDICAL', 'Eye Care'),
    ('MEDICAL_NURSING_CARE',                     'MEDICAL', 'Nursing Care'),
    ('MEDICAL_PHARMACIES_AND_SUPPLEMENTS',       'MEDICAL', 'Pharmacies and Supplements'),
    ('MEDICAL_PRIMARY_CARE',                     'MEDICAL', 'Primary Care'),
    ('MEDICAL_VETERINARY_SERVICES',              'MEDICAL', 'Veterinary Services'),
    ('MEDICAL_OTHER_MEDICAL',                    'MEDICAL', 'Other Medical'),
    -- PERSONAL_CARE
    ('PERSONAL_CARE_GYMS_AND_FITNESS_CENTERS',   'PERSONAL_CARE', 'Gyms and Fitness Centers'),
    ('PERSONAL_CARE_HAIR_AND_BEAUTY',            'PERSONAL_CARE', 'Hair and Beauty'),
    ('PERSONAL_CARE_LAUNDRY_AND_DRY_CLEANING',   'PERSONAL_CARE', 'Laundry and Dry Cleaning'),
    ('PERSONAL_CARE_OTHER_PERSONAL_CARE',        'PERSONAL_CARE', 'Other Personal Care'),
    -- GENERAL_SERVICES
    ('GENERAL_SERVICES_ACCOUNTING_AND_FINANCIAL_PLANNING', 'GENERAL_SERVICES', 'Accounting and Financial Planning'),
    ('GENERAL_SERVICES_AUTOMOTIVE',              'GENERAL_SERVICES', 'Automotive'),
    ('GENERAL_SERVICES_CHILDCARE',               'GENERAL_SERVICES', 'Childcare'),
    ('GENERAL_SERVICES_CONSULTING_AND_LEGAL',    'GENERAL_SERVICES', 'Consulting and Legal'),
    ('GENERAL_SERVICES_EDUCATION',               'GENERAL_SERVICES', 'Education'),
    ('GENERAL_SERVICES_INSURANCE',               'GENERAL_SERVICES', 'Insurance'),
    ('GENERAL_SERVICES_POSTAGE_AND_SHIPPING',    'GENERAL_SERVICES', 'Postage and Shipping'),
    ('GENERAL_SERVICES_STORAGE',                 'GENERAL_SERVICES', 'Storage'),
    ('GENERAL_SERVICES_OTHER_GENERAL_SERVICES',  'GENERAL_SERVICES', 'Other General Services'),
    -- GOVERNMENT_AND_NON_PROFIT
    ('GOVERNMENT_AND_NON_PROFIT_DONATIONS',                         'GOVERNMENT_AND_NON_PROFIT', 'Donations'),
    ('GOVERNMENT_AND_NON_PROFIT_GOVERNMENT_DEPARTMENTS_AND_AGENCIES','GOVERNMENT_AND_NON_PROFIT', 'Government Departments and Agencies'),
    ('GOVERNMENT_AND_NON_PROFIT_TAX_PAYMENT',                       'GOVERNMENT_AND_NON_PROFIT', 'Tax Payment'),
    ('GOVERNMENT_AND_NON_PROFIT_OTHER_GOVERNMENT_AND_NON_PROFIT',   'GOVERNMENT_AND_NON_PROFIT', 'Other Government and Non-Profit'),
    -- TRANSPORTATION
    ('TRANSPORTATION_BIKES_AND_SCOOTERS',        'TRANSPORTATION', 'Bikes and Scooters'),
    ('TRANSPORTATION_GAS',                       'TRANSPORTATION', 'Gas'),
    ('TRANSPORTATION_PARKING',                   'TRANSPORTATION', 'Parking'),
    ('TRANSPORTATION_PUBLIC_TRANSIT',            'TRANSPORTATION', 'Public Transit'),
    ('TRANSPORTATION_TAXIS_AND_RIDE_SHARES',     'TRANSPORTATION', 'Taxis and Ride Shares'),
    ('TRANSPORTATION_TOLLS',                     'TRANSPORTATION', 'Tolls'),
    ('TRANSPORTATION_OTHER_TRANSPORTATION',      'TRANSPORTATION', 'Other Transportation'),
    -- TRAVEL
    ('TRAVEL_FLIGHTS',                           'TRAVEL', 'Flights'),
    ('TRAVEL_LODGING',                           'TRAVEL', 'Lodging'),
    ('TRAVEL_RENTAL_CARS',                       'TRAVEL', 'Rental Cars'),
    ('TRAVEL_OTHER_TRAVEL',                      'TRAVEL', 'Other Travel'),
    -- RENT_AND_UTILITIES
    ('RENT_AND_UTILITIES_GAS_AND_ELECTRICITY',   'RENT_AND_UTILITIES', 'Gas and Electricity'),
    ('RENT_AND_UTILITIES_INTERNET_AND_CABLE',    'RENT_AND_UTILITIES', 'Internet and Cable'),
    ('RENT_AND_UTILITIES_RENT',                  'RENT_AND_UTILITIES', 'Rent'),
    ('RENT_AND_UTILITIES_SEWAGE_AND_WASTE_MANAGEMENT','RENT_AND_UTILITIES', 'Sewage and Waste Management'),
    ('RENT_AND_UTILITIES_TELEPHONE',             'RENT_AND_UTILITIES', 'Telephone'),
    ('RENT_AND_UTILITIES_WATER',                 'RENT_AND_UTILITIES', 'Water'),
    ('RENT_AND_UTILITIES_OTHER_UTILITIES',       'RENT_AND_UTILITIES', 'Other Utilities'),
    -- OTHER
    ('OTHER_OTHER',                              'OTHER', 'Other');

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
    transaction_id           TEXT PRIMARY KEY,
    user_id                  TEXT NOT NULL REFERENCES users(user_id) ON DELETE CASCADE,
    transaction_category_id  TEXT NOT NULL REFERENCES transaction_categories(transaction_category_id),
    pfc_detailed_category_id TEXT REFERENCES pfc_detailed_categories(pfc_detailed_category_id),
    amount                   DOUBLE PRECISION,
    currency_id              TEXT REFERENCES currencies(currency_id),
    description              TEXT,
    transaction_date         DATE NOT NULL DEFAULT CURRENT_DATE,
    created_at               DATE NOT NULL DEFAULT CURRENT_DATE,
    plaid_transaction_id     TEXT UNIQUE
);

-- ── Plaid Items ───────────────────────────────────────────────────────────────
CREATE TABLE plaid_items (
    plaid_item_id TEXT PRIMARY KEY,
    user_id       TEXT NOT NULL REFERENCES users(user_id) ON DELETE CASCADE,
    access_token  TEXT NOT NULL,
    sync_cursor   TEXT NOT NULL DEFAULT '',
    created_at    DATE NOT NULL DEFAULT CURRENT_DATE
);

-- ── Indexes ───────────────────────────────────────────────────────────────────
CREATE INDEX idx_chat_sessions_user_id    ON chat_sessions(user_id);
CREATE INDEX idx_chat_messages_session_id ON chat_messages(chat_session_id);
CREATE INDEX idx_transactions_user_id     ON transactions(user_id);
CREATE INDEX idx_plaid_items_user_id      ON plaid_items(user_id);
