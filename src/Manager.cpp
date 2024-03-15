#include "include/Manager.h"

#include "include/Window.h"

TTF_Font* Manager::font = nullptr;

void Manager::SetViewport(const SDL_Rect* v) {
    SDL_RenderSetViewport(Window::renderer, v);
}

SDL_Color Manager::SetRenderDrawColor(const SDL_Color& c) {
    SDL_Color old;
    SDL_GetRenderDrawColor(Window::renderer, &old.r, &old.g, &old.b, &old.a);

    SDL_SetRenderDrawColor(Window::renderer, c.r, c.g, c.b, c.a);

    return old;
}

/* ----- TEXTURES ----- */

SDL_Texture* Manager::LoadTexture(const std::string& filepath) {
    SDL_Surface* tmpSurface = IMG_Load(filepath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::renderer, tmpSurface);
    
    SDL_FreeSurface(tmpSurface);
    return texture;
}

SDL_Texture* Manager::GenerateText(const std::string& text, const SDL_Color& c) {
    SDL_Surface* tempSurface = TTF_RenderUTF8_Blended(font, text.c_str(), c);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::renderer, tempSurface);
    
    SDL_FreeSurface(tempSurface);
    return texture;
}

SDL_Texture* Manager::GenerateText(const std::string& text, const SDL_Color& c, int mlength) {
    SDL_Surface* tempSurface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), c, mlength);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::renderer, tempSurface);
    
    SDL_FreeSurface(tempSurface);
    return texture;
}

void Manager::Draw(SDL_Texture* text, const SDL_Rect* src, const SDL_Rect* dest) {
    if (text == nullptr) return;
    SDL_RenderCopy(Window::renderer, text, src, dest);
}

void Manager::DrawRect(const SDL_Rect* rect, const SDL_Color& c) {
    SDL_Color old = SetRenderDrawColor(c);

    SDL_RenderDrawRect(Window::renderer, rect);

    SetRenderDrawColor(old);
}

void Manager::DrawFilledRect(const SDL_Rect* rect, const SDL_Color& c) {
    SDL_Color old = SetRenderDrawColor(c);

    SDL_RenderFillRect(Window::renderer, rect);

    SetRenderDrawColor(old);
}

void Manager::DrawBorder(const SDL_Rect* rect, const SDL_Color& c) {
    SDL_Color old = SetRenderDrawColor(c);

    for (int i = -1; i <= 1; i++) {
        SDL_Rect r = {
            rect->x - i,
            rect->y - i,
            rect->w + 2 * i,
            rect->h + 2 * i
        };
        SDL_RenderDrawRect(Window::renderer, &r);
    }

    SetRenderDrawColor(old);
}

/* ----- FONT ----- */

void Manager::OpenFont(const char* filepath) {
    font = TTF_OpenFont(filepath, Config::FONT_DEFAULT_SIZE);
    Config::fontSize = Config::FONT_DEFAULT_SIZE;
}

void Manager::SetFontSize(int size) {
    TTF_SetFontSize(font, size);
}

void Manager::CloseFont() {
    TTF_CloseFont(font);
    font = nullptr;
}

void Manager::SizeText(const std::string& text, int* w, int* h) {
    TTF_SizeText(font, text.c_str(), w, h);
}
