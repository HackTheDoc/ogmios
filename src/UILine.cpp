#include "include/UILine.h"

#include "include/Window.h"
#include "include/Editor.h"
#include "include/Manager.h"

#include <iostream>

UILine::UILine(std::string text, int n) {
    rect = {0,0,Window::screen.w, Editor::LineHeight};
    
    this->text = "";
    texture = nullptr;
    textureRect = {0,0,0,0};

    this->n = 0;
    number = nullptr;
    numberRect = {2,0,0,0};

    setNumber(n);
    setText(text);
}

UILine::~UILine() {}

void UILine::draw() {
    Manager::Draw(number, nullptr, &numberRect);

    SDL_Rect l = {Editor::LeftMargin - 2, numberRect.y+1, 1, numberRect.h-1};
    SDL_Color c = {51,51,51,255}; 
    /// TODO: add c to theme
    Manager::DrawRect(&l, c);

    if (!text.empty())
        Manager::Draw(texture, nullptr, &textureRect);
}

void UILine::update() {
    numberRect.y = n * Editor::LineHeight + 2;
    
    textureRect.x = Editor::LeftMargin;
    textureRect.y = numberRect.y;
}

void UILine::destroy() {
    SDL_DestroyTexture(texture);
    texture = nullptr;

    SDL_DestroyTexture(number);
    number = nullptr;
}

void UILine::reload() {
    destroy();
    rect.w = Window::screen.w;
    setText(text);
    setNumber(n);
}

void UILine::setText(std::string text) {
    this->text = text;

    texture = Manager::GenerateText(text.c_str(), Window::theme.font, rect.w);
    SDL_QueryTexture(texture, NULL, NULL, &textureRect.w, &textureRect.h);
}

void UILine::setNumber(int n) {
    this->n = n;

    number = Manager::GenerateText(std::to_string(n+1).c_str(), Window::theme.font);
    SDL_QueryTexture(number, NULL, NULL, &numberRect.w, &numberRect.h);

    if (numberRect.w > Editor::LeftMargin)
        Editor::LeftMargin = numberRect.w + 4;
}

std::string UILine::getText() {
    return text;
}

int UILine::getNumber() {
    return n;
}

void UILine::insert(int p, char c) {
    text.insert(text.begin() + p, 1, c);
    setText(text);
}

void UILine::insert(int p, char* c) {
    text.insert(p, c);
    setText(text);
}

void UILine::erase(int p, int l) {
    text.erase(p,l);
    setText(text);
}

void UILine::append(std::string s) {
    text.append(s);
    setText(text);
}

int UILine::size() {
    return text.size();
}

void UILine::clear() {
    setText("");
}

int UILine::height() {
    return numberRect.h;
}
