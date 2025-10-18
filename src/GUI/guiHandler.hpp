#ifndef GUI_HANDLER_HPP
#define GUI_HANDLER_HPP

#include "src/Core/Chess.hpp"

void initializeGUI(ChessBoard *Board);

void getPlayerMove(int *move);

void updateGUI(ChessBoard *Board, int *move);

#endif