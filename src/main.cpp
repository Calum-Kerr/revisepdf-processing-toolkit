#include "crow_all.h"
#include "engine.h"
#include <fstream>
#include <sstream>
int main() {
  crow::SimpleApp app;
  PdfEngine engine;
  int port = 8080;
  if (const char* env_port = std::getenv("PORT")) {
    port = std::stoi(env_port);
  }
  CROW_ROUTE(app, "/").methods("GET"_method)
  ([]() {
    std::ifstream file("frontend/home.html");
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto page = crow::response(buffer.str());
    page.set_header("Content-Type", "text/html; charset=utf-8");
    return page;
  });
  CROW_ROUTE(app, "/app").methods("GET"_method)
  ([]() {
    std::ifstream file("frontend/index.html");
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto page = crow::response(buffer.str());
    page.set_header("Content-Type", "text/html; charset=utf-8");
    return page;
  });
  app.port(port).multithreaded().run();
  return 0;
}