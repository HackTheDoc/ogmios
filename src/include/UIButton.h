#pragma once

#include <string>

#include "UIElement.h"

class UIButton : public UIElement {
public:
    static const int WIDTH;
    
    enum ID {
        UNKNOWN,
        SAVE,
        LOAD,
        MINUS_SIZE,
        DEFAULT_SIZE,
        PLUS_SIZE,
        THEME_ICON
    };

    UIButton(UIButton::ID id);
    ~UIButton();

    void place(int x, int y);

    void draw() override;
    void update() override;
    void destroy() override;

private:
    UIButton::ID id;
    SDL_Texture* texture;
    SDL_Rect textRect;
};
