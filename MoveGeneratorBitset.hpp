#ifndef MOVEGENERATORBITSET_H
#define MOVEGENERATORBITSET_H

#pragma once

#include <string>
#include <sstream>
#include <bitset>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "Chess.hpp"

struct BoardState 
{
    uint64_t EPpawn;
    bool whiteCanMove;
    bool whiteCanKingCastle;
    bool whiteCanQueenCastle;
    bool blackCanKingCastle;
    bool blackCanQueenCastle;
    constexpr BoardState() : whiteCanMove(false),
                             whiteCanKingCastle(false), whiteCanQueenCastle(false),
                             blackCanKingCastle(false), blackCanQueenCastle(false), 
                             EPpawn(0) { }
};

class MoveGeneratorBitset
{
    public:
        MoveGeneratorBitset(const std::string& FEN);
        MoveGeneratorBitset(const MoveGeneratorBitset& otherMoveGen);
        MoveGeneratorBitset& operator=(const MoveGeneratorBitset& otherMoveGen);
        ~MoveGeneratorBitset() = default;
        bool move(const int& start, const int& end) noexcept;
        int getIndexLS1B(const uint64_t& bitboard) const noexcept;
        int isInCheck() const noexcept;
        int checkResult() noexcept;
        void setPromoteTo(const pieceType& type) noexcept;
        uint64_t getPromotionMap() const noexcept;
        uint64_t getLegalMoves(const int& sq) const noexcept;
        pieceColor getColor(const int& sq) const noexcept;
        pieceColor getCurMoving() const noexcept;
        pieceType getType (const int& sq) noexcept;
        pieceType getPromoteTo() const noexcept;
        std::unordered_map<pieceColor, uint64_t> getColorBoard() const noexcept;
        std::unordered_map<pieceType , uint64_t> getPieceBoard() const noexcept;

        void printBitboard(const uint64_t& bitboard) noexcept;

    private:
        enum class ray { north, northEast, east, southEast, south, southWest, west, northWest };
    
    private:
        static std::unordered_map<pieceType, std::vector<uint64_t>> magic;
        static std::unordered_map<pieceType, std::vector<int>> relevantBits;
        static std::unordered_map<pieceColor, std::unordered_map<size_t, uint64_t>> pawnCatpureLookup;
        static std::unordered_map<pieceColor, std::unordered_map<size_t, uint64_t>> pawnPushLookup;
        static std::unordered_map<size_t, std::unordered_map<ray, uint64_t>> directions;
        static std::unordered_map<pieceType, std::vector<std::vector<uint64_t>>> slidingAttackTable;
        static std::unordered_map<pieceType, std::unordered_map<size_t, uint64_t>> lookup;

    private:
        int halfMoveClock;
        BoardState boardState;
        uint64_t promotionMap;
        uint64_t castleMask;
        pieceType promoteTo;
        std::vector<uint64_t> pinMap;
        std::unordered_map<pieceColor, bool> colorCanMove;
        std::unordered_map<pieceColor, uint64_t> colorBoard;
        std::unordered_map<pieceColor, uint64_t> attackColorMap;
        std::unordered_map<size_t, uint64_t> legalMoves;
        std::unordered_map<size_t, uint64_t> captureMap;
        std::unordered_map<size_t, uint64_t> pushMap;
        std::unordered_map<pieceType, uint64_t> pieceBoard;
        std::unordered_map<pieceColor, uint64_t> checkMask;
        std::unordered_map<pieceColor, int> kings;
        std::unordered_map<pieceColor, int> checkCount;

    private:
        void resetBitBoard() noexcept;

        void decodeFEN(const std::string& FEN) noexcept;
        void getPiecesFromFEN(const std::string& FEN) noexcept;
        void getCastleRights(const std::string& FEN) noexcept;
        void getEnPassants(const std::string& FEN) noexcept;

        void setUp() noexcept;

        template<pieceType type> uint64_t getAttack(const int& sq, const uint64_t& occ) noexcept;
        int count1(uint64_t b) const noexcept;
        void makeslidingAttackTable(const pieceType& type) noexcept;
        uint64_t getSlidingAttack(const pieceType& type, const int& sq, uint64_t occ) noexcept;
        uint64_t setOccupancy(const int& index, const int& bitCnt, uint64_t attack) noexcept;
        uint64_t batt(const int& sq, const uint64_t& block) const noexcept;
        uint64_t ratt(const int& sq, const uint64_t& block) const noexcept;

        void moveBitboard(const int& start, const int& end, const pieceType& type) noexcept;
        template<pieceType type> pieceType moveType(const int& start, const int& end) noexcept;

