#include "BoardGUI.hpp"

BoardGUI::BoardGUI(const std::string& FEN) : moveGen(FEN),
                                             endDrag({ROW_COUNT, ROW_COUNT}),
                                             isDragging(false),
                                             promotingIndex(ROW_COUNT*ROW_COUNT),
                                             lastMoveStart({ROW_COUNT, ROW_COUNT}),
                                             lastMoveEnd({ROW_COUNT, ROW_COUNT}),
                                             greenColor({56, 168, 50}),
                                             whiteCellColor({202, 174, 121}),
                                             blackCellColor({130, 83, 44})                
{
    setLengths();
    font = TTF_OpenFont("fonts\\Yagora.ttf", cellLen/4);
    setDragToDefault();
    setArrowToDefault();
    setAllArrowsToDefault();
    allPieces.loadFromFile("textures\\All_Chess_Pieces.png");
    allPieces.setBlendMode(SDL_BLENDMODE_BLEND);
    greenCircle.loadFromFile("textures\\Green_Circle.png");
    greenCircle.setBlendMode(SDL_BLENDMODE_BLEND);
    greenCircle.setColor(greenColor.r, greenColor.g, greenColor.b);
    greenCircle.render(0, 0);
    possibleMoveTex.createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
    possibleMoveTex.setBlendMode(SDL_BLENDMODE_BLEND);
    promotionBoardTex.createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
    promotionBoardTex.setBlendMode(SDL_BLENDMODE_BLEND);
    makeBoardTex();
    makeText();
    makePieceTex();
    makeCheckTex();
    makeLastMoveTex();
    makePromotionTex(pieceColor::white);
    makePromotionTex(pieceColor::black);
}

void BoardGUI::render()
{
    boardTex.render(corner.x, corner.y);
    text.render(0, 0);
    checkTex.render(corner.x, corner.y);
    lastMoveTex.render(corner.x, corner.y);
    pieceTex.render(corner.x, corner.y);
    possibleMoveTex.render(corner.x, corner.y);
    curArrowTex->render(corner.x, corner.y);
    allArrowsTex.render(corner.x, corner.y);
    dragTex.render(0, 0);
    promotionBoardTex.render(corner.x, corner.y);
}

void BoardGUI::handleEvent(const SDL_Event& e)
{
    SDL_Point mouse;
    auto buttons = SDL_GetMouseState(&mouse.x, &mouse.y);
    if(e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || 
      (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F11)) {
        setLengths();
        makeBoardTex();
        font = TTF_OpenFont("fonts\\Yagora.ttf", cellLen/4);
        makeCheckTex();
        makeLastMoveTex();
        for(auto& i : arrowMap) {
            for(auto& j : i.second) {
                setArrowToDefault();
                makeArrowTex(SDL_Point{static_cast<int>(i.first%ROW_COUNT), static_cast<int>(i.first/ROW_COUNT)}, 
                             SDL_Point{static_cast<int>(j.first%ROW_COUNT), static_cast<int>(j.first/ROW_COUNT)});
                arrowMap[i.first][j.first] = std::move(curArrowTex);
            }
        }
        setArrowToDefault();
        setAllArrowsToDefault();
        makeText();
        makePieceTex();
        makePromotionTex(pieceColor::white);
        makePromotionTex(pieceColor::black);
        makePromotionBoardTex(endDrag.y*ROW_COUNT+endDrag.x);
        setDragToDefault();
        possibleMoveTex.createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
        possibleMoveTex.setBlendMode(SDL_BLENDMODE_BLEND);
    }
    switch(e.type) {
        case SDL_MOUSEBUTTONDOWN:
            if(promotingIndex != ROW_COUNT*ROW_COUNT) {
                handleLeftMouseButtonDownPromoting(buttons, mouse);
            } else {
                handleLeftMouseButtonDownBoard(buttons, mouse);
                handleRightMouseButtonDown(buttons, mouse);
            }
            break;
        case SDL_MOUSEMOTION:
            if(promotingIndex != ROW_COUNT*ROW_COUNT) break;
            handleLeftMouseMotion(buttons, mouse);
            handleRightMouseMotion(buttons, mouse);
            break;
        case SDL_MOUSEBUTTONUP:
            if(promotingIndex != ROW_COUNT*ROW_COUNT) break;
            handleLeftMouseButtonUp(e, buttons, mouse);
            handleRightMouseButtonUp(e, buttons, mouse);
            break;
    }
}

