#include "MoveGeneratorBitset.hpp"
#include <iostream>

std::unordered_map<pieceType, std::vector<uint64_t>> MoveGeneratorBitset::magic = 
    {{ pieceType::rook, {
        0xa8002c000108020ULL    , 0x6c00049b0002001ULL  , 0x100200010090040ULL  , 0x2480041000800801ULL , 0x280028004000800ULL  ,
        0x900410008040022ULL    , 0x280020001001080ULL  , 0x2880002041000080ULL , 0xa000800080400034ULL , 0x4808020004000ULL    ,
        0x2290802004801000ULL   , 0x411000d00100020ULL  , 0x402800800040080ULL  , 0xb000401004208ULL    , 0x2409000100040200ULL ,
        0x1002100004082ULL      , 0x22878001e24000ULL   , 0x1090810021004010ULL , 0x801030040200012ULL  , 0x500808008001000ULL  ,
        0xa08018014000880ULL    , 0x8000808004000200ULL , 0x201008080010200ULL  , 0x801020000441091ULL  , 0x800080204005ULL     ,
        0x1040200040100048ULL   , 0x120200402082ULL     , 0xd14880480100080ULL  , 0x12040280080080ULL   , 0x100040080020080ULL  , 
        0x9020010080800200ULL   , 0x813241200148449ULL  , 0x491604001800080ULL  , 0x100401000402001ULL  , 0x4820010021001040ULL ,    
        0x400402202000812ULL    , 0x209009005000802ULL  , 0x810800601800400ULL  , 0x4301083214000150ULL , 0x204026458e001401ULL ,
        0x40204000808000ULL     , 0x8001008040010020ULL , 0x8410820820420010ULL , 0x1003001000090020ULL , 0x804040008008080ULL  ,
        0x12000810020004ULL     , 0x1000100200040208ULL , 0x430000a044020001ULL , 0x280009023410300ULL  , 0xe0100040002240ULL   ,
        0x200100401700ULL       , 0x2244100408008080ULL , 0x8000400801980ULL    , 0x2000810040200ULL    , 0x8010100228810400ULL ,    
        0x2000009044210200ULL   , 0x4080008040102101ULL , 0x40002080411d01ULL   , 0x2005524060000901ULL , 0x502001008400422ULL  ,
        0x489a000810200402ULL   , 0x1004400080a13ULL    , 0x4000011008020084ULL , 0x26002114058042ULL   ,
    }}, { pieceType::bishop, {
        0x89a1121896040240ULL   , 0x2004844802002010ULL , 0x2068080051921000ULL , 0x62880a0220200808ULL , 0x4042004000000ULL    ,
        0x100822020200011ULL    , 0xc00444222012000aULL , 0x28808801216001ULL   , 0x400492088408100ULL  , 0x201c401040c0084ULL  ,
        0x840800910a0010ULL     , 0x82080240060ULL      , 0x2000840504006000ULL , 0x30010c4108405004ULL , 0x1008005410080802ULL ,
        0x8144042209100900ULL   , 0x208081020014400ULL  , 0x4800201208ca00ULL   , 0xf18140408012008ULL  , 0x1004002802102001ULL ,
        0x841000820080811ULL    , 0x40200200a42008ULL   , 0x800054042000ULL     , 0x88010400410c9000ULL , 0x520040470104290ULL  ,
        0x1004040051500081ULL   , 0x2002081833080021ULL , 0x400c00c010142ULL    , 0x941408200c002000ULL , 0x658810000806011ULL  ,
        0x188071040440a00ULL    , 0x4800404002011c00ULL , 0x104442040404200ULL  , 0x511080202091021ULL  , 0x4022401120400ULL    ,
        0x80c0040400080120ULL   , 0x8040010040820802ULL , 0x480810700020090ULL  , 0x102008e00040242ULL  , 0x809005202050100ULL  ,
        0x8002024220104080ULL   , 0x431008804142000ULL  , 0x19001802081400ULL   , 0x200014208040080ULL  , 0x3308082008200100ULL ,
        0x41010500040c020ULL    , 0x4012020c04210308ULL , 0x208220a202004080ULL , 0x111040120082000ULL  , 0x6803040141280a00ULL ,
        0x2101004202410000ULL   , 0x8200000041108022ULL , 0x21082088000ULL      , 0x2410204010040ULL    , 0x40100400809000ULL   ,
        0x822088220820214ULL    , 0x40808090012004ULL   , 0x910224040218c9ULL   , 0x402814422015008ULL  , 0x90014004842410ULL   ,
        0x1000042304105ULL      , 0x10008830412a00ULL   , 0x2520081090008908ULL , 0x40102000a0a60140ULL , }}};

std::unordered_map<pieceType, std::vector<int>> MoveGeneratorBitset::relevantBits = 
    {{ pieceType::rook, {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12,
    }}, { pieceType::bishop, {
        6 , 5 , 5 , 5 , 5 , 5 , 5 , 6 ,
        5 , 5 , 5 , 5 , 5 , 5 , 5 , 5 ,
        5 , 5 , 7 , 7 , 7 , 7 , 5 , 5 ,
        5 , 5 , 7 , 9 , 9 , 7 , 5 , 5 ,
        5 , 5 , 7 , 9 , 9 , 7 , 5 , 5 ,
        5 , 5 , 7 , 7 , 7 , 7 , 5 , 5 ,
        5 , 5 , 5 , 5 , 5 , 5 , 5 , 5 ,
        6 , 5 , 5 , 5 , 5 , 5 , 5 , 6 , }}};

std::unordered_map<pieceType, std::vector<std::vector<uint64_t>>> MoveGeneratorBitset::slidingAttackTable = {
        { pieceType::rook  , std::vector<std::vector<uint64_t>>(64, std::vector<uint64_t>(4096)) },
        { pieceType::bishop, std::vector<std::vector<uint64_t>>(64, std::vector<uint64_t>( 512)) }, };

std::unordered_map<size_t, std::unordered_map<MoveGeneratorBitset::ray, uint64_t>> MoveGeneratorBitset::directions = { };
std::unordered_map<pieceType, std::unordered_map<size_t, uint64_t>> MoveGeneratorBitset::lookup = { };
std::unordered_map<pieceColor, std::unordered_map<size_t, uint64_t>> MoveGeneratorBitset::pawnCatpureLookup = { };
std::unordered_map<pieceColor, std::unordered_map<size_t, uint64_t>> MoveGeneratorBitset::pawnPushLookup = { };

MoveGeneratorBitset::MoveGeneratorBitset(const std::string& FEN)  : 
    pinMap(64),
    promoteTo(pieceType::queen),
    castleMask(0ULL),
    promotionMap(0ULL),
    colorCanMove({{pieceColor::white, false}, {pieceColor::black, false}}),
    checkMask({{pieceColor::white, 0xFFFFFFFFFFFFFFFFULL}, {pieceColor::black, 0xFFFFFFFFFFFFFFFFULL}})                                                      
{
    decodeFEN(FEN);
    setUp();
    std::fill(pinMap.begin(), pinMap.end(), 0xFFFFFFFFFFFFFFFFULL);
    makeLegalMoves();
    //for(const auto& i : legalMoves) {
    //    std::cout << i.first/8 << ' ' << i.first%8 << '\n';
    //    printBitboard(i.second);
    //}
}

