#pragma once

#include <SDL2/SDL.h>

class UIElement {
protected:
    SDL_Rect rect;

public:
    static const int SPAN = 5;

    virtual void draw() {}
    virtual void update() {}
    virtual void destroy() {}

    void place(const int x, const int y) {
        rect.x = x;
        rect.y = y;
    }

    int x() { return rect.x; }

    int y() { return rect.y; }

    int width() { return rect.w; }

    int height() { return rect.h; }
};
