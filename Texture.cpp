#include "Texture.hpp"

bool Texture::loadFromFile(const std::string& path)
{
    free();
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface) {
        SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat( loadedSurface, SDL_PIXELFORMAT_RGBA8888, 0 );
        if(formattedSurface) {
            newTexture = SDL_CreateTextureFromSurface(renderer, formattedSurface);
            if(newTexture) {
                tWidth = loadedSurface->w;
                tHeight = loadedSurface->h;
            }
            SDL_FreeSurface(formattedSurface);
        }
        SDL_FreeSurface(loadedSurface);
    }
    texture = newTexture;
    return texture;
}

bool Texture::createBlank(const int& width, const int& height, 
                          const SDL_TextureAccess& access)
{
    free();
    texture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), access, width, height);
    if(texture) {
        tWidth = width;
        tHeight = height;
    }
    return texture;
}

bool Texture::loadText(const std::string& text, TTF_Font* font, const SDL_Color& color)
{
    free();
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if(textSurface) {
        texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if(texture) {
            tWidth = textSurface->w;
            tHeight = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
    }
    return texture;
}

void Texture::free()
{
    if(texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
        tWidth = 0;
        tHeight = 0;
    }
}


void Texture::render(const int& x, const int& y, const int& w, const int& h, const SDL_Rect* clip, 
                     const long double& angle, const SDL_Point* center, const SDL_RendererFlip& flip)
{
    SDL_Rect dest = { x, y, tWidth, tHeight };
    if(w != -1 && h != -1) {
        dest.w = w;
        dest.h = h;
    } else if(clip) {
        dest.w = clip->w;
        dest.h = clip->h;
    }
    SDL_RenderCopyEx(renderer, texture, clip, &dest, angle, center, flip);
}