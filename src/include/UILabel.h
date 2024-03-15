#pragma once

#include <string>

#include "UIElement.h"

class UILabel : public UIElement {
private:
    SDL_Texture* texture;

public:
    UILabel(const std::string& text);
    ~UILabel();

    void draw() override;
    void destroy() override;

    void setText(const std::string& text);
    void setText(const std::string& text, const int mlength);
};
