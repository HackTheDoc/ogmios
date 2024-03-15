#include "include/File.h"

#include <algorithm>
#include <iostream>
#include <fstream>

const char* File::FILTERS[4] = {
    "*.txt",
    "*.text"
};

/* ----- IMPORT ----- */

std::vector<std::string> File::LoadTXT(const fs::path& path) {
    std::vector<std::string> buf;

    {
        std::ifstream infile(path);

        std::string l;
        while (getline(infile, l))
            buf.push_back(l);
    }

    return buf;
}

/* ----- EXPORT ----- */

bool File::Export(const fs::path& path, const std::vector<std::string>& text) {
    return ExportToTXT(path, text);
}

bool File::ExportToTXT(const fs::path& path, const std::vector<std::string>& text) {
    std::ofstream outfile(path);

    for (const auto& l : text)
        outfile << l << std::endl;
    
    outfile.close();

    return true;
}
