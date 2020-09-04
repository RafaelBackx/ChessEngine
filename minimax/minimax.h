#ifndef MINIMAX_H
#define MINIMAX_H
#include "..//game/chess.h"

std::pair<Position,Position> getBestMove(chess::ChessBoard board, int depth, bool maximizing); // minimax function
std::vector<std::pair<Position, Position>> getAllMovesFromCurrentTurn(chess::ChessBoard board);
std::vector<std::pair<Position, Position>> minimax(chess::ChessBoard, int depth, bool maximizing);
int getBestOutcome(chess::ChessBoard board, int depth, bool maximizing);

std::pair<Position, Position> max(std::vector<std::pair<Position, Position>> moves, std::array<std::array<chess::Tile,8>,8> tiles);
std::pair<Position, Position> min(std::vector<std::pair<Position, Position>> moves, std::array<std::array<chess::Tile,8>,8> tiles);
double getValue(int pawn);

#endif // !MINIMAX_H

