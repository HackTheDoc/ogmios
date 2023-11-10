#include "include/File.h"

#include <fstream>
#include <iostream>

const char* File::Filters[4] = {
    "*.txt",
    "*.text",

    "*.md",
    "*.markdown"
};

/* ----- IMPORT FUNCTIONS ----- */
std::vector<std::string> File::LoadTXT(fs::path path) {
    std::vector<std::string> text;

    std::ifstream infile(path);

    std::string l;
    while(getline(infile, l))
        text.push_back(l);

    infile.close();

    return text;
}

/* ----- EXPORT FUNCTIONS ----- */
bool File::Export(fs::path path, std::vector<std::string> text) {
    if (!fs::exists(path));

    std::string extension = path.extension();

    if (extension == ".txt" || extension == ".text")
        return ExportToTXT(path, text);
    if (extension == ".md" || extension == ".markdown")
        return ExportToMD(path, text);
    if (extension == ".pdf")
        return ExportToPDF(path, text);
    
    return false;
}

bool File::ExportToTXT(fs::path path, std::vector<std::string> text) {
    std::ofstream outfile(path);
    
    for (auto l : text) {
        outfile << l << std::endl;
    }
    outfile.close();

    return true;
}

bool File::ExportToMD(fs::path path, std::vector<std::string> text) {
    ExportToTXT(path, text);

    return true;
}

bool File::ExportToPDF(fs::path path, std::vector<std::string> text) {
    ExportToTXT(path, text);

    return true;
}
