#include "include/UILine.h"

#include "include/Window.h"
#include "include/Editor.h"

UILine::UILine(const std::string& t, const int n) {
    rect = { 0, 0, Window::screen.w, Editor::LineHeight };

    text = "";
    lbl_text = new UILabel("");

    number = 0;
    lbl_number = new UILabel("0");

    setNumber(n);
    setText(t);

    lbl_number->place(
        lbl_number->x(),
        n * Editor::LineHeight + 1
    );

    lbl_text->place(lbl_text->x(), lbl_number->y());
}

UILine::~UILine() {}

void UILine::reload() {
    destroy();
    rect.w = Window::screen.w;
    setText(text);
    setNumber(number);
}

void UILine::drawAt(int y) {
    lbl_number->place(lbl_number->x(), y);
    lbl_text->place(lbl_text->x(), y);
    draw();
}

void UILine::draw() {
    lbl_number->draw();

    SDL_Rect l = {
        Editor::LeftMargin - 2,
        lbl_number->y() + 1,
        1,
        lbl_number->height() - 1
    };
    Manager::DrawRect(&l, Theme::clr_ui);

    if (!text.empty())
        lbl_text->draw();
}

void UILine::update() {
    lbl_number->place(
        (Editor::LeftMargin - lbl_number->width()) / 2,
        lbl_number->y()
    );

    lbl_text->place(
        Editor::LeftMargin,
        lbl_text->y()
    );
}

void UILine::destroy() {
    lbl_text->destroy();
    lbl_number->destroy();
}

void UILine::setText(const std::string& t) {
    text = t;

    lbl_text->setText(t, rect.w);
}

void UILine::setNumber(const int n) {
    number = n;

    lbl_number->setText(std::to_string(n + 1));

    if (lbl_number->width() > Editor::LeftMargin)
        useAsMargin();
}

void UILine::insert(const int p, const char c) {
    text.insert(text.begin() + p, 1, c);
    setText(text);
}

void UILine::insert(const int p, const char* c) {
    text.insert(p, c);
    setText(text);
}

void UILine::erase(const int p, const int l) {
    text.erase(p, l);
    setText(text);
}

void UILine::append(const std::string& s) {
    text.append(s);
    setText(text);
}

int UILine::size() {
    return text.size();
}

void UILine::clear() {
    setText("");
}

int UILine::y() {
    return lbl_number->y();
}

int UILine::height() {
    return std::max(std::max(lbl_number->height(), lbl_text->height()), Editor::LineHeight);
}

int UILine::getNumber() {
    return number;
}

std::string UILine::getText() {
    return text;
}

void UILine::useAsMargin() {
    Editor::LeftMargin = lbl_number->width() + 8;
}
