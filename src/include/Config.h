#pragma once

#include <map>
#include <string>
#include <SDL2/SDL.h>

struct Data {
    std::string theme;
    std::string lastfile;
    int cursor_x;
    int cursor_y;
    int scroll_pos;
    int font_size;
    int left_margin;
    int line_height;
};

struct Config {
    static const int BUTTON_WIDTH;

    static const int FONT_DEFAULT_SIZE;
    static const int FONT_MIN_SIZE;
    static const int FONT_MAX_SIZE;

    static const int EDITOR_DEFAULT_LEFT_MARGIN;
    static const int EDITOR_DEFAULT_LINE_HEIGHT;

    static const int WINDOW_MIN_WIDTH;
    static const int WINDOW_MIN_HEIGHT;
    static const int WINDOW_DEFAULT_WIDTH;
    static const int WINDOW_DEFAULT_HEIGHT;

    static int fontSize;

    static void Create();

    static void Load();

    static void Save();
};