void BoardGUI::handleLeftMouseButtonDownPromoting(const Uint32& buttons, const SDL_Point& mouse)
{
    int mouseRow, mouseCol;
    const auto startIndex = startDrag.y*ROW_COUNT+startDrag.x;
    const auto endIndex = endDrag.y*ROW_COUNT+endDrag.x;
    if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT) || buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
        if(getRowsAndCols(mouseRow, mouseCol, mouse)) {
            SDL_Rect queenRect;
            SDL_Rect bishopRect;
            SDL_Rect rookRect;
            SDL_Rect knightRect;
            if(promotingIndex/8 == 7) {
                queenRect   = { corner.x+(7-promotingIndex%8)*cellLen, corner.y+0*cellLen, cellLen, cellLen };
                bishopRect  = { corner.x+(7-promotingIndex%8)*cellLen, corner.y+1*cellLen, cellLen, cellLen };
                knightRect  = { corner.x+(7-promotingIndex%8)*cellLen, corner.y+2*cellLen, cellLen, cellLen };
                rookRect    = { corner.x+(7-promotingIndex%8)*cellLen, corner.y+3*cellLen, cellLen, cellLen };
            } else {
                rookRect    = { corner.x+(7-promotingIndex%8)*cellLen, corner.y+4*cellLen, cellLen, cellLen };
                knightRect  = { corner.x+(7-promotingIndex%8)*cellLen, corner.y+5*cellLen, cellLen, cellLen };
                bishopRect  = { corner.x+(7-promotingIndex%8)*cellLen, corner.y+6*cellLen, cellLen, cellLen };
                queenRect   = { corner.x+(7-promotingIndex%8)*cellLen, corner.y+7*cellLen, cellLen, cellLen };
            }
                 if(SDL_PointInRect(&mouse, &queenRect ))  moveGen.setPromoteTo(pieceType::queen );
            else if(SDL_PointInRect(&mouse, &bishopRect))  moveGen.setPromoteTo(pieceType::bishop);
            else if(SDL_PointInRect(&mouse, &rookRect  ))  moveGen.setPromoteTo(pieceType::rook  );
            else if(SDL_PointInRect(&mouse, &knightRect))  moveGen.setPromoteTo(pieceType::knight);
            else                                           moveGen.setPromoteTo(pieceType::none);
            
            promotionBoardTex.createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
            promotionBoardTex.setBlendMode(SDL_BLENDMODE_BLEND);
            promotingIndex = ROW_COUNT*ROW_COUNT;
            if(moveGen.getPromoteTo() != pieceType::none) {
                moveGen.move(startIndex, endIndex);
                lastMoveStart = startDrag;
                lastMoveEnd   = { endDrag.x, endDrag.y }; 
                makeLastMoveTex();
                makeCheckTex();
            }
            setDragToDefault();
            makePieceTex();
            endDrag = {ROW_COUNT, ROW_COUNT};
            setPossMoveToDefault();
        } 
    }
}

void BoardGUI::handleLeftMouseButtonDownBoard(const Uint32& buttons, const SDL_Point& mouse)
{
    int mouseRow, mouseCol;
    if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if(startArrow.x == ROW_COUNT && startArrow.y == ROW_COUNT) {
            arrowMap.clear();
            setAllArrowsToDefault();
        } 
        setArrowToDefault();
        if(getRowsAndCols(mouseRow, mouseCol, mouse)) {
            const auto index = mouseRow*ROW_COUNT+mouseCol;
            if(startDrag.x != ROW_COUNT && startDrag.y != ROW_COUNT && isDragging == false) {
                const SDL_Point point = { mouseCol, mouseRow };
                const auto startIndex = startDrag.y*ROW_COUNT+startDrag.x;
                if(moveGen.getColorBoard()[moveGen.getCurMoving()] & (1ULL << startIndex) && 
                   moveGen.getPromotionMap() & (1ULL << startIndex) &&
                   moveGen.getLegalMoves(startIndex) & (1ULL << index)) {
                    promotingIndex = index;
                    endDrag = { point.x, point.y };
                    possibleMoveTex.createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
                    possibleMoveTex.setBlendMode(SDL_BLENDMODE_BLEND);
                    dragTex.createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
                    dragTex.setBlendMode(SDL_BLENDMODE_BLEND);
                    makePromotionBoardTex(promotingIndex);
                } else {
                    if(moveGen.move(startIndex, index)) {
                        makePieceTex();
                        makeCheckTex();
                        lastMoveStart = startDrag;
                        lastMoveEnd   =     point; 
                        makeLastMoveTex();
                    }
                    setDragToDefault();
                    setPossMoveToDefault();
                }
            } 
            if(moveGen.getType(index) != pieceType::none &&
               moveGen.getColor(index) == moveGen.getCurMoving()) {
                startDrag = { mouseCol, mouseRow };
                isDragging = true;
                makePieceTex();
                makeDragTex(mouse);
                makePossMoveTex(index);
            }
        } 
    }
}

