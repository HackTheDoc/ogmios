#include "include/Manager.h"

#include "include/Window.h"

#include <iostream>

const int Manager::DEFAULT_FONT_SIZE = 16;
const int Manager::MIN_FONT_SIZE = 8;
const int Manager::MAX_FONT_SIZE = 512;

TTF_Font* Manager::font = nullptr;

void Manager::SetViewport(SDL_Rect* viewport) {
    SDL_RenderSetViewport(Window::renderer, viewport);
}

/* -----  COLOR MANAGER  ----- */

SDL_Color Manager::SetRenderDrawColor(int r, int g, int b, int a) {
    SDL_Color old;
    SDL_GetRenderDrawColor(Window::renderer, &old.r, &old.g, &old.b, &old.a);

    SDL_SetRenderDrawColor(Window::renderer, r, g, b, a);

    return old;
}

SDL_Color Manager::SetRenderDrawColor(SDL_Color c) {
    return SetRenderDrawColor(c.r, c.g, c.b, c.a);
}

/* ----- TEXTURE MANAGER ----- */

SDL_Texture* Manager::LoadTexture(const char* filepath) {
    SDL_Surface* tmpSurface = IMG_Load(filepath);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);


    return texture;
}

SDL_Texture* Manager::GenerateText(const char* text, SDL_Color color) {

    SDL_Surface* tempSurface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return texture;
}

SDL_Texture* Manager::GenerateText(const char* text, SDL_Color color, int length) {

    SDL_Surface* tempSurface = TTF_RenderText_Blended_Wrapped(font, text, color, length);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return texture;
}

void Manager::Draw(SDL_Texture* text, SDL_Rect* src, SDL_Rect* dest, SDL_RendererFlip flip) {
    if (text == nullptr) return;
    SDL_RenderCopyEx(Window::renderer, text, src, dest, 0, NULL, flip);
}

void Manager::DrawRect(SDL_Rect* rect, SDL_Color color) {
    SDL_Color old = SetRenderDrawColor(color);

    SDL_RenderDrawRect(Window::renderer, rect);

    SetRenderDrawColor(old);
}

void Manager::DrawFilledRect(SDL_Rect* rect, SDL_Color color) {
    SDL_Color old = SetRenderDrawColor(color);

    SDL_RenderFillRect(Window::renderer, rect);

    SetRenderDrawColor(old);
}

void Manager::DrawBorder(SDL_Rect* rect, SDL_Color color) {
    SDL_Color old = SetRenderDrawColor(color);

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

/* ----- FONT MANAGER ----- */
void Manager::OpenFont(const char* filepath) {
    font = TTF_OpenFont(filepath, DEFAULT_FONT_SIZE);
    Window::theme.fontSize = DEFAULT_FONT_SIZE;
}

void Manager::SetFontSize(int size) {
    TTF_SetFontSize(font, size);
}

void Manager::CloseFont() {
    TTF_CloseFont(font);
    font = nullptr;
}
