#pragma once

#include "Event.h"
#include "UIElement.h"

class UIButton : public UIElement {
private:
    Event::ID eid;
    SDL_Texture* text;
    SDL_Rect textRect;

public:
    UIButton(const Event::ID eid);
    ~UIButton();

    void place(int x, int y);

    void draw() override;
    void update() override;
    void destroy() override;
};