void BoardGUI::handleRightMouseButtonDown(const Uint32& buttons, const SDL_Point& mouse)
{
    int mouseRow, mouseCol;
    if(buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
        setPossMoveToDefault();
        setDragToDefault();
        makePieceTex();
        if((getRowsAndCols(mouseRow, mouseCol, mouse)) == false) {
            curArrowTex->createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
            curArrowTex->setBlendMode(SDL_BLENDMODE_BLEND);
        } else if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            isDragging = false;
        } else {
            startArrow =  { mouseCol, mouseRow };
            endArrow = { mouseCol, mouseRow };
            makeArrowTex(startArrow, endArrow);
        }
    } 
}

void BoardGUI::handleLeftMouseMotion(const Uint32& buttons, const SDL_Point& mouse)
{
    int mouseRow, mouseCol;
    if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT) && 
       startDrag.x != ROW_COUNT && startDrag.y != ROW_COUNT && isDragging) {
        getRowsAndCols(mouseRow, mouseCol, mouse);
        makeDragTex(mouse);
    }
}

void BoardGUI::handleRightMouseMotion(const Uint32& buttons, const SDL_Point& mouse)
{
    int mouseRow, mouseCol;
    if(buttons & SDL_BUTTON(SDL_BUTTON_RIGHT) && 
       startArrow.x != ROW_COUNT && startArrow.y != ROW_COUNT) {
        if(getRowsAndCols(mouseRow, mouseCol, mouse) == false) {
            curArrowTex->createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
            curArrowTex->setBlendMode(SDL_BLENDMODE_BLEND);
        } else {
            endArrow = { mouseCol, mouseRow };
            makeArrowTex(startArrow, endArrow);
        }
    } 
}

void BoardGUI::handleRightMouseButtonUp(const SDL_Event& e, const Uint32& buttons, const SDL_Point& mouse)
{
    int mouseRow, mouseCol;
    if(e.button.button == SDL_BUTTON_RIGHT && 
       startArrow.x != ROW_COUNT && startArrow.y != ROW_COUNT) {
        if(getRowsAndCols(mouseRow, mouseCol, mouse) == false) {
            setArrowToDefault();
        } else {
            const auto startIndex = startArrow.y * ROW_COUNT +startArrow.x;
            const auto endIndex   =   endArrow.y * ROW_COUNT +  endArrow.x;
            if(arrowMap.find(startIndex) != arrowMap.end() && 
               arrowMap[startIndex].find(endIndex) != arrowMap[startIndex].end()) {
                arrowMap[startIndex].erase(endIndex);
                if(arrowMap[startIndex].empty()) arrowMap.erase(startIndex);
                setAllArrowsToDefault();
            } else {
                allArrowsTex.setAsRenderTarget();
                curArrowTex->render(0, 0);
                SDL_SetRenderTarget(renderer, nullptr);
                arrowMap[startIndex][endIndex] = std::move(curArrowTex);
            }
            setArrowToDefault();
        }
    } 
}

