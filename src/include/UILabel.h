#pragma once

#include <string>

#include "UIElement.h"

class UILabel : public UIElement {
private:
    SDL_Texture* texture;

public:
    UILabel(std::string text);
    ~UILabel();

    void draw() override;
    void destroy() override;
};
