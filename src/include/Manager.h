#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Config.h"
#include "Theme.h"

class Manager {
private:
    static TTF_Font* font;

public:
    static void SetViewport(const SDL_Rect* v);

    static SDL_Color SetRenderDrawColor(const SDL_Color& c);

    /* ----- TEXTURES ----- */

    static SDL_Texture* LoadTexture(const std::string& filepath);

    static SDL_Texture* GenerateText(const std::string& text, const SDL_Color& c);
    static SDL_Texture* GenerateText(const std::string& text, const SDL_Color& c, const int mlength);

    static void Draw(SDL_Texture* texture, const SDL_Rect* src, const SDL_Rect* dest);

    static void DrawRect(const SDL_Rect* r, const SDL_Color& c);
    static void DrawFilledRect(const SDL_Rect* r, const SDL_Color& c);
    static void DrawBorder(const SDL_Rect* r, const SDL_Color& c);

    /* ----- FONT ----- */

    static void OpenFont(const char* filepath);
    static void SetFontSize(const int size);
    static void CloseFont();

    static void SizeText(const std::string& text, int* w, int* h);
};
