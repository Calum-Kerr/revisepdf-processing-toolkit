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
  CROW_ROUTE(app, "/api/convert").methods("POST"_method)
  ([&engine](const crow::request& req) {
    auto result = crow::response(R"({"mode":"api","operation":"convert","status":"ok"})");
    result.set_header("Content-Type", "application/json");
    return result;
  });
  CROW_ROUTE(app, "/api/compress").methods("POST"_method)
  ([&engine](const crow::request& req) {
    auto result = crow::response(R"({"mode":"api","operation":"compress","status":"ok"})");
    result.set_header("Content-Type", "application/json");
    return result;
  });
  CROW_ROUTE(app, "/api/merge").methods("POST"_method)
  ([&engine](const crow::request& req) {
    auto result = crow::response(R"({"mode":"api","operation":"merge","status":"ok"})");
    result.set_header("Content-Type", "application/json");
    return result;
  });
  CROW_ROUTE(app, "/api/split").methods("POST"_method)
  ([&engine](const crow::request& req) {
    auto result = crow::response(R"({"mode":"api","operation":"split","status":"ok"})");
    result.set_header("Content-Type", "application/json");
    return result;
  });
  CROW_ROUTE(app, "/api/rotate").methods("POST"_method)
  ([&engine](const crow::request& req) {
    auto result = crow::response(R"({"mode":"api","operation":"rotate","status":"ok"})");
    result.set_header("Content-Type", "application/json");
    return result;
  });
  app.port(port).multithreaded().run();
  return 0;
}