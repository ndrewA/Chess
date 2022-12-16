
#include "Game.cpp"

bool init()
{
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        success = false;
    } else {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                  WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        SDL_SetWindowMinimumSize(window, 350, 350);
        if(window == nullptr) {
            success = false;
        } else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED  | 
                                                      SDL_RENDERER_TARGETTEXTURE | 
                                                      SDL_RENDERER_PRESENTVSYNC );
            if(renderer == nullptr) {
                success = false;
            } else if(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) < 0) {
                success = false;
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                int imgFlags = IMG_INIT_PNG;
                if( !(IMG_Init(imgFlags) & imgFlags) ) 
                    success= false;
                if( TTF_Init() == -1 )
                    success = false;
            }
        }
    }
    return success;
}

void close()
{
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = nullptr;
	renderer = nullptr;
	IMG_Quit();
	SDL_Quit();
}
/*


#include "MoveGeneratorBitset.cpp"
#include <string>
std::unordered_map<std::string, size_t> freq;

#include <chrono>

long double totalLen = 0.0;

size_t perft(const int depth, const int& limit, MoveGeneratorBitset moveGen)
{
    if(depth >= limit) return 1;
    auto start = std::chrono::high_resolution_clock::now();
    //std::cout << "depth = " << depth << '\n';
    const auto gameRes = moveGen.checkResult();
    if(gameRes != 2 && gameRes != 0) return 0;
    const auto promMap = moveGen.getPromotionMap();
    auto colorBoard = moveGen.getColorBoard()[moveGen.getCurMoving()];
    size_t result = 0;
    while(colorBoard) {
        const auto fromSq = moveGen.getIndexLS1B(colorBoard);
        colorBoard ^= (1ULL << fromSq);
        if(promMap & (1ULL << fromSq));
        if(promMap & (1ULL << fromSq)) {
            for(int curType = 1; curType <= 4; curType++) {
                auto lMoves = moveGen.getLegalMoves(fromSq);
                while(lMoves) {
                    const auto toSq = moveGen.getIndexLS1B(lMoves);
                    lMoves ^= (1ULL << toSq);
                    auto newMoveGen(moveGen);
                    newMoveGen.setPromoteTo(static_cast<pieceType>(curType));
                    newMoveGen.move(fromSq, toSq);
                    const auto newPerft = perft(depth+1, limit, newMoveGen);
                    result += newPerft;
                }
            }
        } else {
            auto lMoves = moveGen.getLegalMoves(fromSq);
            while(lMoves) {
                const auto toSq = moveGen.getIndexLS1B(lMoves);
                lMoves ^= (1ULL << toSq);
                auto newMoveGen(moveGen);
                newMoveGen.move(fromSq, toSq);
                const auto newPerft = perft(depth+1, limit, newMoveGen);
                result += newPerft;
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    totalLen += static_cast<long double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count())/1000000000;
    return result;
}

size_t dividePerft(const int depth, const int& limit, MoveGeneratorBitset moveGen)
{
    if(depth >= limit) return 1;
    const auto gameRes = moveGen.checkResult();
    if(gameRes != 2 && gameRes != 0) return 0;
    const auto promMap = moveGen.getPromotionMap();
    auto colorBoard = moveGen.getColorBoard()[moveGen.getCurMoving()];
    while(colorBoard) {
        const auto fromSq = moveGen.getIndexLS1B(colorBoard);
        colorBoard ^= (1ULL << fromSq);
        if(promMap & (1ULL << fromSq));
        if(promMap & (1ULL << fromSq)) {
            for(int curType = 1; curType <= 4; curType++) {
                auto lMoves = moveGen.getLegalMoves(fromSq);
                while(lMoves) {
                    const auto toSq = moveGen.getIndexLS1B(lMoves);
                    lMoves ^= (1ULL << toSq);
                    auto newMoveGen(moveGen);
                    newMoveGen.setPromoteTo(static_cast<pieceType>(curType));
                    newMoveGen.move(fromSq, toSq);
                    std::string str;
                    str += 'h'-fromSq%8;
                    str += '0'+fromSq/8+1;
                    str += 'h'-toSq%8;
                    str += '0'+toSq/8+1;
                    if(curType == 1) str += 'q';
                    else if(curType == 2) str += 'b';
                    else if(curType == 3) str +=  'n';
                    else str += 'r';
                    str += ": ";
                    //std::cout << "str = " << str << '\n';
                    //std::cout << "from  = "<< fromSq/8 << ' ' << fromSq%8 << " to = " << toSq/8 << ' ' << toSq%8 << '\n';
                    freq[str] += perft(depth+1, limit, newMoveGen);
                }
            }
        } else {
            auto lMoves = moveGen.getLegalMoves(fromSq);
            while(lMoves) {
                const auto toSq = moveGen.getIndexLS1B(lMoves);
                lMoves ^= (1ULL << toSq);
                auto newMoveGen(moveGen);
                newMoveGen.move(fromSq, toSq);
                std::string str;
                str += 'h'-fromSq%8;
                str += '0'+fromSq/8+1;
                str += 'h'-toSq%8;
                str += '0'+toSq/8+1;
                str += ": ";
                //std::cout << "str = " << str << '\n';
                //std::cout << "from  = "<< fromSq/8 << ' ' << fromSq%8 << " to = " << toSq/8 << ' ' << toSq%8 << '\n';
                freq[str] += perft(depth+1, limit, newMoveGen);
            }
        }
    }
    size_t result = 0;
    for(const auto& i : freq) 
        result += i.second;
    return result;
}

int main()
{
    std::cout << "before\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto res = perft(0, 4, MoveGeneratorBitset("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1"));
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "====================\n";
    std::cout << "NPS = " << static_cast<long double>(res)/static_cast<long double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count())*1000000000 << '\n';
    std::cout << "res = " << res << '\n';
    std::cout << "time = " << static_cast<long double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count())/1000000000 << "\n\n";
    std::cout << "---------------\n";
    for(const auto& i : freq)
        std::cout << i.first << i.second << '\n';
    std::cout << "totalLen = " << totalLen << '\n';
    std::cout << "END\n";

    return 0;
}

*/
int main(int argv, char** args)
{
    if(init()) {
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
        bool isFullscreen = false;
        Game game("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        bool quit = false;
        SDL_Event e;
        while(quit == false) {
            while(SDL_PollEvent(&e) != 0) {
                if(e.type == SDL_QUIT){
                    quit = true;
                }
                switch(e.window.event) {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    {
                        WINDOW_WIDTH = e.window.data1;
                        WINDOW_HEIGHT = e.window.data2;
                    } 
                }
                if(e.type == SDL_KEYDOWN) {
                    switch(e.key.keysym.sym) {
                        case SDLK_F11:
                        {
                            isFullscreen ? SDL_SetWindowFullscreen(window, SDL_FALSE) :
                                           SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                            isFullscreen = !isFullscreen;
                        }
                    }
                }
                game.handleEvent(e);
            }
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            const SDL_Rect background = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
            SDL_SetRenderDrawColor(renderer, 22, 21, 18, 255);
            SDL_RenderFillRect(renderer, &background);
            game.render();
            SDL_RenderPresent(renderer);
        }
    }
    close();
    return 0;
}