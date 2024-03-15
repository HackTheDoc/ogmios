#pragma once

#include "UILabel.h"
#include "UIButton.h"

class UI {
private:
    SDL_Rect container;

    UIButton* btn_save;
    UIButton* btn_load;
    UIButton* btn_minus;
    UIButton* btn_size;
    UIButton* btn_plus;
    UIButton* btn_theme;

    UILabel* label;

    SDL_Rect border;

public:
    UI(const int w, const int h);
    ~UI();

    void init();
    void update();
    void render();
    void destroy();

    void place(const int x, const int y);
    void setWidth(const int w);
    void reload();

    int height();    
};
