#include "include/UILabel.h"

#include "include/Window.h"

UILabel::UILabel(const std::string& text) {
    texture = Manager::GenerateText(text, Theme::clr_font);
    rect = { 0,0,0,0 };
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
}

UILabel::~UILabel() {}

void UILabel::draw() {
    Manager::Draw(texture, nullptr, &rect);
}

void UILabel::destroy() {
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void UILabel::setText(const std::string& text) {
    destroy();

    texture = Manager::GenerateText(text, Theme::clr_font);
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
}

void UILabel::setText(const std::string& text, const int mlength) {
    destroy();

    texture = Manager::GenerateText(text, Theme::clr_font, mlength);
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
}
