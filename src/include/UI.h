#pragma once

#include "UILabel.h"
#include "UIButton.h"

class UI {
public:
    UI(int w, int h);
    ~UI();

    void init();
    void update();
    void render();
    void destroy();

    void place(int x, int y);
    void setWidth(int w);
    void reload();

    int height();

private:
    SDL_Rect viewport;

    UIButton* saveButton;
    UIButton* loadButton;
    UIButton* minusSizeButton;
    UIButton* sizeButton;
    UIButton* plusSizeButton;
    UIButton* themeButton;

    UILabel* label;

    SDL_Rect border;
};