void BoardGUI::handleLeftMouseButtonUp(const SDL_Event& e, const Uint32& buttons, const SDL_Point& mouse)
{
    int mouseRow, mouseCol;
    if(startDrag.x != ROW_COUNT && startDrag.y != ROW_COUNT) {
       if(getRowsAndCols(mouseRow, mouseCol, mouse)) {
            SDL_Point point = { mouseCol, mouseRow };
            if((startDrag.x != ROW_COUNT && startDrag.y != ROW_COUNT) &&
               (startDrag.x != point.x || startDrag.y != point.y)) {
                const auto startIndex = startDrag.y*ROW_COUNT+startDrag.x;
                const auto   endIndex = mouseRow*ROW_COUNT+mouseCol;
                if(moveGen.getColorBoard()[moveGen.getCurMoving()] & (1ULL << startIndex) && 
                   moveGen.getPromotionMap() & (1ULL << startIndex) && 
                   moveGen.getLegalMoves(startIndex) & (1ULL << endIndex)) {
                    promotingIndex = endIndex;
                    endDrag = { point.x, point.y };
                    possibleMoveTex.createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
                    possibleMoveTex.setBlendMode(SDL_BLENDMODE_BLEND);
                    dragTex.createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
                    dragTex.setBlendMode(SDL_BLENDMODE_BLEND);
                    makePromotionBoardTex(promotingIndex);
                } else {
                    if(moveGen.move(startIndex, endIndex)) {
                        lastMoveStart = startDrag;
                        lastMoveEnd   =     point; 
                        setPossMoveToDefault();
                        makeCheckTex();
                        makeLastMoveTex();
                    } else {
                        isDragging = false;
                    }
                    makePieceTex();
                    setDragToDefault();
                    setPossMoveToDefault();
                }
            } else {
                isDragging = false;
                makePieceTex();
                dragTex.createBlank(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_TEXTUREACCESS_TARGET);
                dragTex.setBlendMode(SDL_BLENDMODE_BLEND);
            }
        } else {
            isDragging = false;
            makePieceTex();
            setDragToDefault();
            setPossMoveToDefault();
            dragTex.createBlank(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_TEXTUREACCESS_TARGET);
            dragTex.setBlendMode(SDL_BLENDMODE_BLEND);
        }
    }
}

bool BoardGUI::getRowsAndCols(int& row, int& col, const SDL_Point& mouse)
{
    if(mouse.x < corner.x || mouse.y < corner.y) return false;
    if(mouse.x > corner.x+boardLen || mouse.y > corner.y+boardLen) return false;
    col = 7-(mouse.x-corner.x)/cellLen;
    row = 7-(mouse.y-corner.y)/cellLen;
    return true;
}

bool BoardGUI::makePromotionBoardTex(const int& sq)
{   
    if(sq/8 != 0 && sq/8 != 7) return false;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    promotionBoardTex.createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
    promotionBoardTex.setBlendMode(SDL_BLENDMODE_BLEND);
    promotionBoardTex.setAsRenderTarget();
    SDL_Rect cellRect = { 0, 0, boardLen, boardLen };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &cellRect);
    sq/8 == 7 ? promotionTex[pieceColor::white].render((7-sq%8) * cellLen, 0        ) :
                promotionTex[pieceColor::black].render((7-sq%8) * cellLen, 4*cellLen) ;
    SDL_SetRenderTarget(renderer, nullptr);
    return true;
}

void BoardGUI::makePromotionTex(const pieceColor& color)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    promotionTex[color].createBlank(cellLen, 4*cellLen, SDL_TEXTUREACCESS_TARGET);
    promotionTex[color].setBlendMode(SDL_BLENDMODE_BLEND);
    promotionTex[color].setAsRenderTarget();
    SDL_Rect cellRect = { 0, 0, cellLen, 4*cellLen };
    SDL_SetRenderDrawColor(renderer, 145, 145, 145, 200);
    SDL_RenderFillRect(renderer, &cellRect);
    const auto curMoving = moveGen.getCurMoving();
    if(color == pieceColor::white) {
        renderPiece(0, 0, pieceType::queen, pieceColor::white);
        renderPiece(0, cellLen, pieceType::bishop, pieceColor::white);
        renderPiece(0, 2*cellLen, pieceType::knight, pieceColor::white);
        renderPiece(0, 3*cellLen, pieceType::rook, pieceColor::white);
    } else {
        renderPiece(0, 3*cellLen, pieceType::queen, pieceColor::black);
        renderPiece(0, 2*cellLen, pieceType::bishop, pieceColor::black);
        renderPiece(0, cellLen, pieceType::knight, pieceColor::black);
        renderPiece(0, 0, pieceType::rook, pieceColor::black);
    }
    SDL_SetRenderTarget(renderer, nullptr);
}

void BoardGUI::makeBoardTex()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    boardTex.createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
    boardTex.setAsRenderTarget();
    for(int row = 0; row < ROW_COUNT; row++) {
        for(int col = 0; col < ROW_COUNT; col++) {
            SDL_Rect cellRect = { col*cellLen, row*cellLen, cellLen, cellLen };
            if((row+col)%2 == 0) 
                SDL_SetRenderDrawColor(renderer, whiteCellColor.r, whiteCellColor.g, whiteCellColor.b, 255);
            else
                SDL_SetRenderDrawColor(renderer, blackCellColor.r, blackCellColor.g, blackCellColor.b, 255);
            SDL_RenderFillRect(renderer, &cellRect);
        }
    }
    SDL_SetRenderTarget(renderer, nullptr);
}

