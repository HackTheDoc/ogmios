#pragma once

#include <string>
#include <SDL2/SDL.h>

struct Theme {
public:
    static std::string name;

    static SDL_Color clr_font;
    static SDL_Color clr_cursor;
    static SDL_Color clr_ui;
    static SDL_Color clr_ui_background;
    static SDL_Color clr_editor_background;
    static SDL_Color clr_selection;

    static std::string icon;

    static void load(const std::string& name);
};
