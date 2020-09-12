#ifndef MINIMAX_H
#define MINIMAX_H
#include "..//game/chess.h"
#include <sstream>

std::pair<Position,Position> getBestMove(chess::ChessBoard board, int depth, bool maximizing,int alpha, int beta); // minimax function
std::vector<std::pair<Position, Position>> getAllMovesFromCurrentTurn(chess::ChessBoard board);
double getBestOutcome(chess::ChessBoard& board, int depth, bool maximizing, int alpha, int beta);
double max(const std::vector<double>& list);
double min(const std::vector<double>& list);
double getValue(int pawn);

#endif // !MINIMAX_H