MoveGeneratorBitset::MoveGeneratorBitset(const MoveGeneratorBitset& otherMoveGen) : 
    halfMoveClock       (otherMoveGen.halfMoveClock),
    promotionMap        (otherMoveGen.promotionMap),
    boardState          (otherMoveGen.boardState),
    castleMask          (otherMoveGen.castleMask),
    colorBoard          (otherMoveGen.colorBoard),
    legalMoves          (otherMoveGen.legalMoves),
    pieceBoard          (otherMoveGen.pieceBoard),
    kings               (otherMoveGen.kings),
    
    pinMap              (64),
    checkMask           ({{pieceColor::white, 0xFFFFFFFFFFFFFFFFULL}, {pieceColor::black, 0xFFFFFFFFFFFFFFFFULL}}),
    colorCanMove        ({{pieceColor::white, false}, {pieceColor::black, false}})  { }

MoveGeneratorBitset& MoveGeneratorBitset::operator=(const MoveGeneratorBitset& otherMoveGen)
{
    if(this != &otherMoveGen) {
        halfMoveClock       = otherMoveGen.halfMoveClock;
        boardState          = otherMoveGen.boardState;
        promotionMap        = otherMoveGen.promotionMap;
        castleMask          = otherMoveGen.castleMask;
        colorBoard          = otherMoveGen.colorBoard;
        legalMoves          = otherMoveGen.legalMoves;
        pieceBoard          = otherMoveGen.pieceBoard;
        kings               = otherMoveGen.kings;
        
        checkMask           = {{pieceColor::white, 0xFFFFFFFFFFFFFFFFULL}, {pieceColor::black, 0xFFFFFFFFFFFFFFFFULL}};
        colorCanMove        = {{pieceColor::white, false}, {pieceColor::black, false}};
        pinMap.reserve(64);
    } 
    return *this;
}

// sets up MoveGeneratorBitset at the beginning,
// when the constructor is called
//
// needs to be called only once per object
void MoveGeneratorBitset::setUp() noexcept
{
    // makes directions according to their cardinal
    makeDirection<ray::north>();
    makeDirection<ray::south>();
    makeDirection<ray::west>();
    makeDirection<ray::east>();
    makeDirection<ray::northWest>();
    makeDirection<ray::northEast>();
    makeDirection<ray::southWest>();
    makeDirection<ray::southEast>();

    // makes all possible moves for sliding pieces
    // on an empty board, then makes all 
    // possible blockers for sliding
    // pieces, then puts all the possible moves
    // for the sliding piece in slidingAttackTable
    makeLookup<pieceType::rook>();
    makeLookup<pieceType::bishop>();
    makeLookup<pieceType::queen>();
    makeLookup<pieceType::knight>();
    makeLookup<pieceType::king>();
    makePawnLookup<pieceColor::white>();
    makePawnLookup<pieceColor::black>();

    makeslidingAttackTable(pieceType::bishop);
    makeslidingAttackTable(pieceType::rook);
}

// moves peice from square 
// A->B & checks if the move is legal 
bool MoveGeneratorBitset::move(const int& start, const int& end) noexcept
{
    // cannot move no piece somewhere
    const auto curMoving = boardState.whiteCanMove ? pieceColor::white : pieceColor::black;
    // can move only the color who
    // is currently playing
    if((getColor(start) == pieceColor::black  &&  boardState.whiteCanMove) || 
       (getColor(start)) == pieceColor::white && !boardState.whiteCanMove) return false;
    if(static_cast<uint64_t>(legalMoves[start] & (1ULL << end)) == 0ULL) return false;
    // type marks what peiceType the piece should be at
    // index end, after the moving happened 
    // is used for promotions to say what the pawn should promote to
    // all pieces promote to the same piece, except
    // pawns, who, on a terminal rank, promote to something else
    pieceType type;
    // calls respective function for each pieceType
         if(pieceBoard[pieceType::pawn  ] & (1ULL << start))   type = moveType<pieceType::pawn>(start, end);
    else if(pieceBoard[pieceType::king  ] & (1ULL << start))   type = moveType<pieceType::king>(start, end);
    else if(pieceBoard[pieceType::rook  ] & (1ULL << start))   type = moveType<pieceType::rook>(start, end);
    else if(pieceBoard[pieceType::queen ] & (1ULL << start))   type = pieceType::queen;
    else if(pieceBoard[pieceType::knight] & (1ULL << start))   type = pieceType::knight;
    else                                                       type = pieceType::bishop;
    // here it actually move the piece from A->B
    moveBitboard(start, end, type);
    // then we need to rest the board
    // to make space for the next making
    // of legal moves
    resetBitBoard();
    if(pieceBoard[pieceType::pawn] & (1ULL << end) && (std::abs(start/8-end/8) == 2)) 
        boardState.EPpawn |= boardState.whiteCanMove ? (1ULL << end-8) : (1ULL << end+8);
    // swaps currently moving color
    boardState.whiteCanMove = !boardState.whiteCanMove;
    makeLegalMoves();
    if(pieceBoard[pieceType::pawn] & (1ULL << end) && checkCount[getOppositeColor(curMoving)] == 0) ++halfMoveClock;
    else halfMoveClock = 0;
    std::cout << "white checkCnt = " << checkCount[pieceColor::white] << " black = " << checkCount[pieceColor::black] << '\n'; 
    return true;
}

// resets relevent variables for the generation of legal move
// so that the variables do not override each other and create a mess
void MoveGeneratorBitset::resetBitBoard() noexcept
{
    const auto curMoving = boardState.whiteCanMove ? pieceColor::white : pieceColor::black;
    promotionMap = 0ULL;
    castleMask = 0ULL;
    std::fill(pinMap.begin(), pinMap.end(), 0xFFFFFFFFFFFFFFFFULL);
    boardState.EPpawn = 0ULL;
    checkMask[curMoving] = 0xFFFFFFFFFFFFFFFFULL;
    colorCanMove[pieceColor::white] = false;
    colorCanMove[pieceColor::black] = false;
    captureMap.clear();
    pushMap.clear();
    checkCount.clear();
    attackColorMap.clear();
    legalMoves.clear();
} 

