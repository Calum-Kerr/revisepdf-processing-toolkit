#include "engine.h"
PdfEngine::PdfEngine() {}
PdfEngine::~PdfEngine() {}
PdfOperation PdfEngine::pdf_to_jpg(const std::string& input, const std::string& output) {
  return {input, output, "pdf_to_jpg", "api"};
}
PdfOperation PdfEngine::pdf_to_pdfa(const std::string& input, const std::string& output) {
  return {input, output, "pdf_to_pdfa", "api"};
}
PdfOperation PdfEngine::compress(const std::string& input, const std::string& output) {
  return {input, output, "compress", "api"};
}
PdfOperation PdfEngine::merge(const std::vector<std::string>& inputs, const std::string& output) {
  return {inputs[0], output, "merge", "api"};
}
PdfOperation PdfEngine::split(const std::string& input, const std::string& output) {
  return {input, output, "split", "api"};
}
PdfOperation PdfEngine::add_page_numbers(const std::string& input, const std::string& output) {
  return {input, output, "add_page_numbers", "api"};
}
PdfOperation PdfEngine::rotate_pages(const std::string& input, const std::string& output, int angle) {
  return {input, output, "rotate_pages", "api"};
}