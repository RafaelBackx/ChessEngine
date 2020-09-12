#include "HistoryManager.h"

void HistoryManager::addMove(chess::Move move)
{
	if (this->historyIndex < this->history.size())
	{
		this->history[this->historyIndex++] = move;
	}
	else if (this->historyIndex == this->history.size())
	{
		this->history.push_back(move);
		this->historyIndex++;
	}
	else
	{
		std::cout << "something wrong with history index" << std::endl;
	}
}

void HistoryManager::undo(chess::ChessBoard& chessboard)
{
	if (this->historyIndex > 0)
	{
		this->historyIndex--;
		chess::Move move = this->history[this->historyIndex];
		chess::move(chessboard.getTiles(), move.to, move.from);
		if (move.capturedPiece.pawn != 0)
		{
			chessboard.getTiles()[move.to.x][move.to.y].color = move.capturedPiece.color;
			chessboard.getTiles()[move.to.x][move.to.y].pawn = move.capturedPiece.pawn;
		}
		chess::switchTurns(chessboard);
	}
}

void HistoryManager::redo(chess::ChessBoard& chessboard)
{
	if (this->historyIndex < this->history.size())
	{
		chess::Move move = this->history[this->historyIndex++];
		chess::move(chessboard.getTiles(), move.from, move.to);
	}
}