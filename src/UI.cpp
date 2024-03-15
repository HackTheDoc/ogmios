#include "include/UI.h"

#include "include/Window.h"

UI::UI(int w, int h) {
    container = { 0, 0, w, h };
    border = { 0, h - 1, w, 1 };

    btn_save = nullptr;
    btn_load = nullptr;
    btn_minus = nullptr;
    btn_size = nullptr;
    btn_plus = nullptr;
    btn_theme = nullptr;

    label = nullptr;
}

UI::~UI() {}

void UI::init() {
    Manager::SetFontSize(Config::FONT_DEFAULT_SIZE);

    btn_save = new UIButton(Event::ID::SAVE);
    btn_save->place(UIElement::SPAN, UIElement::SPAN);

    btn_load = new UIButton(Event::ID::LOAD);
    btn_load->place(
        btn_save->x() + btn_save->width() + UIElement::SPAN,
        UIElement::SPAN
    );

    btn_minus = new UIButton(Event::ID::MINUS_FONT_SIZE);
    btn_minus->place(
        btn_load->x() + btn_load->width() + UIElement::SPAN,
        UIElement::SPAN
    );

    btn_size = new UIButton(Event::ID::DEFAULT_FONT_SIZE);
    btn_size->place(
        btn_minus->x() + btn_minus->width() + UIElement::SPAN,
        UIElement::SPAN
    );

    btn_plus = new UIButton(Event::ID::PLUS_FONT_SIZE);
    btn_plus->place(
        btn_size->x() + btn_size->width() + UIElement::SPAN,
        UIElement::SPAN
    );

    btn_theme = new UIButton(Event::ID::CHANGE_THEME);
    btn_theme->place(
        Window::screen.w - btn_theme->width() - UIElement::SPAN,
        UIElement::SPAN
    );

    label = new UILabel(Window::Title);
    label->place(
        btn_theme->x() - label->width() - UIElement::SPAN,
        UIElement::SPAN
    );

    Manager::SetFontSize(Config::fontSize);
}

void UI::update() {
    btn_save->update();
    btn_load->update();
    btn_minus->update();
    btn_size->update();
    btn_plus->update();
    btn_theme->update();
}

void UI::render() {
    Manager::DrawFilledRect(&container, Theme::clr_ui_background);

    Manager::SetViewport(&container);

    btn_save->draw();
    btn_load->draw();
    btn_minus->draw();
    btn_size->draw();
    btn_plus->draw();
    btn_theme->draw();

    label->draw();

    Manager::DrawFilledRect(&border, Theme::clr_ui);

    Manager::SetViewport(nullptr);
}

void UI::destroy() {
    btn_save->destroy();
    btn_load->destroy();
    btn_minus->destroy();
    btn_size->destroy();
    btn_plus->destroy();
    btn_theme->destroy();
    label->destroy();
}

void UI::place(int x, int y) {
    container.x = x;
    container.y = y;
}

void UI::setWidth(int w) {
    container.w = w;
    border.w = w;

    btn_theme->place(
        Window::screen.w - btn_theme->width() - UIElement::SPAN,
        UIElement::SPAN
    );

    label->place(
        btn_theme->x() - label->width() - UIElement::SPAN,
        UIElement::SPAN
    );
}

void UI::reload() {
    destroy();
    init();
}

int UI::height() {
    return container.h;
}
