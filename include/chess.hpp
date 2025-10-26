#ifndef PIECE_HPP
#define PIECE_HPP

#include <string>

using namespace std;

#define WHITE_TURN 'w'
#define BLACK_TURN 'b'

// A move is represented as an array of four integers: {fromX, fromY, toX, toY}
typedef char Move[4];

/// @brief  Pieces types in chess
namespace PieceType {
    enum Type {
        BLACK_PAWN = 'p',
        BLACK_KNIGHT = 'n',
        BLACK_BISHOP = 'b',
        BLACK_ROOK = 'r',
        BLACK_QUEEN = 'q',
        BLACK_KING = 'k',

        WHITE_PAWN = 'P',
        WHITE_KNIGHT = 'N',
        WHITE_BISHOP = 'B',
        WHITE_ROOK = 'R',
        WHITE_QUEEN = 'Q',
        WHITE_KING = 'K',

        EMPTY = ' '
    };
}

/// @brief  Piece states in chess
namespace PieceState {
    enum State {
        EMPTY = ' ',
        NORMAL = '1',
        CAPTURED = '2',
        CHECK = '3',
        CHECKMATE = '4',
        PROMOTION = '5'
    };
}

/// @brief Represents a chess piece and the chess board
class Piece
{
    private:
        char _type;
        char _state;
        int _id;
        int _y;
        int _x;
        
    public:

        /** 
         * @brief Piece Constructor
         * @class Piece
         * @param type: The piece type (e.g., 'P' for white pawn, 'k' for black king). Use PieceType for reference.
         * @param state: The piece state (e.g., '1' for normal, '2' for captured). Use PieceState for reference.
        */
        Piece(char type = PieceType::EMPTY, char state = PieceState::NORMAL);

        /** 
         * @brief Get the current position of the piece
         * @param enPassant: Whether the position is for an en passant capture, default is false
         * @return The position of the piece as a string (e.g., "e4")
        */
        string getPosition(bool enPassant = false) const;

        /**
         * @brief Move the piece to a new position
         * @param toX The target x-coordinate (file) e.g., 0 for 'a', 1 for 'b', ..., 7 for 'h'
         * @param toY The target y-coordinate (rank)
        */
        void move(int toX, int toY);

        /**
         * @brief Capture the piece
        */
        void capture();

        /**
         * @brief Get the type of the piece
         * @return The piece type as a char, check PieceType for reference
        */
        char getType() const;
        /**
         * @brief Get the state of the piece
         * @return The piece state as a char, check PieceState for reference
        */
        char getState() const;
        /**
         * @brief Get the unique ID of the piece
         * @return The piece ID as an integer
        */
        int getId() const;
};

class ChessBoard
{
    private:
        Piece*** _board; // 2D array of Piece pointers

    public:
        /// @brief  Current turn: 'w' for white, 'b' for black
        char turn = WHITE_TURN;

        /// @brief White king-side castle
        bool wck = true;
        /// @brief White queen-side castle
        bool wcq = true;
        /// @brief Black king-side castle
        bool bck = true;
        /// @brief Black queen-side castle
        bool bcq = true;

        /// @brief Move count
        int moveCount = 1;

        /// @brief En passant target piece or nullptr if none
        Piece* enPassantTarget = nullptr;
        
        /** 
         * @brief ChessBoard Constructor
         * @param startingPosition: If true, initializes to standard chess starting position; otherwise, empty board
        */
        ChessBoard(bool startingPosition);
        /** 
         * @brief Reset the chess board
         * @param startingPosition: If true, resets to standard chess starting position; otherwise, empties the board
        */
        void resetBoard(bool startingPosition);

        /**
         * @brief Get the piece at a specific position
         * @param file The x-coordinate (file) e.g., 0 for 'a', 1 for 'b', ..., 7 for 'h'
         * @param rank The y-coordinate (rank)
         * @return Pointer to the Piece at the specified position, or nullptr if empty
        */
        Piece* getPieceAt(int file, int rank) const;

        /**
         * @brief Set a piece at a specific position
         * @param file The x-coordinate (file) e.g., 0 for 'a', 1 for 'b', ..., 7 for 'h'
         * @param rank The y-coordinate (rank)
         * @param piece Pointer to the Piece to place at the specified position
        */
        void setPieceAt(int file, int rank, Piece* piece);

        /**
         * @brief Move a piece from one position to another
         * @param fromFile The starting x-coordinate (file)
         * @param fromRank The starting y-coordinate (rank)
         * @param toFile The target x-coordinate (file)
         * @param toRank The target y-coordinate (rank)
        */
        void movePiece(int fromFile, int fromRank, int toFile, int toRank);

        /**
         * @brief Convert the current board state to FEN notation
         * @return The FEN string representing the current board state
        */
        string boardToFEN();

        /**
         * @brief Set up the board from a FEN string
         * @param fen The FEN string to parse and set up the board
         */
        void FENToBoard(const string& fen);

        /**
         * @brief  Convert file and rank from int to char representation
         * @param  file: The file as an integer (0-7)
         * @param  rank: The rank as an integer (0-7)
         * @param  outFile: Reference to store the converted file character ('a'-'h')
         * @param  outRank: Reference to store the converted rank character ('1'-
         */
        static void _convIntToChar(int file, int rank, char &outFile, char &outRank);

        /**
         * @brief  Convert file and rank from char to int representation
         * @param  file: The file as a character ('a'-'h')
         * @param  rank: The rank as a character ('1'-'8')
         * @param  outFile: Reference to store the converted file integer (0-7)
         * @param  outRank: Reference to store the converted rank integer (0-7)
         */
        static void _convCharToInt(char file, char rank, int &outFile, int &outRank);
};


#endif // PIECE_HPP