// actualy moves piece from A->B, not like method MoveGeneratorBitset::move(), who also bookkeeps
void MoveGeneratorBitset::moveBitboard(const int& start, const int& end, const pieceType& type) noexcept
{
    const auto startColor   = getColor(start);
    const auto endColor     = getColor(end);
    // deletes start position
    pieceBoard[pieceType::king  ]  &= ~((1ULL << start) | (1ULL << end));
    pieceBoard[pieceType::queen ]  &= ~((1ULL << start) | (1ULL << end));
    pieceBoard[pieceType::bishop]  &= ~((1ULL << start) | (1ULL << end));
    pieceBoard[pieceType::knight]  &= ~((1ULL << start) | (1ULL << end));
    pieceBoard[pieceType::rook  ]  &= ~((1ULL << start) | (1ULL << end));
    pieceBoard[pieceType::pawn  ]  &= ~((1ULL << start) | (1ULL << end));
    colorBoard[startColor] &= ~(1ULL << start);
    colorBoard[  endColor] &= ~(1ULL << end  );
    // makes new piece on end position
    pieceBoard[type]  |= (1ULL << end);
    colorBoard[startColor] |= (1ULL << end);
}

void MoveGeneratorBitset::decodeFEN(const std::string& FEN) noexcept
{
    std::istringstream ss(FEN);
    std::string partFEN;
    ss >> partFEN;
    // gets piece positon from FEN
    getPiecesFromFEN(partFEN);
    ss >> partFEN;
    // sets the currently moving color
    partFEN == "w" ? boardState.whiteCanMove |= true
                   : boardState.whiteCanMove |= false;
    ss >> partFEN;
    getCastleRights(partFEN);
    ss >> partFEN;
    while(isalpha(partFEN[0])) {
        getEnPassants(partFEN);
        ss >> partFEN;
    }
    // reads halfMoveClock  ss
    if(partFEN == "-") ss >> partFEN;
    halfMoveClock = stoi(partFEN);
}

// gets piece position from a partial FEN
void MoveGeneratorBitset::getPiecesFromFEN(const std::string& FEN) noexcept
{
    size_t indexFEN = 0;
    // for all row's and col's
    for(int row = 0; row < 8; row++) {
        int col = 0;
        while(col < 8) {
            const auto index = (7-row)*8+7-col;
            // if FEN[indexFEN] describes a piece, put it in board
            if(isalpha(FEN[indexFEN])) {
                // gets piece type
                switch(tolower(FEN[indexFEN])) {
                    case 'k':
                        pieceBoard[pieceType::  king] |= (1ULL << index);
                        islower(FEN[indexFEN]) ? kings[pieceColor::black] = index
                                               : kings[pieceColor::white] = index;
                        break;
                    case 'q':
                        pieceBoard[pieceType:: queen] |= (1ULL << index);
                        break;
                    case 'b':
                        pieceBoard[pieceType::bishop] |= (1ULL << index);
                        break;
                    case 'n':
                        pieceBoard[pieceType::knight] |= (1ULL << index);
                        break;
                    case 'r':
                        pieceBoard[pieceType::  rook] |= (1ULL << index);
                        break;
                    case 'p':
                        pieceBoard[pieceType::  pawn] |= (1ULL << index);
                        break;
                }
                // gets color
                islower(FEN[indexFEN])
                    ? colorBoard[pieceColor::black] |= (1ULL << index) 
                    : colorBoard[pieceColor::white] |= (1ULL << index) ;
                ++col;
            } else if(FEN[indexFEN] != '/') {
                // else it describes how many empty squares there are,
                // and we don't care about empty square, so we skip them
                col += (FEN[indexFEN]-'0');
            }
            ++indexFEN;
        }
        ++indexFEN;
    }
}

// gets castling rights from a partial FEN
void MoveGeneratorBitset::getCastleRights(const std::string& FEN) noexcept
{
    // if FEN does not describe any castling rights, return
    if(FEN == "-") return;
    // for all castling rights
    for(const auto& i : FEN) {
        // gets side
        switch(tolower(i)) {
            case 'k':
                // get color
                islower(i)
                    ? boardState.blackCanKingCastle = true
                    : boardState.whiteCanKingCastle = true ;
                break;
            case 'q':
                islower(i)
                    ? boardState.blackCanQueenCastle = true
                    : boardState.whiteCanQueenCastle = true ;
                break;
        }
    }
}

// en passant's from a partial FEN
void MoveGeneratorBitset::getEnPassants(const std::string& FEN) noexcept
{
    const int index = (FEN[1]-'1')*8 + 7-FEN[0]+'a';
    // puts en passant's in boardState.EPpawn
    boardState.EPpawn |= (1ULL << index);
}

// uses *magic*
// gets attacks from a sliding piece of type type, on square sq, with occupied squares occ
uint64_t MoveGeneratorBitset::getSlidingAttack(const pieceType& type, const int& sq, uint64_t occ) noexcept
{
    // gets attacks on an empty board
    occ &= lookup[type][sq];
    // uses *magic* to determine what the piece can actually see
    occ *= magic[type][sq];
    occ >>= 64 - relevantBits[type][sq];
    return slidingAttackTable[type][sq][occ];
}
 
// makes the slidingAttackTable
void MoveGeneratorBitset::makeslidingAttackTable(const pieceType& type) noexcept
{
    for(int sq = 0; sq < 64; sq++) {
        // for all squares
        uint64_t mask = lookup[type][sq]; 
        int n = count1(mask);
        for(int cnt = 0; cnt < (1 << n); cnt++) {
            // makes occuppied sequences of boards
            uint64_t occ = setOccupancy(cnt, n, mask);
            // uses *magicIndex* to determine where on the slidingAttackTable
            // this specific case of occupancy occ, the piece visibility should be
            uint64_t magicIndex = occ * magic[type][sq] >> 64 - relevantBits[type][sq];
            // uses batt/ratt to determine what the piece can actually see
            slidingAttackTable[type][sq][magicIndex] = (type == pieceType::bishop) ? batt(sq, occ) : ratt(sq, occ);
        }
    }
}

// counts the number of 1's on bitboard b
int MoveGeneratorBitset::count1(uint64_t b) const noexcept
{
    int cnt;
    // while b is still NOT 0ULL, remove one of b's 1's and increase counter cnt 
    for(cnt = 0; b; cnt++, b &= b - 1);
    return cnt;
}

// sets all occupancy's for slidingAttackTable 
uint64_t MoveGeneratorBitset::setOccupancy(const int& index, const int& bitCnt, uint64_t attack) noexcept
{
    uint64_t occ = 0ULL;
    // for all squares that the piece can see on an empty baord
    for(int cnt = 0; cnt < bitCnt; cnt++) {
        // gets one of those squares in sq
        int sq = getIndexLS1B(attack);
        // removes it and sets occ to also
        // includes (1ULL << sq)
        if(attack & (1ULL << sq)) attack ^= (1ULL << sq);
        if(index & (1 << cnt)) occ |= (1ULL << sq);
    }
    return occ;
}

// gets one of the 1's in bitboard bitboard
// if bitbaord is empty, returns 0
int MoveGeneratorBitset::getIndexLS1B(const uint64_t& bitboard) const noexcept
{
    return bitboard ? count1((bitboard & -bitboard)-1) : 0;
}

