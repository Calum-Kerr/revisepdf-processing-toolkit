#include "crow_all.h"
#include "engine.h"
#include <fstream>
#include <sstream>
#include <functional>
void add_security_headers(crow::response& res) {
  res.set_header("Content-Security-Policy", "default-src 'self'; script-src 'self'; style-src 'self' 'unsafe-inline'; img-src 'self' data:; font-src 'self'; connect-src 'self'; frame-ancestors 'none'; base-uri 'self'; form-action 'self'");
  res.set_header("X-Content-Type-Options", "nosniff");
  res.set_header("X-Frame-Options", "DENY");
  res.set_header("X-XSS-Protection", "1; mode=block");
  res.set_header("Strict-Transport-Security", "max-age=31536000; includeSubDomains; preload");
  res.set_header("Referrer-Policy", "no-referrer");
  res.set_header("Permissions-Policy", "geolocation=(), microphone=(), camera=()");
}
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
    add_security_headers(page);
    return page;
  });
  CROW_ROUTE(app, "/features").methods("GET"_method)
  ([]() {
    std::ifstream file("frontend/features.html");
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto page = crow::response(buffer.str());
    page.set_header("Content-Type", "text/html; charset=utf-8");
    add_security_headers(page);
    return page;
  });
  CROW_ROUTE(app, "/app").methods("GET"_method)
  ([]() {
    std::ifstream file("frontend/index.html");
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto page = crow::response(buffer.str());
    page.set_header("Content-Type", "text/html; charset=utf-8");
    add_security_headers(page);
    return page;
  });
  CROW_ROUTE(app, "/app.js").methods("GET"_method)
  ([]() {
    std::ifstream file("frontend/app.js");
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto page = crow::response(buffer.str());
    page.set_header("Content-Type", "application/javascript");
    add_security_headers(page);
    return page;
  });
  CROW_ROUTE(app, "/manifest.json").methods("GET"_method)
  ([]() {
    std::ifstream file("frontend/manifest.json");
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto page = crow::response(buffer.str());
    page.set_header("Content-Type", "application/json");
    add_security_headers(page);
    return page;
  });
  CROW_ROUTE(app, "/service-worker.js").methods("GET"_method)
  ([]() {
    std::ifstream file("frontend/service-worker.js");
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto page = crow::response(buffer.str());
    page.set_header("Content-Type", "application/javascript");
    add_security_headers(page);
    return page;
  });
  CROW_ROUTE(app, "/robots.txt").methods("GET"_method)
  ([]() {
    std::ifstream file("frontend/robots.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto page = crow::response(buffer.str());
    page.set_header("Content-Type", "text/plain");
    add_security_headers(page);
    return page;
  });
  CROW_ROUTE(app, "/sitemap.xml").methods("GET"_method)
  ([]() {
    std::ifstream file("frontend/sitemap.xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto page = crow::response(buffer.str());
    page.set_header("Content-Type", "application/xml");
    add_security_headers(page);
    return page;
  });
  CROW_ROUTE(app, "/api/convert").methods("POST"_method)
  ([&engine](const crow::request& req) {
    auto result = crow::response(R"({"mode":"api","operation":"convert","status":"ok"})");
    result.set_header("Content-Type", "application/json");
    add_security_headers(result);
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