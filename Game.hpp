#ifndef GAME_H
#define GAME_H

#pragma once

#include <memory>
#include "BoardGUI.cpp"

class Game
{
    private:
        BoardGUI boardInt;
    public:
        Game(const std::string& FEN) : boardInt(FEN) { }
        ~Game() { }
        void render() { boardInt.render(); }
        void handleEvent(const SDL_Event& e) { boardInt.handleEvent(e); }
};

#endif