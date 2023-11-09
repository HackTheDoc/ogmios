#pragma once

#include <string>
#include <SDL2/SDL.h>

class Theme {
public:
    enum Type {
        DAY,
        NIGHT
    };

    Type next;

    SDL_Color font;
    SDL_Color cursor;
    SDL_Color ui;
    SDL_Color ui2;
    SDL_Color uiBackground;
    SDL_Color textBackground;
    SDL_Color selection;

    std::string icon;

    int fontSize;

    Theme();
    ~Theme();

    void load(Type t);
};
