#pragma once

#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class File {
private:
    static bool ExportToTXT(const fs::path& path, const std::vector<std::string>& text);

public:
    static const char* FILTERS[4];

    static std::vector<std::string> LoadTXT(const fs::path& path);

    static bool Export(const fs::path& path, const std::vector<std::string>& text);
};
