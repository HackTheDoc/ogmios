#include "include/Config.h"

#include "include/Window.h"

#include <filesystem>
#include <fstream>

const int Config::BUTTON_WIDTH = 50;

const int Config::FONT_DEFAULT_SIZE = 16;
const int Config::FONT_MIN_SIZE = 8;
const int Config::FONT_MAX_SIZE = 512;

const int Config::EDITOR_DEFAULT_LEFT_MARGIN = 24;
const int Config::EDITOR_DEFAULT_LINE_HEIGHT = 18;

const int Config::WINDOW_MIN_WIDTH = 384;
const int Config::WINDOW_MIN_HEIGHT = 128;
const int Config::WINDOW_DEFAULT_WIDTH = 800;
const int Config::WINDOW_DEFAULT_HEIGHT = 600;

int Config::fontSize = Config::FONT_DEFAULT_SIZE;

void Config::Create() {
    Data c = {
        .theme = "day",
        .lastfile = "",
        .cursor_x = 0,
        .cursor_y = 0,
        .scroll_pos = 0,
        .font_size = FONT_DEFAULT_SIZE,
        .left_margin = EDITOR_DEFAULT_LEFT_MARGIN,
        .line_height = EDITOR_DEFAULT_LINE_HEIGHT
    };

    std::ofstream outfile("config", std::ios::binary);

    size_t stheme = c.theme.size();
    outfile.write(reinterpret_cast<const char*>(&stheme), sizeof(size_t));
    outfile.write(c.theme.data(), stheme);

    size_t sfile = c.lastfile.size();
    outfile.write(reinterpret_cast<const char*>(&sfile), sizeof(size_t));
    outfile.write(c.lastfile.data(), sfile);

    outfile.write(reinterpret_cast<const char*>(&c.cursor_x), sizeof(int));
    outfile.write(reinterpret_cast<const char*>(&c.cursor_y), sizeof(int));
    outfile.write(reinterpret_cast<const char*>(&c.scroll_pos), sizeof(int));
    outfile.write(reinterpret_cast<const char*>(&c.font_size), sizeof(int));
    outfile.write(reinterpret_cast<const char*>(&c.left_margin), sizeof(int));
    outfile.write(reinterpret_cast<const char*>(&c.line_height), sizeof(int));

    outfile.close();
}

void Config::Load() {
    if (!std::filesystem::exists("config")) Create();

    Data d;

    std::ifstream infile("config", std::ios::binary);

    size_t stheme;
    infile.read(reinterpret_cast<char*>(&stheme), sizeof(size_t));
    d.theme.resize(stheme);
    infile.read(&d.theme[0], stheme);

    size_t sfile;
    infile.read(reinterpret_cast<char*>(&sfile), sizeof(size_t));
    d.lastfile.resize(sfile);
    infile.read(&d.lastfile[0], sfile);

    infile.read(reinterpret_cast<char*>(&d.cursor_x), sizeof(int));
    infile.read(reinterpret_cast<char*>(&d.cursor_y), sizeof(int));
    infile.read(reinterpret_cast<char*>(&d.scroll_pos), sizeof(int));
    infile.read(reinterpret_cast<char*>(&d.font_size), sizeof(int));
    infile.read(reinterpret_cast<char*>(&d.left_margin), sizeof(int));
    infile.read(reinterpret_cast<char*>(&d.line_height), sizeof(int));

    infile.close();

    Window::editor->importData(d);
};

void Config::Save() {
    Data d = Window::editor->exportData();

    std::ofstream outfile("config", std::ios::binary);

    size_t stheme = d.theme.size();
    outfile.write(reinterpret_cast<const char*>(&stheme), sizeof(size_t));
    outfile.write(d.theme.data(), stheme);

    size_t sfile = d.lastfile.size();
    outfile.write(reinterpret_cast<const char*>(&sfile), sizeof(size_t));
    outfile.write(d.lastfile.data(), sfile);

    outfile.write(reinterpret_cast<const char*>(&d.cursor_x), sizeof(int));
    outfile.write(reinterpret_cast<const char*>(&d.cursor_y), sizeof(int));
    outfile.write(reinterpret_cast<const char*>(&d.scroll_pos), sizeof(int));
    outfile.write(reinterpret_cast<const char*>(&d.font_size), sizeof(int));
    outfile.write(reinterpret_cast<const char*>(&d.left_margin), sizeof(int));
    outfile.write(reinterpret_cast<const char*>(&d.line_height), sizeof(int));

    outfile.close();
}
