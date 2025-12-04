#include <botHandler.hpp>
#include <chess.hpp>

using namespace std;

int main() {
    ChessBoard board(true); // Standard starting position, bot plays black
    board.printBoard();
    for (int i = 0; i < 3; i++) {
        getBotMove(&board);
        board.printBoard();
        sleep(1); 
    }
    return 0;
}