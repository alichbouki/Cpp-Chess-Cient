#ifndef GUI_HANDLER_HPP
#define GUI_HANDLER_HPP

#include "src/Core/Chess.hpp"

void initializeGUI(Piece **Board);

void getPlayerMove(int *move);

void updateGUI(Piece **Board, int *move);

#endif