// makes a bitboard of the visibility of a rook
// on square sq, with possible blockers block
uint64_t MoveGeneratorBitset::ratt(const int& sq, const uint64_t& block) const noexcept
{
    uint64_t result = 0ULL;
    int rk = sq/8, fl = sq%8, r, f;
    // north
    for(r = rk+1; r <= 7; r++) {
        result |= (1ULL << (fl + r*8));
        if(block & (1ULL << (fl + r*8))) break;
    }
    // south
    for(r = rk-1; r >= 0; r--) {
        result |= (1ULL << (fl + r*8));
        if(block & (1ULL << (fl + r*8))) break;
    }
    // west
    for(f = fl+1; f <= 7; f++) {
        result |= (1ULL << (f + rk*8));
        if(block & (1ULL << (f + rk*8))) break;
    }
    // east
    for(f = fl-1; f >= 0; f--) {
        result |= (1ULL << (f + rk*8));
        if(block & (1ULL << (f + rk*8))) break;
    }
    return result;
}

// makes a bitboard of the visibility of a bishop
// on square sq, with possible blockers block
uint64_t MoveGeneratorBitset::batt(const int& sq, const uint64_t& block) const noexcept
{
    uint64_t result = 0ULL;
    int rk = sq/8, fl = sq%8, r, f;
    // north east
    for(r = rk+1, f = fl+1; r <= 7 && f <= 7; r++, f++) {
        result |= (1ULL << (f + r*8));
        if(block & (1ULL << (f + r * 8))) break;
    }
    // north west
    for(r = rk+1, f = fl-1; r <= 7 && f >= 0; r++, f--) {
        result |= (1ULL << (f + r*8));
        if(block & (1ULL << (f + r * 8))) break;
    }
    // south east
    for(r = rk-1, f = fl+1; r >= 0 && f <= 7; r--, f++) {
        result |= (1ULL << (f + r*8));
        if(block & (1ULL << (f + r * 8))) break;
    }
    // south west
    for(r = rk-1, f = fl-1; r >= 0 && f >= 0; r--, f--) {
        result |= (1ULL << (f + r*8));
        if(block & (1ULL << (f + r * 8))) break;
    }
    return result;
}

// makes legalMoves contain all the legal moves
void MoveGeneratorBitset::makeLegalMoves() noexcept
{ 
    // generates all the pseudo legal moves
    makeCaptureMap<pieceType::king>();
    makeCaptureMap<pieceType::queen>();
    makeCaptureMap<pieceType::bishop>();
    makeCaptureMap<pieceType::knight>();
    makeCaptureMap<pieceType::rook>();
    makeCaptureMap<pieceType::pawn>();

    makePushMap<pieceType::king>();
    makePushMap<pieceType::queen>();
    makePushMap<pieceType::bishop>();
    makePushMap<pieceType::knight>();
    makePushMap<pieceType::rook>();
    makePushMap<pieceType::pawn>();

    makeAttackColorMap();

    // makes pins which result from either a rook or a bishop
    makePinMap<pieceType::rook  >(pieceColor::white);
    makePinMap<pieceType::bishop>(pieceColor::white);
    makePinMap<pieceType::rook  >(pieceColor::black);
    makePinMap<pieceType::bishop>(pieceColor::black);

    // makes legal king moves
    makeMoves<pieceType::king>(pieceColor::white);
    makeMoves<pieceType::king>(pieceColor::black);
    // if a respective king is checked by more pieces, 
    // then no peice can block multiple checks at once,
    // so there are no other legal moves for said color
    if(checkCount[pieceColor::white] < 2) {
        makeWhiteKingCastleMove ();
        makeWhiteQueenCastleMove();

        makeMoves<pieceType::pawn>(pieceColor::white);

        makeMoves<pieceType::bishop>(pieceColor::white);
        makeMoves<pieceType::rook  >(pieceColor::white);
        makeMoves<pieceType::queen >(pieceColor::white);
        makeMoves<pieceType::knight>(pieceColor::white);
    }
    if(checkCount[pieceColor::black] < 2) {
        makeBlackKingCastleMove ();
        makeBlackQueenCastleMove();

        makeMoves<pieceType::pawn>(pieceColor::black);

        makeMoves<pieceType::bishop>(pieceColor::black);
        makeMoves<pieceType::rook  >(pieceColor::black);
        makeMoves<pieceType::queen >(pieceColor::black);
        makeMoves<pieceType::knight>(pieceColor::black);
    }
}

// makes legal white kingside castle
void MoveGeneratorBitset::makeWhiteKingCastleMove() noexcept
{
    if(static_cast<uint64_t> (0xEULL & attackColorMap[pieceColor::black]) != 0ULL) return;
    if(static_cast<uint64_t> (colorBoard[pieceColor::white] & pieceBoard[pieceType::rook] & 0x1ULL) == 0ULL) return;
    if(static_cast<uint64_t>((colorBoard[pieceColor::white] | colorBoard[pieceColor::black]) & 0x6ULL) != 0ULL) return;
    if(boardState.whiteCanKingCastle) {
        castleMask |= 0x2ULL;
        legalMoves[kings[pieceColor::white]] |= 0x2ULL;
    }
}

// makes legal white queenside castle
void MoveGeneratorBitset::makeWhiteQueenCastleMove() noexcept
{
    if(static_cast<uint64_t> (0x38ULL & attackColorMap[pieceColor::black]) != 0ULL) return;
    if(static_cast<uint64_t> (colorBoard[pieceColor::white] & pieceBoard[pieceType::rook] & 0x80ULL) == 0ULL) return;
    if(static_cast<uint64_t>((colorBoard[pieceColor::white] | colorBoard[pieceColor::black]) & 0x70ULL) != 0ULL) return;
    if(boardState.whiteCanQueenCastle) {
        castleMask |= 0x20ULL;
        legalMoves[kings[pieceColor::white]] |= 0x20ULL;
    }
}

// makes legal black kingside castle
void MoveGeneratorBitset::makeBlackKingCastleMove() noexcept
{
    if(static_cast<uint64_t> (0xE00000000000000ULL & attackColorMap[pieceColor::white]) != 0ULL) return;
    if(static_cast<uint64_t> (colorBoard[pieceColor::black] & pieceBoard[pieceType::rook] & 0x100000000000000ULL) == 0ULL) return;
    if(static_cast<uint64_t>((colorBoard[pieceColor::white] | colorBoard[pieceColor::black]) & 0x600000000000000ULL) != 0ULL) return;
    if(boardState.blackCanKingCastle) {
        castleMask |= 0x200000000000000ULL;
        legalMoves[kings[pieceColor::black]] |= 0x200000000000000ULL;
    }
}