void BoardGUI::makeCheckTex()
{
    std::cout << "pos1\n";
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    checkTex.createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
    checkTex.setBlendMode(SDL_BLENDMODE_BLEND);
    checkTex.setAsRenderTarget();
    const auto kingCheck = moveGen.isInCheck();
    if(kingCheck != ROW_COUNT*ROW_COUNT) {
        SDL_Rect cellRect = { boardLen-cellLen-(kingCheck%ROW_COUNT)*cellLen, 
                              boardLen-cellLen-(kingCheck/ROW_COUNT)*cellLen, cellLen, cellLen };
        SDL_SetRenderDrawColor(renderer, 250, 12, 0, 200);
        SDL_RenderFillRect(renderer, &cellRect);
    }   
    SDL_SetRenderTarget(renderer, nullptr);
}

void BoardGUI::makeLastMoveTex()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    lastMoveTex.createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
    lastMoveTex.setBlendMode(SDL_BLENDMODE_BLEND);
    lastMoveTex.setAsRenderTarget();
    if(lastMoveStart.x != ROW_COUNT || lastMoveStart.y != ROW_COUNT ||
       lastMoveEnd.x   != ROW_COUNT || lastMoveEnd.y   != ROW_COUNT) {
        SDL_Rect cellRect = { boardLen-cellLen-(lastMoveStart.x)*cellLen, 
                              boardLen-cellLen-(lastMoveStart.y)*cellLen, cellLen, cellLen };
        SDL_SetRenderDrawColor(renderer, greenColor.r, greenColor.g, greenColor.b, 130);
        SDL_RenderFillRect(renderer, &cellRect);
        cellRect = { boardLen-cellLen-(lastMoveEnd.x)*cellLen, 
                     boardLen-cellLen-(lastMoveEnd.y)*cellLen, cellLen, cellLen };
        SDL_RenderFillRect(renderer, &cellRect);
    }
    SDL_SetRenderTarget(renderer, nullptr);
}

void BoardGUI::makeText()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    text.createBlank(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_TEXTUREACCESS_TARGET);
    text.setBlendMode(SDL_BLENDMODE_BLEND);
    text.setAsRenderTarget();
    const int distance = 0.035*std::min(WINDOW_HEIGHT, WINDOW_WIDTH);
    for(char curLetter = 'a'; curLetter < 'a'+ROW_COUNT; curLetter++) {
        std::string textString(1, curLetter);
        Texture curTexture;
        curTexture.createBlank(cellLen, cellLen);
        curTexture.setBlendMode(SDL_BLENDMODE_BLEND);
        if((curLetter-'a')%2)   curTexture.loadText(textString, font, 
                                    { blackCellColor.r, blackCellColor.g, blackCellColor.b, 255 });
        else                    curTexture.loadText(textString, font, 
                                    { whiteCellColor.r, whiteCellColor.g, whiteCellColor.b, 255 });
        curTexture.render(corner.x+(curLetter-'a')*cellLen+cellLen/15, 
                          corner.y+boardLen-0.035*std::min(WINDOW_HEIGHT, WINDOW_WIDTH));
    }
    for(char curNum = 1; curNum < 1+ROW_COUNT; curNum++) {
        std::string textString(1, curNum);
        Texture curTexture;
        curTexture.createBlank(cellLen, cellLen);
        curTexture.setBlendMode(SDL_BLENDMODE_BLEND);
        if((ROW_COUNT-curNum)%2)    curTexture.loadText(std::to_string(curNum), font, 
                                        { blackCellColor.r, blackCellColor.g, blackCellColor.b, 255 });
        else                        curTexture.loadText(std::to_string(curNum), font, 
                                        { whiteCellColor.r, whiteCellColor.g, whiteCellColor.b, 255 });
        curTexture.render(corner.x+boardLen-0.025*std::min(WINDOW_HEIGHT, WINDOW_WIDTH), 
                          corner.y+(ROW_COUNT-curNum)*cellLen+cellLen/15);
    }
    SDL_SetRenderTarget(renderer, nullptr);
    return;
}

