//////#include <iostream>
//////#define ASIO_STANDALONE
//////#include "asio.hpp"
//////#include "crow/crow_all.h"
//////
//////int main() {
//////    crow::SimpleApp app;
//////
//////    CROW_ROUTE(app, "/")([]() {
//////        return "<h1>Hello, Crow + ASIO is working!</h1>";
//////        });
//////
//////    std::cout << "Server running on http://127.0.0.1:18080\n";
//////
//////    app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
//////}


#include <windows.h>
#include <winhttp.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>

#include <map>
#include <sstream>
#include <iomanip> // for std::setprecision

#pragma comment(lib, "winhttp.lib")
#include <algorithm>
#include <cctype>

#include <random>

struct Transaction {
    std::string date;
    std::string merchant;
    double amount;
};

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::string escape_json(const std::string& input) {

    std::string output;
    for (char c : input) {
        switch (c) {
        case '\"': output += "\\\""; break;
        case '\\': output += "\\\\"; break;
        case '\n': output += "\\n"; break;
        case '\r': output += "\\r"; break;
        case '\t': output += "\\t"; break;
        default: output += c;
        }
    }
    return output;
}

std::string json_unescape(const std::string& input) {
    std::string output;

    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '\\' && i + 1 < input.size()) {
            char next = input[i + 1];

            if (next == 'n') { output += '\n'; i++; }
            else if (next == 't') { output += '\t'; i++; }
            else if (next == 'r') { output += '\r'; i++; }
            else if (next == '\\') { output += '\\'; i++; }
            else if (next == '"') { output += '"'; i++; }

            // Handle Unicode like \u2019
            else if (next == 'u' && i + 5 < input.size()) {
                std::string hex = input.substr(i + 2, 4);

                int code = std::stoi(hex, nullptr, 16);

                // Convert basic Unicode to UTF-8
                if (code <= 0x7F) {
                    output += static_cast<char>(code);
                }
                else if (code <= 0x7FF) {
                    output += static_cast<char>(0xC0 | (code >> 6));
                    output += static_cast<char>(0x80 | (code & 0x3F));
                }
                else {
                    output += static_cast<char>(0xE0 | (code >> 12));
                    output += static_cast<char>(0x80 | ((code >> 6) & 0x3F));
                    output += static_cast<char>(0x80 | (code & 0x3F));
                }

                i += 5;
            }
            else {
                output += input[i];
            }
        }
        else {
            output += input[i];
        }
    }

    return output;
}

// Reads API key from file
std::string read_api_key(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open API key file\n";
        return "";
    }

    std::string key;
    std::getline(file, key);
    return key;
}

std::string extract_text(const std::string& response) {
    std::cout << "\n==== RAW RESPONSE ====\n" << response << "\n======================\n";
    size_t pos = response.find("\"output_text\"");
    if (pos == std::string::npos) return "";

    pos = response.find("\"text\":", pos);
    if (pos == std::string::npos) return "";

    pos += 7;

    while (pos < response.size() &&
        (response[pos] == ' ' || response[pos] == '\"')) pos++;

    std::string result;
    bool escape = false;

    for (size_t i = pos; i < response.size(); i++) {
        char c = response[i];

        if (escape) {
            if (c == 'n') result += '\n';
            else if (c == 't') result += '\t';
            else if (c == 'r') result += '\r';
            else if (c == '\\') result += '\\';
            else if (c == '"') result += '"';
            escape = false;
        }
        else if (c == '\\') escape = true;
        else if (c == '"') break;
        else result += c;
    }

    return result;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::string api_key = read_api_key("C:/models/key.txt");
    if (api_key.empty()) return 1;

    // Open session ONCE
    HINTERNET hSession = WinHttpOpen(
        L"C++ OpenAI Chat/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0
    );

    if (!hSession) {
        std::cerr << "Failed to open session\n";
        return 1;
    }

    HINTERNET hConnect = WinHttpConnect(
        hSession,
        L"api.openai.com",
        INTERNET_DEFAULT_HTTPS_PORT,
        0
    );

    if (!hConnect) {
        std::cerr << "Failed to connect\n";
        return 1;
    }

    std::cout << "=== Simple Chat (type 'exit' to quit) ===\n";

    while (true) {
        std::string prompt;
        std::cout << "\nYou: ";
        std::getline(std::cin, prompt);

        if (prompt == "exit" || prompt == "quit") {
            break;
        }

        std::string preprompt = "You are a helpful, slightly sarcastic father figure who gives practical life advice. You only know so many things";

  
        // Build JSON to send
        std::string data =
            "{ \"model\": \"gpt-5-nano\", \"max_output_tokens\": 50, \"input\": ["
            "{ \"role\": \"system\", \"content\": \"" + escape_json(preprompt) + "\" },"
            "{ \"role\": \"user\", \"content\": \"" + escape_json(prompt) + "\" }"
            "] }";

        HINTERNET hRequest = WinHttpOpenRequest(
            hConnect,
            L"POST",
            L"/v1/responses",
            NULL,
            WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            WINHTTP_FLAG_SECURE
        );

        if (!hRequest) {
            std::cerr << "Failed to open request\n";
            continue;
        }

        std::wstring headers = L"Content-Type: application/json\r\n";
        headers += L"Authorization: Bearer " +
            std::wstring(api_key.begin(), api_key.end());

        auto start = std::chrono::high_resolution_clock::now();

        if (!WinHttpSendRequest(
            hRequest,
            headers.c_str(),
            headers.length(),
            (LPVOID)data.c_str(),
            data.length(),
            data.length(),
            0)) {
            std::cerr << "Send request failed\n";
            WinHttpCloseHandle(hRequest);
            continue;
        }

        if (!WinHttpReceiveResponse(hRequest, NULL)) {
            std::cerr << "Receive response failed\n";
            WinHttpCloseHandle(hRequest);
            continue;
        }

        // Read response
        DWORD bytesAvailable = 0;
        std::string response;

        while (WinHttpQueryDataAvailable(hRequest, &bytesAvailable) &&
            bytesAvailable > 0) {
            std::vector<char> buffer(bytesAvailable);
            DWORD bytesRead = 0;

            if (WinHttpReadData(hRequest, buffer.data(), bytesAvailable, &bytesRead)) {
                response.append(buffer.data(), bytesRead);
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        std::string answer = json_unescape(extract_text(response));

        std::cout << "Assistant: "
            << (answer.empty() ? "[no parsed output]" : answer)
            << "\n";

        std::cout << "(Time: " << elapsed.count() << "s)\n";

        WinHttpCloseHandle(hRequest);
        Sleep(1500);
    }

    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return 0;
}