// makes legal black queenide castle
void MoveGeneratorBitset::makeBlackQueenCastleMove() noexcept
{
    if(static_cast<uint64_t> (0x3800000000000000ULL & attackColorMap[pieceColor::white]) != 0ULL) return;
    if(static_cast<uint64_t> (colorBoard[pieceColor::black] & pieceBoard[pieceType::rook] & 0x8000000000000000ULL) == 0ULL) return;
    if(static_cast<uint64_t>((colorBoard[pieceColor::white] | colorBoard[pieceColor::black]) & 0x7000000000000000ULL) != 0ULL) return;
    if(boardState.blackCanQueenCastle) {
        castleMask |= 0x2000000000000000ULL;
        legalMoves[kings[pieceColor::black]] |= 0x2000000000000000ULL;
    }
}

void MoveGeneratorBitset::makeAttackColorMap() noexcept 
{
    for(const auto& i : pieceBoard) {
        auto curMask = i.second;
        while(curMask) {
            const auto sq = getIndexLS1B(curMask);
            curMask & (1ULL << sq) ? curMask ^= (1ULL << sq) : curMask = 0ULL;
            const auto color = getColor(sq);
            const auto opposite = getOppositeColor(color);
            const uint64_t occ = (colorBoard[pieceColor::white] | colorBoard[pieceColor::black]) ^ (1ULL << kings[opposite]);
                 if(i.first == pieceType::king  ) attackColorMap[color] |= getAttack<pieceType::king  >(sq, occ);
            else if(i.first == pieceType::queen ) attackColorMap[color] |= getAttack<pieceType::queen >(sq, occ);
            else if(i.first == pieceType::bishop) attackColorMap[color] |= getAttack<pieceType::bishop>(sq, occ);
            else if(i.first == pieceType::knight) attackColorMap[color] |= getAttack<pieceType::knight>(sq, occ);
            else if(i.first == pieceType::rook  ) attackColorMap[color] |= getAttack<pieceType::rook  >(sq, occ);
            else if(i.first == pieceType::pawn  ) attackColorMap[color] |= getAttack<pieceType::pawn  >(sq, occ);
        }
    }
}

// makes en passant captures for pawns and checks if the
// king whould be in check
bool MoveGeneratorBitset::isValidEPCapture(const int& capturer, const int& EP) noexcept
{
    const auto curMoving = getCurMoving();
    const auto oppositeColor = getOppositeColor(curMoving);
    if(kings[curMoving]/8 != capturer/8) return true;
    // occ is a bitboard that includes all occupied pieces
    // that are on the row that the king is on,
    // that are NOT the pawn captured by the en passant
    // thar are NOT the pawn that would capture the en passant
    uint64_t occ = (colorBoard[pieceColor::white] | colorBoard[pieceColor::black]) & 
                    (0xFFULL << (kings[curMoving]/8*8)) & 
                    ~(0x101010101010101ULL << (EP%8)) &
                    ~(1ULL << capturer); 
    // checks for the pieces that are visible from the pawn capturer index
    uint64_t pawnView = getAttack<pieceType::rook>(capturer, occ) & occ;
    // checks if the pieces are a king and a rook/queen
    if(pawnView & (1ULL << kings[curMoving])) {
        pawnView &= ~(1ULL << kings[curMoving]);
        // if they are, continue
        if((pieceBoard[pieceType::rook] | pieceBoard[pieceType::queen]) & colorBoard[oppositeColor] & pawnView) return false;
    }
    return true;
}

std::unordered_map<pieceColor, uint64_t> MoveGeneratorBitset::getColorBoard() const noexcept
{
    return colorBoard; 
}
std::unordered_map<pieceType , uint64_t> MoveGeneratorBitset::getPieceBoard() const noexcept
{ 
    return pieceBoard; 
}

void MoveGeneratorBitset::setPromoteTo(const pieceType& type) noexcept
{
    promoteTo = type;
}

uint64_t MoveGeneratorBitset::getPromotionMap() const noexcept
{
    return promotionMap;
}

uint64_t MoveGeneratorBitset::getLegalMoves(const int& sq) const noexcept
{
    return legalMoves.find(sq) == legalMoves.end() ? 0ULL : legalMoves.find(sq)->second;
}

pieceColor MoveGeneratorBitset::getOppositeColor(const pieceColor& color) const noexcept
{
    return color == pieceColor::white ? pieceColor::black : pieceColor::white;
}

pieceColor MoveGeneratorBitset::getColor(const int &sq) const noexcept
{
    return colorBoard.find(pieceColor::white)->second & (1ULL << sq) ? pieceColor::white : pieceColor::black;
}

pieceColor MoveGeneratorBitset::getCurMoving() const noexcept
{
    return boardState.whiteCanMove ? pieceColor::white : pieceColor::black;
}

pieceType MoveGeneratorBitset::getType (const int& sq) noexcept
{
         if(pieceBoard[pieceType::king  ] & (1ULL << sq))   return pieceType::king;
    else if(pieceBoard[pieceType::queen ] & (1ULL << sq))   return pieceType::queen;
    else if(pieceBoard[pieceType::bishop] & (1ULL << sq))   return pieceType::bishop;
    else if(pieceBoard[pieceType::knight] & (1ULL << sq))   return pieceType::knight;
    else if(pieceBoard[pieceType::rook  ] & (1ULL << sq))   return pieceType::rook;
    else if(pieceBoard[pieceType::pawn  ] & (1ULL << sq))   return pieceType::pawn;
    else                                                    return pieceType::none;
}

pieceType MoveGeneratorBitset::getPromoteTo() const noexcept
{
    return promoteTo;
}

// 2  = still playing
// 1  = white won
// 0  = draw
// -1 = black won 
int MoveGeneratorBitset::checkResult() noexcept
{
    if(!colorCanMove[pieceColor::black]) return checkCount[pieceColor::black] ?  1 : 0;
    if(!colorCanMove[pieceColor::white]) return checkCount[pieceColor::white] ?  -1 : 0;
    if(halfMoveClock >= 50) return 0;
    return 2;
}

int MoveGeneratorBitset::isInCheck() const noexcept
{
    const auto curColor = boardState.whiteCanMove ? pieceColor::black : pieceColor::white;
    const auto oppositeColor = getOppositeColor(curColor);
    return checkCount.find(curColor)->second ? kings.find(oppositeColor)->second : 64;
}

void MoveGeneratorBitset::printBitboard(const uint64_t& bitboard) noexcept
{
    auto s = std::bitset<64>(bitboard).to_string();
    std::replace(s.begin(), s.end(), '0', '.');
    for(int i = 0; i < 8; i++) 
        std::cout << s.substr(i*8, 8) << '\n';
    std::cout << '\n';
}

template<pieceType type> 
pieceType MoveGeneratorBitset::moveType(const int& start, const int& end) noexcept
{
    return getType(start);
}

template<> 
pieceType MoveGeneratorBitset::moveType<pieceType::pawn>(const int& start, const int& end) noexcept
{
    // captures en passant pawn
    if(boardState.EPpawn & (1ULL << end)) {
        if(boardState.whiteCanMove) {
            pieceBoard[pieceType::pawn  ] &= ~(1ULL << (end-8));
            colorBoard[pieceColor::black] &= ~(1ULL << (end-8));
        } else {
            pieceBoard[pieceType::pawn  ] &= ~(1ULL << (end+8));
            colorBoard[pieceColor::white] &= ~(1ULL << (end+8));
        }   
    } else if(promotionMap & (1ULL << start)) {
        // returns what pieceType the pawn should promote to
        return promoteTo;
    } 
    // else pieceType 
    return pieceType::pawn;
}