void BoardGUI::makePossMoveTex(const int& sq)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    possibleMoveTex.createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
    possibleMoveTex.setBlendMode(SDL_BLENDMODE_BLEND);
    possibleMoveTex.setAsRenderTarget();
    const auto mask = moveGen.getLegalMoves(sq);
    for(int row = 0; row < ROW_COUNT; row++) {
        for(int col = 0; col < ROW_COUNT; col++) {
            const auto index = 63-(col*ROW_COUNT+row);
            if(mask & (1ULL << index)) greenCircle.render(row*cellLen+cellLen/3, col*cellLen+cellLen/3, cellLen/3, cellLen/3);
        }
    }
    possibleMoveTex.setAlpha(220);
    SDL_SetRenderTarget(renderer, nullptr);
}

void BoardGUI::makeArrowTex(SDL_Point start, SDL_Point end)
{
    if(start.x < 0 || end.x < 0) return;
    if(start.x > 7 || end.x > 7) return;
    if(start.y < 0 || end.y < 0) return;
    if(start.y > 7 || end.y > 7) return;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    curArrowTex = std::make_unique<Texture>();
    curArrowTex->createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
    curArrowTex->setBlendMode(SDL_BLENDMODE_BLEND);
    if(start.x == end.x && start.y == end.y) {
        curArrowTex->setAsRenderTarget();
        greenCircle.render((7-start.x)*cellLen, (7-start.y)*cellLen, cellLen, cellLen);
        SDL_SetRenderTarget(renderer, nullptr);
        curArrowTex->setAlpha(200);
        return;
    }
    const long double angle = atan2((start.y-end.y), (start.x-end.x)) * 180/3.14159265358979323846;
    const int distance = std::round(cellLen*sqrt(((start.y-end.y)*(start.y-end.y) + 
                                                  (start.x-end.x)*(start.x-end.x))));
    const int thickness = cellLen/6;
    Texture newArrow;
    newArrow.createBlank(distance, 5*thickness, SDL_TEXTUREACCESS_TARGET);
    newArrow.setBlendMode(SDL_BLENDMODE_BLEND);
    newArrow.setAsRenderTarget();
    const auto bodyRatio = 3*thickness;
    const auto radius = cellLen/3;
    const SDL_Rect base = { radius, (newArrow.getHeight()-thickness)/2, distance-bodyRatio-radius, thickness };
    SDL_SetRenderDrawColor(renderer, greenColor.r, greenColor.g, greenColor.b, 200);
    SDL_RenderFillRect(renderer, &base);
    const std::vector<SDL_Vertex> headVerts {
        { SDL_FPoint{ distance-bodyRatio, thickness/2 }, 
            SDL_Color{ greenColor.r, greenColor.g, greenColor.b, 200 }, SDL_FPoint{ 0 } },
        { SDL_FPoint{ distance, newArrow.getHeight()/2 }, 
            SDL_Color{ greenColor.r, greenColor.g, greenColor.b, 200 }, SDL_FPoint{ 0 } },
        { SDL_FPoint{ distance-bodyRatio, newArrow.getHeight()-thickness/2 }, 
            SDL_Color{ greenColor.r, greenColor.g, greenColor.b, 200 }, SDL_FPoint{ 0 } }
    };
    SDL_RenderGeometry(renderer, nullptr, headVerts.data(), headVerts.size(), nullptr, 0);
    curArrowTex->setAsRenderTarget();
    SDL_Point center = { 0, newArrow.getHeight()/2 };
    newArrow.render((7-start.x)*cellLen+cellLen/2, (7-start.y)*cellLen+cellLen/2-newArrow.getHeight()/2, -1, -1, nullptr, angle, &center);
    SDL_SetRenderTarget(renderer, nullptr);
    return;
}

void BoardGUI::renderPiece(const int& x, const int& y, const pieceType& type, const pieceColor& color)
{
    if(type == pieceType::none || color == pieceColor::none) return;
    SDL_Rect clip;
    switch(type) {
        case pieceType::king:
            clip = { 0, 0, 200, 200 };
            break;
        case pieceType::queen:
            clip = { 200, 0, 200, 200 };
            break;
        case pieceType::bishop:
            clip = { 400, 0, 200, 200 };
            break;
        case pieceType::knight:
            clip = { 600, 0, 200, 200 };
            break;
        case pieceType::rook:
            clip = { 800, 0, 200, 200 };
            break;
        case pieceType::pawn:
            clip = { 1000, 0, 200, 200 };
            break;
    }
    if(color == pieceColor::black) clip.y = 200;
    allPieces.render(x, y, cellLen, cellLen, &clip);
    return;
}

