#ifndef HISORY_MANAGER_H
#define HISTORY_MANAGER_H
#include "chess.h"

class HistoryManager
{
	std::vector<chess::Move> history;
	int historyIndex=0;
public:
	void undo(chess::ChessBoard& board);
	void redo(chess::ChessBoard& board);
	void addMove(Position from, Position to, chess::Tile capturedPiece) { this->addMove({ from,to,capturedPiece }); }
	void addMove(chess::Move move);
};
#endif // !HISORY_MANAGER_H

