#ifndef TEXTURE_H
#define TEXTURE_H

#pragma once

#include <stdint.h>
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Chess.hpp"

using index_t = std::pair<size_t, size_t>;

constexpr int ROW_COUNT = 8;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

int WINDOW_HEIGHT = 720;
int WINDOW_WIDTH = 1280;

class Texture
{
    private:
        SDL_Texture* texture;
        int tWidth;
        int tHeight;

    public:
        Texture() : texture(nullptr), tWidth(0), tHeight(0) { }
        ~Texture() { free(); }
        bool loadFromFile(const std::string& path);
        bool createBlank(const int& width, const int& height, const SDL_TextureAccess& access = SDL_TEXTUREACCESS_STATIC);
        bool loadText(const std::string& text, TTF_Font* font, const SDL_Color& color);
        void free();
        void setBlendMode(const SDL_BlendMode& blending) { SDL_SetTextureBlendMode(texture, blending); }
        void setAlpha(const Uint8& alpha) { SDL_SetTextureAlphaMod(texture, alpha); }
        void setColor(const Uint8& red, const Uint8& green, const Uint8& blue) { SDL_SetTextureColorMod(texture, red, green, blue); }
        void render(const int& x, const int& y, const int& w = -1, const int& h = -1, const SDL_Rect* clip = nullptr, 
                    const long double& angle = 0.0, const SDL_Point* center = nullptr, const SDL_RendererFlip& flip = SDL_FLIP_NONE);
        void setAsRenderTarget() { SDL_SetRenderTarget(renderer, texture); }
        int getWidth() { return tWidth; }
        int getHeight() { return tHeight; } 
};

#endif