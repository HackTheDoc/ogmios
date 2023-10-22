#include "include/UIButton.h"

#include "include/Window.h"

const int UIButton::WIDTH = 50;

UIButton::UIButton(UIButton::ID id) {
    this->id = id;

    rect = {0,0,UIButton::WIDTH, 2*Window::ui->height()/3};

    switch (id) {
    case UIButton::ID::SAVE:
        texture = Manager::GenerateText("Save", Window::theme.ui);
        break;
    case UIButton::ID::LOAD:
        texture = Manager::GenerateText("Load", Window::theme.ui);
        break;
    case UIButton::ID::MINUS_SIZE:
        texture = Manager::GenerateText("-", Window::theme.ui);
        break;
    case UIButton::ID::DEFAULT_SIZE:
        texture = Manager::GenerateText("Size", Window::theme.ui);
        rect.w -= 10;
        break;
    case UIButton::ID::PLUS_SIZE:
        texture = Manager::GenerateText("+", Window::theme.ui);
        break;
    case UIButton::ID::THEME_ICON:
        texture = Manager::LoadTexture(Window::theme.icon.c_str());
        rect.w = rect.h;
        break;
    case UIButton::ID::UNKNOWN:
    default:
        texture = nullptr;
        break;
    }

    textRect = {0,0,0,0};
    SDL_QueryTexture(texture, NULL, NULL, &textRect.w, &textRect.h);

    place(0,0);
}

UIButton::~UIButton() {}

void UIButton::draw() {
    if (texture == nullptr) return;

    Manager::Draw(texture, nullptr, &textRect);

    Manager::DrawRect(&rect, Window::theme.ui);
}

void UIButton::update() {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    if (SDL_PointInRect(&mouse, &rect) && Window::event.mouseClickLeft())
        Window::event.handleButtonClick(id);
}

void UIButton::destroy() {
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void UIButton::place(int x, int y) {
    rect.x = x;
    rect.y = y;

    textRect.x = x + (rect.w - textRect.w)/2;
    textRect.y = y + (rect.h - textRect.h)/2;
}