template<> 
pieceType MoveGeneratorBitset::moveType<pieceType::king>(const int& start, const int& end) noexcept
{
    const auto curMoving = boardState.whiteCanMove ? pieceColor::white : pieceColor::black;
    // if king is moved the color who is moving
    // no longer has any castling rights
    if(curMoving == pieceColor::white) {
        boardState.whiteCanKingCastle  = false;
        boardState.whiteCanQueenCastle = false;
    } else {
        boardState.blackCanKingCastle  = false;
        boardState.blackCanQueenCastle = false;
    }
    // if castle happened move the rook
    if((castleMask & (1ULL << end)) && 
      ((curMoving == pieceColor::white && start == 3 ) ||
       (curMoving == pieceColor::black && start == 59))) {
        int rookIndex;
        // makes new rook for castle
        if(kings[curMoving]%8 <= end%8) {
            rookIndex = kings[curMoving]/8*8+7;
            pieceBoard[pieceType::rook] |= (1ULL << (end-1));
            colorBoard[curMoving] |= (1ULL << (end-1));
        } else {
            rookIndex = kings[curMoving]/8*8;
            pieceBoard[pieceType::rook] |= (1ULL << (end+1));
            colorBoard[curMoving] |= (1ULL << (end+1));
        }
        // deletes old rook
        pieceBoard[pieceType::rook] &= ~(1ULL << rookIndex);
        colorBoard[curMoving] &= ~(1ULL << rookIndex);
    }
    kings[getCurMoving()] = end;
    return pieceType::king;
}

template<> 
pieceType MoveGeneratorBitset::moveType<pieceType::rook>(const int& start, const int& end) noexcept
{
    const auto curMoving = boardState.whiteCanMove ? pieceColor::white : pieceColor::black;
    if(kings[curMoving]%8 < end%8) {
        if(curMoving == pieceColor::white) boardState.whiteCanQueenCastle = false;
        else boardState.blackCanQueenCastle = false;
    } else {
        if(curMoving == pieceColor::white) boardState.whiteCanKingCastle = false;
        else boardState.blackCanKingCastle = false;
    }
    return pieceType::rook;
}


// makes a bitboard with all the squares that a piece can see
// on a board with occupied squares occ, type type and on square sq

template<pieceType type>
uint64_t MoveGeneratorBitset::getAttack(const int& sq, const uint64_t& occ) noexcept
{
    return lookup[type][sq];
}

template<>
uint64_t MoveGeneratorBitset::getAttack<pieceType::rook>(const int& sq, const uint64_t& occ) noexcept
{
    return getSlidingAttack(pieceType::rook, sq, occ);
}

template<>
uint64_t MoveGeneratorBitset::getAttack<pieceType::bishop>(const int& sq, const uint64_t& occ) noexcept
{
    return getSlidingAttack(pieceType::bishop, sq, occ);
}

template<>
uint64_t MoveGeneratorBitset::getAttack<pieceType::queen>(const int& sq, const uint64_t& occ) noexcept
{
    return getSlidingAttack(pieceType::rook, sq, occ) | getSlidingAttack(pieceType::bishop, sq, occ);
}

template<>
uint64_t MoveGeneratorBitset::getAttack<pieceType::pawn>(const int& sq, const uint64_t& occ) noexcept
{
    return pawnCatpureLookup[getColor(sq)][sq];
}

// shoots rays from they king with type == rook, or
// like type == bishop, then checks for friend pieces, then
// shoots the same ray type from the piece and checks for a 
// rook, a bishop, or a queen; if it finds one of them, 
// we have found a pinned piece

template<pieceType type>
void MoveGeneratorBitset::makePinMap(const pieceColor& color) noexcept
{
    const auto oppositeColor = getOppositeColor(color);
    uint64_t occ = colorBoard[pieceColor::white] | colorBoard[pieceColor::black];
    // friend piece on the ray from the king
    uint64_t neighbours = getAttack<type>(kings[color], occ);
    uint64_t possiblePin = neighbours & colorBoard[color];
    // while there still are such pieces
    while(possiblePin) {
        const auto sq = getIndexLS1B(possiblePin);
        possiblePin & (1ULL << sq) ? possiblePin ^= (1ULL << sq) : possiblePin = 0ULL;
        // shoots another ray from the piece
        const uint64_t pinnerMask = getAttack<type>(kings[color], occ^(1ULL << sq)) & colorBoard[oppositeColor] & ~neighbours;
        // checks for either a queen or a type piece
        if(pinnerMask && (pieceBoard[type] & pinnerMask || pieceBoard[pieceType::queen] & pinnerMask)) 
            pinMap[sq] = getCheckMap<type>(kings[color], getIndexLS1B(pinnerMask), occ^(1ULL << sq)) ^ (1ULL << sq);
    }
}

template<pieceType type>
uint64_t MoveGeneratorBitset::getCheckMap(const int& kingIndex, const int& pieceIndex, const uint64_t& occ) noexcept
{
    return (1ULL << pieceIndex);
}

template<>
uint64_t MoveGeneratorBitset::getCheckMap<pieceType::rook>(const int& kingIndex, const int& pieceIndex, const uint64_t& occ) noexcept
{   
    return getAttack<pieceType::rook>(kingIndex , occ) & 
           getAttack<pieceType::rook>(pieceIndex, occ) | 
           (1ULL << pieceIndex);
}

template<>
uint64_t MoveGeneratorBitset::getCheckMap<pieceType::bishop>(const int& kingIndex, const int& pieceIndex, const uint64_t& occ) noexcept
{   
    return getAttack<pieceType::bishop>(kingIndex , occ) & 
           getAttack<pieceType::bishop>(pieceIndex, occ) |
           (1ULL << pieceIndex);
}

template<>
uint64_t MoveGeneratorBitset::getCheckMap<pieceType::queen>(const int& kingIndex, const int& pieceIndex, const uint64_t& occ) noexcept
{   
    return getAttack<pieceType::bishop>(kingIndex, occ) & (1ULL << pieceIndex)
           ? getCheckMap<pieceType::bishop>(kingIndex, pieceIndex, occ)
           : getCheckMap<pieceType::rook  >(kingIndex, pieceIndex, occ);
}

