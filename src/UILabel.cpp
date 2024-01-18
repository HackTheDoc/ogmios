#include "include/UILabel.h"

#include "include/Window.h"

UILabel::UILabel(std::string text) {
    texture = Manager::GenerateText(text.c_str(), Window::theme.font);
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
