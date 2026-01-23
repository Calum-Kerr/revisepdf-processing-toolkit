#include "engine.h"
#include <cstdlib>
#include <sstream>
#include <fstream>
PdfEngine::PdfEngine() {}
PdfEngine::~PdfEngine() {}
PdfOperation PdfEngine::pdf_to_jpg(const std::string& input, const std::string& output) {
  std::string output_pattern = output.substr(0, output.find_last_of('.')) + "_%d.jpg";
  std::ostringstream cmd;
  cmd << "gs -q -dNOPAUSE -dBATCH -dSAFER -sDEVICE=jpeg -r150 -dJPEGQ=85 -sOutputFile=\"" << output_pattern << "\" \"" << input << "\" 2>/dev/null";
  int result = std::system(cmd.str().c_str());
  if (result == 0) {
    std::ostringstream zip_cmd;
    std::string zip_file = output.substr(0, output.find_last_of('.')) + ".zip";
    zip_cmd << "cd /tmp && zip -q \"" << zip_file << "\" " << output.substr(0, output.find_last_of('.')) << "_*.jpg 2>/dev/null && rm -f " << output.substr(0, output.find_last_of('.')) << "_*.jpg";
    std::system(zip_cmd.str().c_str());
    return {input, zip_file, "pdf_to_jpg", "api"};
  }
  return {input, output, "pdf_to_jpg", "error"};
}
PdfOperation PdfEngine::pdf_to_pdfa(const std::string& input, const std::string& output) {
  std::ostringstream cmd;
  cmd << "gs -q -dNOPAUSE -dBATCH -dSAFER -dPDFA=1 -dPDFACompatibilityPolicy=1 -sOutputFile=\"" << output << "\" \"" << input << "\" 2>/dev/null";
  int result = std::system(cmd.str().c_str());
  return {input, output, "pdf_to_pdfa", result == 0 ? "api" : "error"};
}
PdfOperation PdfEngine::compress(const std::string& input, const std::string& output) {
  std::ostringstream cmd;
  cmd << "gs -q -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -dCompatibilityLevel=1.4 -dPDFSETTINGS=/screen -dCompressFonts=true -dCompressStreams=true -r72x72 -dDownsampleColorImages=true -dColorImageResolution=150 -dDownsampleGrayImages=true -dGrayImageResolution=150 -dDownsampleMonoImages=true -dMonoImageResolution=150 -sOutputFile=\"" << output << "\" \"" << input << "\" 2>/dev/null";
  int result = std::system(cmd.str().c_str());
  return {input, output, "compress", result == 0 ? "api" : "error"};
}
PdfOperation PdfEngine::merge(const std::vector<std::string>& inputs, const std::string& output) {
  if (inputs.empty()) {
    return {output, output, "merge", "error"};
  }
  if (inputs.size() == 1) {
    std::ostringstream cmd;
    cmd << "cp \"" << inputs[0] << "\" \"" << output << "\" 2>/dev/null";
    int result = std::system(cmd.str().c_str());
    return {inputs[0], output, "merge", result == 0 ? "api" : "error"};
  }
  std::ostringstream cmd;
  cmd << "gs -q -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -sOutputFile=\"" << output << "\"";
  for (const auto& input : inputs) {
    cmd << " \"" << input << "\"";
  }
  cmd << " 2>/dev/null";
  int result = std::system(cmd.str().c_str());
  return {inputs[0], output, "merge", result == 0 ? "api" : "error"};
}
PdfOperation PdfEngine::split(const std::string& input, const std::string& output) {
  std::string output_pattern = output.substr(0, output.find_last_of('.')) + "_%d.pdf";
  std::ostringstream cmd;
  cmd << "gs -q -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -sOutputFile=\"" << output_pattern << "\" \"" << input << "\" 2>/dev/null";
  int result = std::system(cmd.str().c_str());
  if (result == 0) {
    std::ostringstream zip_cmd;
    std::string zip_file = output.substr(0, output.find_last_of('.')) + ".zip";
    zip_cmd << "cd /tmp && zip -q \"" << zip_file << "\" " << output.substr(0, output.find_last_of('.')) << "_*.pdf 2>/dev/null && rm -f " << output.substr(0, output.find_last_of('.')) << "_*.pdf";
    std::system(zip_cmd.str().c_str());
    return {input, zip_file, "split", "api"};
  }
  return {input, output, "split", "error"};
}
PdfOperation PdfEngine::add_page_numbers(const std::string& input, const std::string& output) {
  std::ostringstream cmd;
  cmd << "gs -q -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -sOutputFile=\"" << output << "\" \"" << input << "\" 2>/dev/null";
  int result = std::system(cmd.str().c_str());
  return {input, output, "add_page_numbers", result == 0 ? "api" : "error"};
}
PdfOperation PdfEngine::rotate_pages(const std::string& input, const std::string& output, int angle) {
  std::ostringstream cmd;
  cmd << "gs -q -dNOPAUSE -dBATCH -dSAFER -sDEVICE=pdfwrite -c \"<</Rotate " << angle << ">> setpagedevice\" -f \"" << input << "\" -sOutputFile=\"" << output << "\" 2>/dev/null";
  int result = std::system(cmd.str().c_str());
  return {input, output, "rotate_pages", result == 0 ? "api" : "error"};
}