void BoardGUI::makePieceTex() 
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    pieceTex.createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
    pieceTex.setBlendMode(SDL_BLENDMODE_BLEND);
    pieceTex.setAsRenderTarget();
    for(int row = 0; row < ROW_COUNT; row++) {
        for(int col = 0; col < ROW_COUNT; col++) {
            const auto index = row*ROW_COUNT+col;
            if(moveGen.getType(index) != pieceType::none) {
                if(row == startDrag.y && col == startDrag.x && isDragging) {
                    allPieces.setAlpha(100);
                    SDL_Rect cellRect = { boardLen-cellLen-col*cellLen, boardLen-cellLen-row*cellLen, cellLen, cellLen };
                    SDL_SetRenderDrawColor(renderer, greenColor.r, greenColor.g, greenColor.b, 150);
                    SDL_RenderFillRect(renderer, &cellRect);
                } else {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                }
                renderPiece(boardLen-cellLen-col*cellLen, boardLen-cellLen-row*cellLen, moveGen.getType(index), moveGen.getColor(index));
                allPieces.setAlpha(255);
            }
        }
    }
    SDL_SetRenderTarget(renderer, nullptr);
    return;
}

void BoardGUI::makeDragTex(const SDL_Point& mouse)
{
    if(startDrag.x == ROW_COUNT || startDrag.y == ROW_COUNT) return;
    dragTex.createBlank(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_TEXTUREACCESS_TARGET);
    dragTex.setBlendMode(SDL_BLENDMODE_BLEND);
    const auto type = moveGen.getType(startDrag.y*ROW_COUNT+startDrag.x);
    const auto color = moveGen.getColor(startDrag.y*ROW_COUNT+startDrag.x);
    if(type == pieceType::none) return;
    dragTex.setAsRenderTarget();
    renderPiece(mouse.x-cellLen/2, mouse.y-cellLen/2, type, color);
    allPieces.setAlpha(255);
    SDL_SetRenderTarget(renderer, nullptr);
    return;
}

void BoardGUI::setLengths()
{
    const float percent = 10;
    const auto minLen = std::min(WINDOW_HEIGHT, WINDOW_WIDTH);
    const int percentLen = percent/100*minLen;
    boardLen = std::round(static_cast<float>(minLen-percentLen)/ROW_COUNT)*ROW_COUNT;
    cellLen = boardLen/ROW_COUNT;
    if(WINDOW_HEIGHT > WINDOW_WIDTH) corner = {percentLen/2, static_cast<int>(WINDOW_HEIGHT-WINDOW_WIDTH+percentLen)/2};
    else corner = {static_cast<int>(WINDOW_WIDTH-WINDOW_HEIGHT+percentLen)/2, percentLen/2};
}

void BoardGUI::setDragToDefault()
{
    startDrag = { ROW_COUNT, ROW_COUNT };
    dragTex.createBlank(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_TEXTUREACCESS_TARGET);
    dragTex.setBlendMode(SDL_BLENDMODE_BLEND);
}

void BoardGUI::setArrowToDefault()
{
    startArrow = { ROW_COUNT, ROW_COUNT };
    endArrow = { ROW_COUNT, ROW_COUNT };
    curArrowTex = std::make_unique<Texture>();
    curArrowTex->createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
    curArrowTex->setBlendMode(SDL_BLENDMODE_BLEND);
    curArrowTex->setAlpha(220);
}

void BoardGUI::setAllArrowsToDefault()
{
    allArrowsTex.createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
    allArrowsTex.setBlendMode(SDL_BLENDMODE_BLEND);
    allArrowsTex.setAsRenderTarget();
    allArrowsTex.setAlpha(200);
    for(auto& i : arrowMap) 
        for(auto& j : i.second) 
            j.second->render(0, 0);
    SDL_SetRenderTarget(renderer, nullptr);
}

void BoardGUI::setPossMoveToDefault()
{
    possibleMoveTex.createBlank(boardLen, boardLen, SDL_TEXTUREACCESS_TARGET);
    possibleMoveTex.setBlendMode(SDL_BLENDMODE_BLEND);
    isDragging = false; 
}