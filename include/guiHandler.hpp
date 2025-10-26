#ifndef GUI_HANDLER_HPP
#define GUI_HANDLER_HPP

#include <chess.hpp>

void initializeGUI(ChessBoard *Board);

void getPlayerMove(char *move);

void updateGUI(ChessBoard *Board, char *move);

#endif