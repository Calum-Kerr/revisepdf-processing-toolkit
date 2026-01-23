#include "engine.h"
#include <cstdlib>
#include <sstream>
#include <fstream>
PdfEngine::PdfEngine() {}
PdfEngine::~PdfEngine() {}
PdfOperation PdfEngine::pdf_to_jpg(const std::string& input, const std::string& output) {
  std::ostringstream cmd;
  cmd << "gs -q -dNOPAUSE -dBATCH -dSAFER -sDEVICE=jpeg -r150 -sOutputFile=\"" << output << "\" \"" << input << "\"";
  int result = std::system(cmd.str().c_str());
  return {input, output, "pdf_to_jpg", result == 0 ? "api" : "error"};
}
PdfOperation PdfEngine::pdf_to_pdfa(const std::string& input, const std::string& output) {
  std::ostringstream cmd;
  cmd << "gs -q -dNOPAUSE -dBATCH -dSAFER -dPDFA=1 -sOutputFile=\"" << output << "\" \"" << input << "\"";
  int result = std::system(cmd.str().c_str());
  return {input, output, "pdf_to_pdfa", result == 0 ? "api" : "error"};
}
PdfOperation PdfEngine::compress(const std::string& input, const std::string& output) {
  std::ostringstream cmd;
  cmd << "gs -q -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -dCompatibilityLevel=1.4 -dPDFSETTINGS=/ebook -sOutputFile=\"" << output << "\" \"" << input << "\"";
  int result = std::system(cmd.str().c_str());
  return {input, output, "compress", result == 0 ? "api" : "error"};
}
PdfOperation PdfEngine::merge(const std::vector<std::string>& inputs, const std::string& output) {
  std::ostringstream cmd;
  cmd << "gs -q -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -sOutputFile=\"" << output << "\"";
  for (const auto& input : inputs) {
    cmd << " \"" << input << "\"";
  }
  int result = std::system(cmd.str().c_str());
  return {inputs[0], output, "merge", result == 0 ? "api" : "error"};
}
PdfOperation PdfEngine::split(const std::string& input, const std::string& output) {
  std::ostringstream cmd;
  cmd << "gs -q -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -sOutputFile=\"" << output << "\" \"" << input << "\"";
  int result = std::system(cmd.str().c_str());
  return {input, output, "split", result == 0 ? "api" : "error"};
}
PdfOperation PdfEngine::add_page_numbers(const std::string& input, const std::string& output) {
  std::ostringstream cmd;
  cmd << "gs -q -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -sOutputFile=\"" << output << "\" \"" << input << "\"";
  int result = std::system(cmd.str().c_str());
  return {input, output, "add_page_numbers", result == 0 ? "api" : "error"};
}
PdfOperation PdfEngine::rotate_pages(const std::string& input, const std::string& output, int angle) {
  std::ostringstream cmd;
  cmd << "gs -q -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -sOutputFile=\"" << output << "\" \"" << input << "\"";
  int result = std::system(cmd.str().c_str());
  return {input, output, "rotate_pages", result == 0 ? "api" : "error"};
}