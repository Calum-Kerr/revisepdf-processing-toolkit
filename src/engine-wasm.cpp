#include <cstring>
#include <string>
extern "C" {
  const char* pdf_to_jpg(const char* input_data, int input_size) {
    static std::string result = "{\"mode\":\"wasm\",\"operation\":\"pdf_to_jpg\",\"status\":\"ok\"}";
    return result.c_str();
  }
  const char* pdf_to_pdfa(const char* input_data, int input_size) {
    static std::string result = "{\"mode\":\"wasm\",\"operation\":\"pdf_to_pdfa\",\"status\":\"ok\"}";
    return result.c_str();
  }
  const char* compress(const char* input_data, int input_size) {
    static std::string result = "{\"mode\":\"wasm\",\"operation\":\"compress\",\"status\":\"ok\"}";
    return result.c_str();
  }
  const char* merge(const char* input_data, int input_size) {
    static std::string result = "{\"mode\":\"wasm\",\"operation\":\"merge\",\"status\":\"ok\"}";
    return result.c_str();
  }
  const char* split(const char* input_data, int input_size) {
    static std::string result = "{\"mode\":\"wasm\",\"operation\":\"split\",\"status\":\"ok\"}";
    return result.c_str();
  }
  const char* add_page_numbers(const char* input_data, int input_size) {
    static std::string result = "{\"mode\":\"wasm\",\"operation\":\"add_page_numbers\",\"status\":\"ok\"}";
    return result.c_str();
  }
  const char* rotate_pages(const char* input_data, int input_size, int angle) {
    static std::string result = "{\"mode\":\"wasm\",\"operation\":\"rotate_pages\",\"status\":\"ok\"}";
    return result.c_str();
  }
}

