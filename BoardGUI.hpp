#ifndef BOARDGUI_H
#define BOARDGUI_H

#pragma once

#include "Texture.cpp"
#include "MoveGeneratorBitset.cpp"
#include <vector>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <memory>

class BoardGUI
{
    private:
        std::unordered_map<int, std::unordered_map<int, std::unique_ptr<Texture>>> arrowMap;
        std::unique_ptr<Texture> curArrowTex;
        MoveGeneratorBitset moveGen;
        Texture allPieces;
        Texture checkTex;
        Texture boardTex;
        Texture text;
        Texture allArrowsTex;
        Texture pieceTex;
        Texture lastMoveTex;
        Texture dragTex;
        Texture possibleMoveTex;
        std::unordered_map<pieceColor, Texture> promotionTex;
        Texture promotionBoardTex;
        Texture greenCircle;
        SDL_Point corner;
        SDL_Point startDrag;
        SDL_Point endDrag;
        SDL_Point startArrow;
        SDL_Point endArrow;
        SDL_Point lastMoveStart;
        SDL_Point lastMoveEnd;
        const SDL_Color greenColor;
        const SDL_Color whiteCellColor;
        const SDL_Color blackCellColor;
        TTF_Font* font;
        bool isDragging;
        int promotingIndex;
        int boardLen;
        int cellLen;

    public:
        BoardGUI(const std::string& FEN);
        ~BoardGUI() { TTF_CloseFont(font); font = nullptr; }
        void render();
        void handleEvent(const SDL_Event& e);

    private:
        void handleLeftMouseButtonDownBoard(const Uint32& buttons, const SDL_Point& mouse);
        void handleLeftMouseButtonDownPromoting(const Uint32& buttons, const SDL_Point& mouse);
        void handleRightMouseButtonDown(const Uint32& buttons, const SDL_Point& mouse);
        void handleLeftMouseMotion(const Uint32& buttons, const SDL_Point& mouse);
        void handleRightMouseMotion(const Uint32& buttons, const SDL_Point& mouse);
        void handleRightMouseButtonUp(const SDL_Event& e, const Uint32& buttons, const SDL_Point& mouse);
        void handleLeftMouseButtonUp(const SDL_Event& e, const Uint32& buttons, const SDL_Point& mouse);

        bool getRowsAndCols(int& row, int& col, const SDL_Point& mouse);

        void decodeFEN(const std::string& FEN);
        bool makePromotionBoardTex(const int& sq);
        void makePromotionTex(const pieceColor& color);
        void makeBoardTex();

        void makeText();
        void makeCheckTex();
        void makeLastMoveTex();
        void makeArrowTex(SDL_Point start, SDL_Point end);
        void makePossMoveTex(const int& sq);
        void makePieceTex();
        void makeDragTex(const SDL_Point& mouse);

        void renderPiece(const int& x, const int& y, const pieceType& type, const pieceColor& color);

        void setLengths();
        void setDragToDefault();
        void setArrowToDefault();
        void setAllArrowsToDefault();
        void setPossMoveToDefault();
};

#endif