#include "src/Core/Chess.hpp"

using namespace PieceTypes;
using namespace PieceStates;

static int _pieceCounter = 0;

Piece::Piece(PieceType type, PieceState state = PieceState::NORMAL)
{
    _type = type;
    _state = state;
    _id = _pieceCounter;
    _pieceCounter++;
}

void Piece::move(int toX, int toY)
{
    _x = toX;
    _y = toY;
}

void Piece::capture()
{
    _state = PieceState::CAPTURED;
}

PieceType Piece::getType() const
{
    return _type;
}

PieceState Piece::getState() const
{
    return _state;
}

int Piece::getId() const
{
    return _id;
}

Piece* ChessBoard::getPieceAt(int x, int y) const
{
    return _board[x][y];
}

void ChessBoard::setPieceAt(int x, int y, Piece* piece)
{
    _board[x][y] = piece;
}

void ChessBoard::movePiece(int fromX, int fromY, int toX, int toY)
{
    Piece* piece = getPieceAt(fromX, fromY);

    if (getPieceAt(toX, toY) != nullptr) {
        
        getPieceAt(toX, toY)->capture();
    }

    setPieceAt(toX, toY, piece);
    setPieceAt(fromX, fromY, nullptr);
    
    piece->move(toX, toY);
}