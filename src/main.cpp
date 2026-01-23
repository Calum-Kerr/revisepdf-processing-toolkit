#include "crow.h"
#include "engine.h"
#include <fstream>
#include <sstream>
#include <functional>
#include <nlohmann/json.hpp>
#include <ctime>
#include <cstdio>
using json = nlohmann::json;
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
  CROW_ROUTE(app, "/algorithms.js").methods("GET"_method)
  ([]() {
    std::ifstream file("frontend/algorithms.js", std::ios::binary);
    if (!file.good()) {
      auto page = crow::response("");
      page.code = 404;
      return page;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto page = crow::response(buffer.str());
    page.set_header("Content-Type", "application/javascript; charset=utf-8");
    add_security_headers(page);
    return page;
  });
  CROW_ROUTE(app, "/algorithms.wasm").methods("GET"_method)
  ([]() {
    std::ifstream file("frontend/algorithms.wasm", std::ios::binary);
    if (!file.good()) {
      auto page = crow::response("");
      page.code = 404;
      return page;
    }
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    auto page = crow::response(std::string(buffer.begin(), buffer.end()));
    page.set_header("Content-Type", "application/wasm");
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
  CROW_ROUTE(app, "/favicon.ico").methods("GET"_method)
  ([]() {
    auto page = crow::response("");
    page.set_header("Content-Type", "image/x-icon");
    page.code = 204;
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
    json response;
    if (req.body.empty()) {
      response = {{"mode", "api"}, {"operation", "convert"}, {"status", "error"}, {"message", "no file provided"}};
    } else {
      std::string input_file = "/tmp/input_" + std::to_string(std::time(nullptr)) + ".pdf";
      std::string output_file = "/tmp/output_" + std::to_string(std::time(nullptr)) + ".jpg";
      std::ofstream out(input_file, std::ios::binary);
      out.write(req.body.c_str(), req.body.size());
      out.close();
      auto op = engine.pdf_to_jpg(input_file, output_file);
      std::ifstream in(output_file, std::ios::binary);
      if (in.good()) {
        std::string file_content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        in.close();
        response = {{"mode", "api"}, {"operation", "convert"}, {"status", "ok"}, {"size", file_content.size()}};
        std::remove(input_file.c_str());
        std::remove(output_file.c_str());
      } else {
        response = {{"mode", "api"}, {"operation", "convert"}, {"status", "error"}, {"message", "conversion failed"}};
        std::remove(input_file.c_str());
      }
    }
    auto result = crow::response(response.dump());
    result.set_header("Content-Type", "application/json");
    add_security_headers(result);
    return result;
  });
  CROW_ROUTE(app, "/api/compress").methods("POST"_method)
  ([&engine](const crow::request& req) {
    json response;
    if (req.body.empty()) {
      response = {{"mode", "api"}, {"operation", "compress"}, {"status", "error"}, {"message", "no file provided"}};
    } else {
      std::string input_file = "/tmp/input_" + std::to_string(std::time(nullptr)) + ".pdf";
      std::string output_file = "/tmp/output_" + std::to_string(std::time(nullptr)) + ".pdf";
      std::ofstream out(input_file, std::ios::binary);
      out.write(req.body.c_str(), req.body.size());
      out.close();
      auto op = engine.compress(input_file, output_file);
      std::ifstream in(output_file, std::ios::binary);
      if (in.good()) {
        std::string file_content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        in.close();
        response = {{"mode", "api"}, {"operation", "compress"}, {"status", "ok"}, {"size", file_content.size()}};
        std::remove(input_file.c_str());
        std::remove(output_file.c_str());
      } else {
        response = {{"mode", "api"}, {"operation", "compress"}, {"status", "error"}, {"message", "compression failed"}};
        std::remove(input_file.c_str());
      }
    }
    auto result = crow::response(response.dump());
    result.set_header("Content-Type", "application/json");
    add_security_headers(result);
    return result;
  });
  CROW_ROUTE(app, "/api/merge").methods("POST"_method)
  ([&engine](const crow::request& req) {
    json response;
    if (req.body.empty()) {
      response = {{"mode", "api"}, {"operation", "merge"}, {"status", "error"}, {"message", "no file provided"}};
    } else {
      std::string input_file = "/tmp/input_" + std::to_string(std::time(nullptr)) + ".pdf";
      std::string output_file = "/tmp/output_" + std::to_string(std::time(nullptr)) + ".pdf";
      std::ofstream out(input_file, std::ios::binary);
      out.write(req.body.c_str(), req.body.size());
      out.close();
      std::vector<std::string> inputs = {input_file};
      auto op = engine.merge(inputs, output_file);
      std::ifstream in(output_file, std::ios::binary);
      if (in.good()) {
        std::string file_content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        in.close();
        response = {{"mode", "api"}, {"operation", "merge"}, {"status", "ok"}, {"size", file_content.size()}};
        std::remove(input_file.c_str());
        std::remove(output_file.c_str());
      } else {
        response = {{"mode", "api"}, {"operation", "merge"}, {"status", "error"}, {"message", "merge failed"}};
        std::remove(input_file.c_str());
      }
    }
    auto result = crow::response(response.dump());
    result.set_header("Content-Type", "application/json");
    add_security_headers(result);
    return result;
  });
  CROW_ROUTE(app, "/api/split").methods("POST"_method)
  ([&engine](const crow::request& req) {
    json response;
    if (req.body.empty()) {
      response = {{"mode", "api"}, {"operation", "split"}, {"status", "error"}, {"message", "no file provided"}};
    } else {
      std::string input_file = "/tmp/input_" + std::to_string(std::time(nullptr)) + ".pdf";
      std::string output_file = "/tmp/output_" + std::to_string(std::time(nullptr)) + ".pdf";
      std::ofstream out(input_file, std::ios::binary);
      out.write(req.body.c_str(), req.body.size());
      out.close();
      auto op = engine.split(input_file, output_file);
      std::ifstream in(output_file, std::ios::binary);
      if (in.good()) {
        std::string file_content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        in.close();
        response = {{"mode", "api"}, {"operation", "split"}, {"status", "ok"}, {"size", file_content.size()}};
        std::remove(input_file.c_str());
        std::remove(output_file.c_str());
      } else {
        response = {{"mode", "api"}, {"operation", "split"}, {"status", "error"}, {"message", "split failed"}};
        std::remove(input_file.c_str());
      }
    }
    auto result = crow::response(response.dump());
    result.set_header("Content-Type", "application/json");
    add_security_headers(result);
    return result;
  });
  CROW_ROUTE(app, "/api/rotate").methods("POST"_method)
  ([&engine](const crow::request& req) {
    json response;
    if (req.body.empty()) {
      response = {{"mode", "api"}, {"operation", "rotate"}, {"status", "error"}, {"message", "no file provided"}};
    } else {
      std::string input_file = "/tmp/input_" + std::to_string(std::time(nullptr)) + ".pdf";
      std::string output_file = "/tmp/output_" + std::to_string(std::time(nullptr)) + ".pdf";
      std::ofstream out(input_file, std::ios::binary);
      out.write(req.body.c_str(), req.body.size());
      out.close();
      auto op = engine.rotate_pages(input_file, output_file, 90);
      std::ifstream in(output_file, std::ios::binary);
      if (in.good()) {
        std::string file_content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        in.close();
        response = {{"mode", "api"}, {"operation", "rotate"}, {"status", "ok"}, {"size", file_content.size()}};
        std::remove(input_file.c_str());
        std::remove(output_file.c_str());
      } else {
        response = {{"mode", "api"}, {"operation", "rotate"}, {"status", "error"}, {"message", "rotation failed"}};
        std::remove(input_file.c_str());
      }
    }
    auto result = crow::response(response.dump());
    result.set_header("Content-Type", "application/json");
    add_security_headers(result);
    return result;
  });
  app.port(port).multithreaded().run();
  return 0;
}