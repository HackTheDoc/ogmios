#pragma once

#include "UILabel.h"

class UILine : public UIElement {
private:
    std::string text;
    UILabel* lbl_text;

    int number;
    UILabel* lbl_number;

public:
    UILine(const std::string& t, int n);
    ~UILine();

    void reload();

    void drawAt(const int y);
    void draw() override;
    void update() override;
    void destroy() override;

    void setText(const std::string& t);
    void setNumber(const int n);

    void insert(const int p, const char c);
    void insert(const int p, const char* c);
    void erase(const int p, const int l);
    void append(const std::string& s);
    int size();
    void clear();

    int y();
    int height();
    int getNumber();
    std::string getText();

    void useAsMargin();
};
