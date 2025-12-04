#include <chess.hpp>

static int _pieceCounter = 0;

Piece::Piece(char type, char state)
{
    _type = type;
    _state = state;
    _id = _pieceCounter;
    _pieceCounter++;
}

void Piece::capture()
{
    _state = PieceState::CAPTURED;
}

char Piece::getType() const
{
    return _type;
}

char Piece::getState() const
{
    return _state;
}

int Piece::getId() const
{
    return _id;
}

ChessBoard::ChessBoard(bool startingPosition){
    _board = new Piece**[8];
    for (int i = 0; i < 8; i++) {
        _board[i] = new Piece*[8];
        for (int j = 0; j < 8; j++) {
            _board[i][j] = nullptr;
        }
    }

    eval = 0.0f;

    resetBoard(startingPosition);
}

void ChessBoard::resetBoard(bool startingPosition) {
    if (startingPosition) {
        FENToBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    } 
    else {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                _board[i][j] = nullptr;
            }
        }

        enPassantTarget = nullptr;
        turn = WHITE_TURN;
        wck = true;
        wcq = true;
        bck = true;
        bcq = true;
        moveCount = 1;
    }
}

Piece* ChessBoard::getPieceAt(int file, int rank) const
{
    return _board[file][rank];
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
    if (turn == WHITE_TURN) {
        turn = BLACK_TURN;
    } else {
        turn = WHITE_TURN;
        moveCount++;
    }
}

string ChessBoard::boardToFEN() {
    string fen;
    string piecesPlacement = "";

    for (int i = 7; i >= 0; i--) {
        int emptyCount = 0;
        for (int j = 0; j < 8; j++) {
            Piece* piece = this->getPieceAt(j, i);
            if (piece == nullptr) {
                emptyCount++;
            } else {
                if (emptyCount > 0) {
                    piecesPlacement += to_string(emptyCount);
                    emptyCount = 0;
                }
                piecesPlacement += piece->getType();
            }
        }
        if (emptyCount > 0) {
            piecesPlacement += to_string(emptyCount);
        }
        if (i > 0) {
            piecesPlacement += '/';
        }
    }

    string enPassantStr = "";

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = this->getPieceAt(i, j);
            if (piece != nullptr && (piece->getType() == PieceType::BLACK_PAWN || piece->getType() == PieceType::WHITE_PAWN)) {
                if (enPassantTarget != nullptr && enPassantTarget->getId() == piece->getId()) {
                    enPassantStr = string(1, i + 'a') + to_string(j + 1);
                }
            }
        }
    }

    fen = 
        piecesPlacement + " " + 
        turn +
        " " +
        (wck ? "K" : "") +
        (wcq ? "Q" : "") +
        (bck ? "k" : "") +
        (bcq ? "q" : "") +
        (bcq || bck || wcq || wck ? "" : "-") +
        " " +
        (enPassantTarget != nullptr ? enPassantStr : "-") +
        " 0 " +
        to_string(moveCount);

    return fen;    
}

void ChessBoard::FENToBoard(const string& fen) {
    // Split FEN string into its components
    size_t pos = 0;
    size_t nextSpace = fen.find(' ', pos);
    string piecesPlacement = fen.substr(pos, nextSpace - pos);
    
    // Parse piece placement
    int rank = 7;
    int file = 0;
    for (size_t i = 0; i < piecesPlacement.length(); i++) {
        char c = piecesPlacement[i];
        
        if (c == '/') {
            rank--;
            file = 0;
        } else if (isdigit(c)) {
            // Empty squares
            int emptyCount = c - '0';
            for (int j = 0; j < emptyCount; j++) {
                _board[file][rank] = nullptr;
                file++;
            }
        } else {
            // Piece
            Piece* piece = new Piece(c);
            _board[file][rank] = piece;
            file++;
        }
    }
    
    // Parse active color (turn)
    pos = nextSpace + 1;
    nextSpace = fen.find(' ', pos);
    if (nextSpace != string::npos) {
        turn = fen[pos];
    }
    
    // Parse castling rights
    pos = nextSpace + 1;
    nextSpace = fen.find(' ', pos);
    if (nextSpace != string::npos) {
        string castling = fen.substr(pos, nextSpace - pos);
        wck = (castling.find('K') != string::npos);
        wcq = (castling.find('Q') != string::npos);
        bck = (castling.find('k') != string::npos);
        bcq = (castling.find('q') != string::npos);
    }
    
    // Parse en passant target
    pos = nextSpace + 1;
    nextSpace = fen.find(' ', pos);
    if (nextSpace != string::npos) {
        string enPassant = fen.substr(pos, nextSpace - pos);
        if (enPassant != "-") {
            // Convert algebraic notation to coordinates (e.g., "e3" -> x=4, y=2)
            int epFile = enPassant[0] - 'a';
            int epRank = enPassant[1] - '1';
            enPassantTarget = this->getPieceAt(epFile, epRank);
        } else {
            enPassantTarget = nullptr;
        }
    }
    
    // Parse halfmove clock (skipped for now as it's not stored in ChessBoard)
    pos = nextSpace + 1;
    nextSpace = fen.find(' ', pos);
    
    // Parse fullmove number
    if (nextSpace != string::npos) {
        pos = nextSpace + 1;
        moveCount = stoi(fen.substr(pos));
    }
}

void ChessBoard::_convIntToChar(int file, int rank, char *outFile, char *outRank) {
    *outFile = static_cast<char>(file + 'a');
    *outRank = static_cast<char>(rank + '1');
}

void ChessBoard::_convCharToInt(char file, char rank, int *outFile, int *outRank) {
    *outFile = static_cast<int>(file - 'a');
    *outRank = static_cast<int>(rank - '1');
}

void ChessBoard::printBoard() const {
    for (int rank = 7; rank >= 0; rank--) {
        cout << rank + 1 << " ";
        for (int file = 0; file < 8; file++) {
            Piece* piece = getPieceAt(file, rank);
            if (piece != nullptr) {
                cout << piece->getType() << " ";
            } else {
                cout << ". ";
            }
        }
        cout << endl;
    }
    cout << "  a b c d e f g h" << endl;
}