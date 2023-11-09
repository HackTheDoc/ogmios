#pragma once

#include <string>
#include <SDL2/SDL.h>

#include "UIElement.h"

class UILine : public UIElement {
public:
    UILine(std::string text, int n);
    ~UILine();

    void draw() override;
    void update() override;
    void destroy() override;

    void reload();

    void setText(std::string text);
    void setNumber(int n);

    std::string getText();
    int getNumber();

    void insert(int p, char c);
    void insert(int p, char* c);
    void erase(int p, int l);
    void append(std::string s);
    int size();
    void clear();

    int height();

    void useAsMargin();

private:
    std::string text;
    SDL_Texture* texture;
    SDL_Rect textureRect;
    
    int n;
    SDL_Texture* number;
    SDL_Rect numberRect;
};
