#pragma once
#include <string>
#include <vector>
struct PdfOperation {
  std::string input_path;
  std::string output_path;
  std::string operation;
  std::string mode;
};
class PdfEngine {
public:
  PdfEngine();
  ~PdfEngine();
  PdfOperation pdf_to_jpg(const std::string& input, const std::string& output);
  PdfOperation pdf_to_pdfa(const std::string& input, const std::string& output);
  PdfOperation compress(const std::string& input, const std::string& output);
  PdfOperation merge(const std::vector<std::string>& inputs, const std::string& output);
  PdfOperation split(const std::string& input, const std::string& output);
  PdfOperation add_page_numbers(const std::string& input, const std::string& output);
  PdfOperation rotate_pages(const std::string& input, const std::string& output, int angle);
};