template<pieceType type>
void MoveGeneratorBitset::makeCaptureMap() noexcept
{
    const uint64_t occ = colorBoard[pieceColor::white] | colorBoard[pieceColor::black];
    auto curPieces = pieceBoard[type];
    while(curPieces) {
        const auto sq = getIndexLS1B(curPieces);
        curPieces & (1ULL << sq) ? curPieces ^= (1ULL << sq) : curPieces = 0ULL;
        const auto color = getColor(sq);
        const auto oppositeColor = getOppositeColor(color);
        captureMap[sq] = getAttack<type>(sq, occ) & colorBoard[oppositeColor];
        if(captureMap[sq] & (1ULL << kings[oppositeColor])) {
            ++checkCount[color];
            checkMask[oppositeColor] = getCheckMap<type>(kings[oppositeColor], sq, occ);
        }
    }
}

template<>
void MoveGeneratorBitset::makeCaptureMap<pieceType::pawn>() noexcept
{
    const uint64_t occ = colorBoard[pieceColor::white] | colorBoard[pieceColor::black];
    auto curPieces = pieceBoard[pieceType::pawn];
    while(curPieces) {
        const auto sq = getIndexLS1B(curPieces);
        curPieces & (1ULL << sq) ? curPieces ^= (1ULL << sq) : curPieces = 0ULL;
        const auto color = getColor(sq);
        const auto oppositeColor = getOppositeColor(color);
        captureMap[sq] = pawnCatpureLookup[color][sq] & colorBoard[oppositeColor];
        if(captureMap[sq] & (1ULL << kings[oppositeColor])) { 
            ++checkCount[color];
            checkMask[oppositeColor] = getCheckMap<pieceType::pawn>(kings[oppositeColor], sq, occ);
        }
        if(boardState.EPpawn & pawnCatpureLookup[color][sq]) {
            const auto epIndex = getIndexLS1B(boardState.EPpawn & pawnCatpureLookup[color][sq]);
            if(isValidEPCapture(sq, epIndex)) captureMap[sq] |= (1ULL << epIndex);
        }
    }
}

template<pieceType type>
void MoveGeneratorBitset::makePushMap() noexcept
{
    const uint64_t occ = colorBoard[pieceColor::white] | colorBoard[pieceColor::black];
    auto curPieces = pieceBoard[type];
    while(curPieces) {
        const auto sq = getIndexLS1B(curPieces);
        curPieces & (1ULL << sq) ? curPieces ^= (1ULL << sq) : curPieces = 0ULL;
        pushMap[sq] = getAttack<type>(sq, occ) & ~occ;
    }
}

template<>
void MoveGeneratorBitset::makePushMap<pieceType::pawn>() noexcept
{
    const uint64_t occ = colorBoard[pieceColor::white] | colorBoard[pieceColor::black];
    auto curPieces = pieceBoard[pieceType::pawn];
    while(curPieces) {
        const auto sq = getIndexLS1B(curPieces);
        curPieces & (1ULL << sq) ? curPieces ^= (1ULL << sq) : curPieces = 0ULL;
        pushMap[sq] = pawnPushLookup[getColor(sq)][sq] & getAttack<pieceType::rook>(sq, occ)  & ~occ;
        const auto color = getColor(sq);
        if((sq/8 == 1 && color == pieceColor::black) || (sq/8 == 6 && color == pieceColor::white)) promotionMap |= (1ULL << sq);
    }
}


// iterates through all squares and makes the papawnPushLookupwn and the pawnCatpureLookup
template<pieceColor color>
const void MoveGeneratorBitset::makePawnLookup() noexcept 
{ }

template<>
const void MoveGeneratorBitset::makePawnLookup<pieceColor::white>() noexcept
{   
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(i == 7) continue;
            const auto index = i*8+j;
                            pawnPushLookup      [pieceColor::white][index] |= (1ULL << (index+8 ));
            if(i == 1)      pawnPushLookup      [pieceColor::white][index] |= (1ULL << (index+16));
            if(j >  0)      pawnCatpureLookup   [pieceColor::white][index] |= (1ULL << (index+7 ));
            if(j <  7)      pawnCatpureLookup   [pieceColor::white][index] |= (1ULL << (index+9 ));
        }
    }
}

template<>
const void MoveGeneratorBitset::makePawnLookup<pieceColor::black>() noexcept
{
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(i == 0) continue; 
            const auto index = i*8+j;
                            pawnPushLookup      [pieceColor::black][index] |= (1ULL << (index-8 ));
            if(i == 6)      pawnPushLookup      [pieceColor::black][index] |= (1ULL << (index-16));
            if(j > 0)       pawnCatpureLookup   [pieceColor::black][index] |= (1ULL << (index-9 ));
            if(j < 7)       pawnCatpureLookup   [pieceColor::black][index] |= (1ULL << (index-7 ));
        }
    }
}

template<pieceType type> 
void MoveGeneratorBitset::makeMoves(const pieceColor& color) noexcept
{
    uint64_t mask = pieceBoard[type] & colorBoard[color];
    // while there still are type pieces which are unchecked
    while(mask) {
        const auto sq = getIndexLS1B(mask);
        mask & (1ULL << sq) ? mask ^= (1ULL << sq) : mask = 0ULL;
        legalMoves[sq] = ((pushMap[sq] | captureMap[sq]) & checkMask[color]) & pinMap[sq];
        if(legalMoves[sq]) colorCanMove[getColor(sq)] = true;
    }
}

template<> 
void MoveGeneratorBitset::makeMoves<pieceType::king>(const pieceColor& color) noexcept
{
    const auto oppositeColor = getOppositeColor(color);
    // legal moves are those that don't result in a check for the same color
    legalMoves[kings[color]] = (pushMap[kings[color]] | captureMap[kings[color]]) & ~attackColorMap[oppositeColor];
    if(legalMoves[kings[color]] != 0) colorCanMove[color] = true;
}

template<> 
void MoveGeneratorBitset::makeMoves<pieceType::pawn>(const pieceColor& color) noexcept
{
    uint64_t pawnMask = pieceBoard[pieceType::pawn] & colorBoard[color];
    // checks can also be obstructed by capturing an en passant
    const auto pawnSq = getIndexLS1B(checkMask[color] & pieceBoard[pieceType::pawn]);
    uint64_t enPassantCheckMask = color == pieceColor::white ? boardState.EPpawn & (1ULL << (pawnSq+8))
                                                             : boardState.EPpawn & (1ULL << (pawnSq-8));
    // for all pawn
    while(pawnMask) {
        const auto sq = getIndexLS1B(pawnMask);
        pawnMask & (1ULL << sq) ? pawnMask ^= (1ULL << sq) : pawnMask = 0ULL;
        legalMoves[sq] = (pushMap[sq] | captureMap[sq]) & (checkMask[color] | enPassantCheckMask) & pinMap[sq];
        if(legalMoves[sq]) colorCanMove[getColor(sq)] = true;
    }
}

template<pieceType type> 
const void MoveGeneratorBitset::makeLookup() noexcept
{ }

template<> 
const void MoveGeneratorBitset::makeLookup<pieceType::rook>() noexcept
{
    for(int sq = 0; sq < 64; sq++)
        lookup[pieceType::rook][sq] = directions[sq][ray::north] | directions[sq][ray::south] | 
                                      directions[sq][ray::west ] | directions[sq][ray::east ] ;
}

