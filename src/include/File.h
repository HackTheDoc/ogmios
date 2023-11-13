#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class File {
public:
    static const char* Filters[4];
    static std::vector<std::string> LoadTXT(fs::path path);

    static bool Export(fs::path path, std::vector<std::string> text);

private:
    // to format markdown file saving
    static const std::vector<char> MardkdownFlags;

    static bool ExportToTXT(fs::path path, std::vector<std::string> text);
    static bool ExportToMD(fs::path path, std::vector<std::string> text);
    static bool ExportToPDF(fs::path path, std::vector<std::string> text);
};
