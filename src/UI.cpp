#include "include/UI.h"

#include "include/Window.h"

UI::UI(int w, int h) {
    viewport = { 0,0,w,h };
    border = { 0,h - 1,w,1 };

    saveButton = nullptr;
    loadButton = nullptr;
    minusSizeButton = nullptr;
    sizeButton = nullptr;
    plusSizeButton = nullptr;
    themeButton = nullptr;

    label = nullptr;

}

UI::~UI() {}

void UI::init() {
    Manager::SetFontSize(Manager::DEFAULT_FONT_SIZE);

    Manager::SetRenderDrawColor(Window::theme.ui);

    saveButton = new UIButton(UIButton::ID::SAVE);
    saveButton->place(UIElement::span, UIElement::span);

    loadButton = new UIButton(UIButton::ID::LOAD);
    loadButton->place(
        saveButton->x() + saveButton->width() + UIElement::span,
        UIElement::span
    );

    minusSizeButton = new UIButton(UIButton::ID::MINUS_SIZE);
    minusSizeButton->place(
        loadButton->x() + loadButton->width() + UIElement::span,
        UIElement::span
    );

    sizeButton = new UIButton(UIButton::ID::DEFAULT_SIZE);
    sizeButton->place(
        minusSizeButton->x() + minusSizeButton->width() + UIElement::span,
        UIElement::span
    );

    plusSizeButton = new UIButton(UIButton::ID::PLUS_SIZE);
    plusSizeButton->place(
        sizeButton->x() + sizeButton->width() + UIElement::span,
        UIElement::span
    );

    themeButton = new UIButton(UIButton::ID::THEME_ICON);
    themeButton->place(
        Window::screen.w - themeButton->width() - UIElement::span,
        UIElement::span
    );

    label = new UILabel("Ogmios Editor");
    label->place(
        themeButton->x() - label->width() - UIElement::span,
        UIElement::span
    );

    Manager::SetFontSize(Window::theme.fontSize);
}

void UI::update() {
    saveButton->update();
    loadButton->update();
    minusSizeButton->update();
    sizeButton->update();
    plusSizeButton->update();
    themeButton->update();
}

void UI::render() {
    Manager::DrawFilledRect(&viewport, Window::theme.uiBackground);

    Manager::SetViewport(&viewport);

    saveButton->draw();
    loadButton->draw();
    minusSizeButton->draw();
    sizeButton->draw();
    plusSizeButton->draw();
    themeButton->draw();

    label->draw();

    Manager::DrawFilledRect(&border, Window::theme.ui);

    Manager::SetViewport(nullptr);
}

void UI::destroy() {
    saveButton->destroy();
    saveButton = nullptr;

    loadButton->destroy();
    loadButton = nullptr;

    minusSizeButton->destroy();
    minusSizeButton = nullptr;

    sizeButton->destroy();
    sizeButton = nullptr;

    plusSizeButton->destroy();
    plusSizeButton = nullptr;

    themeButton->destroy();
    themeButton = nullptr;

    label->destroy();
    label = nullptr;
}

void UI::place(int x, int y) {
    viewport.x = x;
    viewport.y = y;
}

void UI::setWidth(int w) {
    viewport.w = w;
    border.w = w;

    themeButton->place(
        Window::screen.w - themeButton->width() - UIElement::span,
        UIElement::span
    );

    label->place(
        themeButton->x() - label->width() - UIElement::span,
        UIElement::span
    );
}

void UI::reload() {
    destroy();
    init();
}

int UI::height() {
    return viewport.h;
}