        void getLegalMoves() noexcept;
        void makeLegalMoves() noexcept;
        template<pieceType type> void makeMoves(const pieceColor& color) noexcept;
        void makeWhiteKingCastleMove() noexcept;
        void makeWhiteQueenCastleMove() noexcept;
        void makeBlackKingCastleMove() noexcept;
        void makeBlackQueenCastleMove() noexcept;

        template<pieceType type> void makeCaptureMap() noexcept;
        template<pieceType type> void makePushMap() noexcept;
        template<pieceType type> void makePinMap(const pieceColor& color) noexcept;
        template<pieceType type> uint64_t getCheckMap(const int& kingIndex, 
                                                      const int& pieceIndex, 
                                                      const uint64_t& occ) noexcept;
        template<pieceColor color> const void makePawnLookup() noexcept;
        void makeAttackColorMap() noexcept;
        bool isValidEPCapture(const int& capturer, const int& EP) noexcept;
        
        template<pieceType type> const void makeLookup() noexcept;
        template<ray ray> const void makeDirection() noexcept;
        
        pieceColor getOppositeColor(const pieceColor& color) const noexcept;
};

template<> pieceType MoveGeneratorBitset::moveType<pieceType::rook>(const int& start, const int& end) noexcept;
template<> pieceType MoveGeneratorBitset::moveType<pieceType::pawn>(const int& start, const int& end) noexcept;
template<> pieceType MoveGeneratorBitset::moveType<pieceType::king>(const int& start, const int& end) noexcept;

template<> uint64_t MoveGeneratorBitset::getAttack<pieceType::rook  >(const int& sq, const uint64_t& occ) noexcept;
template<> uint64_t MoveGeneratorBitset::getAttack<pieceType::bishop>(const int& sq, const uint64_t& occ) noexcept;
template<> uint64_t MoveGeneratorBitset::getAttack<pieceType::queen >(const int& sq, const uint64_t& occ) noexcept;
template<> uint64_t MoveGeneratorBitset::getAttack<pieceType::pawn  >(const int& sq, const uint64_t& occ) noexcept;

template<> uint64_t MoveGeneratorBitset::getCheckMap<pieceType::rook  >(const int& kingIndex, 
                                                                        const int& pieceIndex, 
                                                                        const uint64_t& occ) noexcept;
template<> uint64_t MoveGeneratorBitset::getCheckMap<pieceType::bishop>(const int& kingIndex, 
                                                                        const int& pieceIndex, 
                                                                        const uint64_t& occ) noexcept;
template<> uint64_t MoveGeneratorBitset::getCheckMap<pieceType::queen >(const int& kingIndex, 
                                                                        const int& pieceIndex, 
                                                                        const uint64_t& occ) noexcept;

template<> void MoveGeneratorBitset::makeCaptureMap<pieceType::pawn>() noexcept;

template<> void MoveGeneratorBitset::makePushMap<pieceType::pawn>() noexcept;

template<> const void MoveGeneratorBitset::makePawnLookup<pieceColor::white>() noexcept;
template<> const void MoveGeneratorBitset::makePawnLookup<pieceColor::black>() noexcept;

template<> void MoveGeneratorBitset::makeMoves<pieceType::king>(const pieceColor& color) noexcept;
template<> void MoveGeneratorBitset::makeMoves<pieceType::pawn>(const pieceColor& color) noexcept;

template<> const void MoveGeneratorBitset::makeLookup<pieceType::rook  >() noexcept;
template<> const void MoveGeneratorBitset::makeLookup<pieceType::bishop>() noexcept;
template<> const void MoveGeneratorBitset::makeLookup<pieceType::queen >() noexcept;
template<> const void MoveGeneratorBitset::makeLookup<pieceType::knight>() noexcept;
template<> const void MoveGeneratorBitset::makeLookup<pieceType::king  >() noexcept;

template<> const void MoveGeneratorBitset::makeDirection<MoveGeneratorBitset::ray::east     >() noexcept;
template<> const void MoveGeneratorBitset::makeDirection<MoveGeneratorBitset::ray::west     >() noexcept;
template<> const void MoveGeneratorBitset::makeDirection<MoveGeneratorBitset::ray::north    >() noexcept;
template<> const void MoveGeneratorBitset::makeDirection<MoveGeneratorBitset::ray::south    >() noexcept;
template<> const void MoveGeneratorBitset::makeDirection<MoveGeneratorBitset::ray::northWest>() noexcept;
template<> const void MoveGeneratorBitset::makeDirection<MoveGeneratorBitset::ray::southEast>() noexcept;
template<> const void MoveGeneratorBitset::makeDirection<MoveGeneratorBitset::ray::northEast>() noexcept;
template<> const void MoveGeneratorBitset::makeDirection<MoveGeneratorBitset::ray::southWest>() noexcept;

#endif