template<> 
const void MoveGeneratorBitset::makeLookup<pieceType::bishop>() noexcept
{
    for(int sq = 0; sq < 64; sq++)
        lookup[pieceType::bishop][sq] = directions[sq][ray::northWest] | directions[sq][ray::northEast] | 
                                        directions[sq][ray::southWest] | directions[sq][ray::southEast] ;
}

template<> 
const void MoveGeneratorBitset::makeLookup<pieceType::queen>() noexcept
{
    for(int sq = 0; sq < 64; sq++)
        lookup[pieceType::queen ][sq] = lookup[pieceType::rook  ][sq]  | 
                                        lookup[pieceType::bishop][sq]  ;
}

template<> 
const void MoveGeneratorBitset::makeLookup<pieceType::knight>() noexcept
{
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            const auto index = (7-i)*8+7-j;
            if(j > 0) {
                if(i > 1) lookup[pieceType::knight][index] |= (1ULL << index+17);
                if(i < 6) lookup[pieceType::knight][index] |= (1ULL << index-15);
            }
            if(j > 1) {
                if(i > 0) lookup[pieceType::knight][index] |= (1ULL << index+10);
                if(i < 7) lookup[pieceType::knight][index] |= (1ULL << index-6);
            }
            if(j < 7) {
                if(i > 1) lookup[pieceType::knight][index] |= (1ULL << index+15);
                if(i < 6) lookup[pieceType::knight][index] |= (1ULL << index-17);
            }
            if(j < 6) {
                if(i > 0) lookup[pieceType::knight][index] |= (1ULL << index+6);
                if(i < 7) lookup[pieceType::knight][index] |= (1ULL << index-10);
            }
        }
    }
}

template<> 
const void MoveGeneratorBitset::makeLookup<pieceType::king>() noexcept
{
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            const auto index = i*8+j;
            if(j > 0) {
                lookup[pieceType::king][index] |= (1ULL << (index-1));
                if(i < 7)   lookup[pieceType::king][index] |= (1ULL << (index+7));
                if(i > 0)   lookup[pieceType::king][index] |= (1ULL << (index-9));
            }
            if(j < 7) {
                lookup[pieceType::king][index] |= (1ULL << (index+1));
                if(i < 7)   lookup[pieceType::king][index] |= (1ULL << (index+9));
                if(i > 0)   lookup[pieceType::king][index] |= (1ULL << (index-7));
            }
            if(i < 7)       lookup[pieceType::king][index] |= (1ULL << (index+8));
            if(i > 0)       lookup[pieceType::king][index] |= (1ULL << (index-8));
        }
    }
}

template<MoveGeneratorBitset::ray ray> 
const void MoveGeneratorBitset::makeDirection() noexcept
{ }

// makes east direction
template<> 
const void MoveGeneratorBitset::makeDirection<MoveGeneratorBitset::ray::east>() noexcept
{
    uint64_t east = 0x7F;
    for(int i = 0; i < 8; i++) {
        uint64_t es = east;
        for(int j = 0; j < 8; j++) {
            const auto index = i*8+7-j;
            directions[index][ray::east] = es & ~0x101010101010101ULL;
            es &= ~(1ULL << (i*8+6-j));
        }
        east<<=8;
    }
}

// makes west direction
template<> 
const void MoveGeneratorBitset::makeDirection<MoveGeneratorBitset::ray::west>() noexcept
{
    uint64_t west = 0xFE;
    for(int i = 0; i < 8; i++) {
        uint64_t we = west;
        for(int j = 0; j < 8; j++) {
            const auto index = i*8+j;
            directions[index][ray::west] = we & ~0x8080808080808080ULL;
            we &= ~(1ULL << j+1+i*8);
        }
        west<<=8;
    }
}

// makes north direction
template<> 
const void MoveGeneratorBitset::makeDirection<MoveGeneratorBitset::ray::north>() noexcept
{
    uint64_t nort = 0x0101010101010100ULL;
    for(int sq = 0; sq < 64; sq++) {
        const auto index = sq;
        directions[index][ray::north] = nort & ~0xFF00000000000000ULL;
        nort<<=1;
    }
}

// makes south direction
template<> 
const void MoveGeneratorBitset::makeDirection<MoveGeneratorBitset::ray::south>() noexcept
{
    uint64_t sout = 0x0080808080808080ULL;
    for (int sq=63; sq >= 0; sq--){
        const auto index = sq;
        directions[index][ray::south] = sout & ~0xFFULL;
        sout >>= 1;
    }
}

// makes north west direction
template<> 
const void MoveGeneratorBitset::makeDirection<MoveGeneratorBitset::ray::northWest>() noexcept
{
    uint64_t nowe = 0x102040810204000ULL;
    for(int i = 0; i < 8; i++) {
        uint64_t nw = nowe;
        for (int j = 0; j < 8; j++) {
            const auto index = i*8+7-j;
            directions[index][ray::northWest] = nw & ~0xFF818181818181FFULL;  
            if(j-i >= 0) nw &= ~(1ULL << 8*(7-j+i)); 
            nw>>=1; 
        }
        nowe<<=8;
    }
}

// makes south east direction
template<> 
const void MoveGeneratorBitset::makeDirection<MoveGeneratorBitset::ray::southEast>() noexcept
{
    uint64_t soea = 0x40201008040201ULL;
    for(int i = 0; i < 8; i++) {
        uint64_t se = soea;
        for (int j = 0; j < 8; j++) {
            const auto index = (7-i)*8+7-j;
            directions[index][ray::southEast] = se & ~0xFF818181818181FFULL;
            if(j-i >= 0) se &= ~(1ULL << 8*(j-i));
            se>>=1;
        }
        soea>>=8;
    }
}

// makes north east direction
template<> 
const void MoveGeneratorBitset::makeDirection<MoveGeneratorBitset::ray::northEast>() noexcept
{
    uint64_t noea = 0x8040201008040200ULL;
    for(int i = 0; i < 8; i++) {
        uint64_t ne = noea;
        for (int j = 0; j < 8; j++) {
            const auto index = i*8+j;
            directions[index][ray::northEast] = ne & ~0xFF818181818181FFULL;
            if(j-i >= 0) ne &= ~(1ULL << 8*(8+i-j)-1); 
            ne<<=1;
        }
        noea<<=8;
    }
}

// makes south west direction
template<> 
const void MoveGeneratorBitset::makeDirection<MoveGeneratorBitset::ray::southWest>() noexcept
{
    uint64_t sowe = 0x2040810204080ULL;
    for(int i = 0; i < 8; i++) {
        uint64_t sw = sowe;
        for (int j = 0; j < 8; j++) {
            const auto index = (7-i)*8 + j;
            directions[index][ray::southWest] = sw & ~0xFF818181818181FFULL;
            if(j-i >= 0) sw &= ~(1ULL << (j-i)*8+7); 
            sw<<=1;
        }
        sowe>>=8;
    }
}