#pragma once

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Manager {
public:
    static const int DEFAULT_FONT_SIZE;
    static const int MIN_FONT_SIZE;
    static const int MAX_FONT_SIZE;

    static TTF_Font* font;
    
    /// @brief Set the viewport to use. Give a nullptr viewport pointer to reset it to the full window
    /// @param viewport pointer to the viewport to use (reset: nullptr)
    static void SetViewport(SDL_Rect* viewport);

    /* -----  COLOR MANAGER  ----- */

    /// @brief Set renderer draw color
    /// @param r red
    /// @param g green
    /// @param b blue
    /// @param a alpha (default: 255)
    /// @return old draw color of the renderer
    static SDL_Color SetRenderDrawColor(int r, int g, int b, int a = 255);

    /// @brief Set renderer draw color
    /// @param c color to set
    /// @return old draw color of the renderer
    static SDL_Color SetRenderDrawColor(SDL_Color c);

    /* ----- TEXTURE MANAGER ----- */

    /// @brief Load an existing texture
    /// @param filepath path to the texture to load
    /// @return the loaded texture or a null pointer
    static SDL_Texture* LoadTexture(const char* filepath);
    
    /// @brief Generate a text texture
    /// @param text text to generate
    /// @param color color to use
    /// @return pointer to the generated texture (nullptr if an error happen)
    static SDL_Texture* GenerateText(const char* text, SDL_Color color);
    
    /// @brief Generate a text texture
    /// @param text text to generate
    /// @param color color to use
    /// @param length max length of the texture (in pixels)
    /// @return pointer to the generated texture (nullptr if an error happen)
    static SDL_Texture* GenerateText(const char* text, SDL_Color color, int length);

    /// @brief Draw a texture on the window
    /// @param texture texture to draw
    /// @param src source rectangle on the texture
    /// @param dest destination rectangle on the window
    /// @param flip flip mode (default: none)
    static void Draw(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest, SDL_RendererFlip flip = SDL_FLIP_NONE);

    /// @brief Draw a rectangle on the window
    /// @param rect rectangle to draw
    /// @param color name of the rectangle's color
    static void DrawRect(SDL_Rect* rect, SDL_Color color);

    /// @brief Draw a filled rectangle on the window
    /// @param rect rectangle to draw
    /// @param color name of the rectangle's color
    static void DrawFilledRect(SDL_Rect* rect, SDL_Color color);

    /// @brief Draw a rectangle border on the window (multiple pixels width)
    /// @param rect border to draw
    /// @param color name of the border's color
    static void DrawBorder(SDL_Rect* rect, SDL_Color color);

    /* ----- FONT MANAGER ------ */
    /// @brief Open and configure the font
    static void OpenFont(const char* filepath);

    /// @brief Set the font size
    /// @param size new size in pixels
    static void SetFontSize(int size);

    /// @brief Just close the font lol
    static void CloseFont();
};
