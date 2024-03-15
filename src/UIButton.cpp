#include "include/UIButton.h"

#include "include/Window.h"
#include "include/Config.h"

UIButton::UIButton(const Event::ID eid) {
    this->eid = eid;

    rect = { 0, 0, Config::BUTTON_WIDTH, 2 * Window::ui->height() / 3 };

    switch (eid) {
    case Event::ID::SAVE:
        text = Manager::GenerateText("Save", Theme::clr_ui);
        break;
    case Event::ID::LOAD:
        text = Manager::GenerateText("Load", Theme::clr_ui);
        break;
    case Event::ID::MINUS_FONT_SIZE:
        text = Manager::GenerateText("-", Theme::clr_ui);
        break;
    case Event::ID::DEFAULT_FONT_SIZE:
        text = Manager::GenerateText("Size", Theme::clr_ui);
        rect.w -= 10;
        break;
    case Event::ID::PLUS_FONT_SIZE:
        text = Manager::GenerateText("+", Theme::clr_ui);
        break;
    case Event::ID::CHANGE_THEME:
        text = Manager::LoadTexture(Theme::icon);
        rect.w = rect.h;
        break;
    case Event::ID::UNKNOWN:
    default:
        text = nullptr;
        break;
    }

    textRect = { 0,0,0,0 };
    SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);

    place(0, 0);
}

UIButton::~UIButton() {}

void UIButton::draw() {
    Manager::Draw(text, nullptr, &textRect);

    Manager::DrawRect(&rect, Theme::clr_ui);
}

void UIButton::update() {
    SDL_Point m;
    SDL_GetMouseState(&m.x, &m.y);

    if (SDL_PointInRect(&m, &rect) && Window::event.mouseClickLeft())
        Window::event.raise(eid);
}

void UIButton::destroy() {
    SDL_DestroyTexture(text);
    text = nullptr;
}

void UIButton::place(int x, int y) {
    rect.x = x;
    rect.y = y;

    textRect.x = x + (rect.w - textRect.w) / 2;
    textRect.y = y + (rect.h - textRect.h) / 2;
}
