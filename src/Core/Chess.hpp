#ifndef PIECE_HPP
#define PIECE_HPP

// This array will hold move information: pieceId, toX, toY, specialMoveFlag
typedef int Move[4];

namespace PieceTypes {
    enum Type {
        PAWN = 1,
        KNIGHT = 2,
        BISHOP = 3,
        ROOK = 4,
        QUEEN = 5,
        KING = 6,
        EMPTY = 0
    };
}
typedef PieceTypes::Type PieceType;

namespace PieceStates {
    enum State {
        EMPTY = 0,
        NORMAL = 1,
        CAPTURED = 2,
        CHECK = 3,
        CHECKMATE = 4
    };
}
typedef PieceStates::State PieceState;

class Piece
{
    private:
        PieceType _type;
        PieceState _state;
        int _id;
        int _x;
        int _y;

    public:
        Piece(PieceType type, PieceState state);

        void move(int toX, int toY);
        void capture();

        PieceType getType() const;
        PieceState getState() const;
        int getId() const;
};

class ChessBoard
{
    private:
        Piece*** _board; // 2D array of Piece pointers

    public:
        Piece* getPieceAt(int x, int y) const;
        void setPieceAt(int x, int y, Piece* piece);
        void movePiece(int fromX, int fromY, int toX, int toY);
};

#endif // PIECE_HPP