#include <iostream>
#define ASIO_STANDALONE
#include "asio.hpp"
#include "crow/crow_all.h"

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        return "<h1>Hello, Crow + ASIO is working!</h1>";
        });

    std::cout << "Server running on http://127.0.0.1:18080\n";

    app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}