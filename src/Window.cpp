#include "include/Window.h"

#include <iostream>

const int Window::MIN_WIDTH = 384;
const int Window::MIN_HEIGHT = 128;
const int Window::DEFAULT_WIDTH = 800;
const int Window::DEFAULT_HEIGHT = 600;
const std::string Window::TITLE = "Ogmios";

bool Window::isRunning = false;
SDL_Renderer* Window::renderer = nullptr;
SDL_Rect Window::screen = {0, 0, Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT};
Manager Window::manager;
Event Window::event;
Theme Window::theme;

UI* Window::ui = nullptr;
Editor* Window::editor = nullptr;

void Window::Reload() {
    ui->reload();
    editor->reload();
}

Window::Window() : window(nullptr) {}

Window::~Window() {}

int Window::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << SDL_GetError() << std::endl;
        return -1;
    }

    if (TTF_Init() != 0) {
        std::cerr << SDL_GetError() << std::endl;
        return -2;
    }

    window = SDL_CreateWindow(
        TITLE.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screen.w,
        screen.h,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        std::cerr << "error creating the window" << std::endl;
        return -3;
    }
    SDL_SetWindowMinimumSize(window, Window::MIN_WIDTH, Window::MIN_HEIGHT);
/*
    SDL_Surface* icon = IMG_Load("icons/Ogmios.png");
    SDL_SetWindowIcon(window, icon);*/

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "error creating the renderer" << std::endl;
        return -4;
    }  
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    event.linkTo(this);

    theme.load(Theme::Type::DAY);

    Manager::OpenFont("./fonts/Nunito-Regular.ttf");

    ui = new UI(screen.w, 30);
    ui->init();
    ui->place(0,0);

    editor = new Editor(screen.w, screen.h - ui->height());
    editor->init();
    editor->place(0,ui->height());

    Manager::SetRenderDrawColor(theme.textBackground);

    SDL_StartTextInput();

    isRunning = true;

    return 0;
}

void Window::handleEvents() {
    SDL_PollEvent(&event.e);

    switch (event.e.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        event.handleKeyboardInputs();
        break;
    }
}

void Window::update() {
    ui->update();
    editor->update();
}

void Window::render() {
    SDL_RenderClear(renderer);

    editor->render();
    ui->render();

    SDL_RenderPresent(renderer);
}

void Window::kill() {
    Manager::CloseFont();

    ui->destroy();
    delete ui;
    ui = nullptr;

    editor->destroy();
    delete editor;
    editor = nullptr;

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_Quit();
}

void Window::switchTheme() {
    theme.load(theme.next);

    Reload();

    Manager::SetRenderDrawColor(theme.textBackground);
}

void Window::resize(int w, int h) {
    screen.w = w;
    screen.h = h;
    
    ui->setWidth(w);
    editor->setWidth(w);
}
