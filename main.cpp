#include "src/Core/Chess.hpp"
#include "src/GUI/guiHandler.hpp"
#include "src/Bot/botHandler.hpp"

int main(){
    Piece **Board;

    initializeGUI(Board);

    Move move;

    while (true)
    {
        getPlayerMove(move);
        updateGUI(Board, move);

        getBotMove(Board, move);
        updateGUI(Board, move);
    }
    return 0;
}