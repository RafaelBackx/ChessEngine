#include "minimax.h"
#include <tuple>
#include <algorithm>

std::pair<Position,Position> getBestMove(chess::ChessBoard board,int depth, bool maximizing)
{
	/*if (!board.isCheckMate())
	{
		auto moves = minimax(board, depth, maximizing);
		return maximizing ? max(moves,board.getTiles()) : min(moves,board.getTiles());
	}
	return std::pair<Position, Position>(Position(0, 0), Position(0, 0));*/
	if (!board.isCheckMate())
	{
		auto moves = getAllMovesFromCurrentTurn(board);
		int max = INT_MAX;
		std::pair<Position, Position> bestpos = moves[0];
		int teller = 0;
		for (std::pair<Position,Position> move : moves)
		{
			std::cout << "move " << ++teller << move.first << " " << move.second << std::endl;
			chess::Tile removedTile = board.getTiles()[move.second.x][move.second.y];
			chess::move(board.getTiles(), move.first, move.second);
			chess::switchTurns(board);
			int value = getBestOutcome(board, depth-1, maximizing);
			if (value < max)
			{
				max = value;
				bestpos = move;
			}
			//std::cout << "move: " << move.first << " to " << move.second << " will result in a " << value << std::endl;
			chess::move(board.getTiles(), move.second, move.first);
			board.getTiles()[move.second.x][move.second.y].pawn = removedTile.pawn;
			board.getTiles()[move.second.x][move.second.y].color = removedTile.color;
		}
		//std::cout << "bestmove: from " << bestpos.first << " to " << bestpos.second << " with a value of " << max << std::endl;
		return bestpos;
	}
}

double getStaticEvaluation(chess::ChessBoard board)
{
	int white = 0, black = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board.getTiles()[i][j].color)
			{
				white += getValue(board.getTiles()[i][j].pawn);
			}
			else
			{
				black += getValue(board.getTiles()[i][j].pawn);
			}
		}
	}
	std::cout << white - black << std::endl;
	return white - black;  // if white is playing better then black it will be positive otherwise it will be negative
}

int getBestOutcome(chess::ChessBoard board,int depth, bool maximizing)
{
	if (depth == 0 || board.isCheckMate())
	{
		return getStaticEvaluation(board);
	}
	else
	{
		std::vector<int> valueList;
		auto moves = getAllMovesFromCurrentTurn(board);
		for (std::pair<Position,Position> move : moves)
		{
			std::string turn = board.getTurn() ? "white: " : "black: ";
			std::cout << turn << move.first << " " << move.second << " ";
			chess::Tile removedTile = board.getTiles()[move.second.x][move.second.y];
			chess::move(board.getTiles(), move.first, move.second);
			chess::switchTurns(board);
			valueList.push_back(getBestOutcome(board, depth-1, !maximizing));
			chess::move(board.getTiles(), move.second, move.first);
			chess::switchTurns(board);
			board.getTiles()[move.second.x][move.second.y].pawn = removedTile.pawn;
			board.getTiles()[move.second.x][move.second.y].color = removedTile.color;
		}
		if (maximizing)
		{
			return *std::max_element(valueList.begin(), valueList.end());
		}
		else
		{
			return *std::min_element(valueList.begin(), valueList.end());
		}
	}
}

std::vector<std::pair<Position, Position>> minimax(chess::ChessBoard board, int depth, bool maximizing)
{
	auto moves = getAllMovesFromCurrentTurn(board);
	if (depth == 0 || board.isCheckMate())
	{
		return moves;
	}
	else
	{
		if (maximizing)
		{
			std::vector<std::pair<Position, Position>> max_list;
			for (std::pair<Position,Position> pos:moves)
			{
				chess::Tile removedTile = board.getTiles()[pos.second.x][pos.second.y];
				chess::move(board.getTiles(), pos.first, pos.second);
				chess::switchTurns(board);
				max_list.push_back(max(minimax(board, depth - 1, false),board.getTiles()));
				chess::move(board.getTiles(), pos.second, pos.first);
				board.getTiles()[pos.second.x][pos.second.y].pawn = removedTile.pawn;
				board.getTiles()[pos.second.x][pos.second.y].color = removedTile.color;
			}
			return max_list;
		}
		else
		{
			std::vector<std::pair<Position, Position>> min_list;
			for (std::pair<Position, Position> pos : moves)
			{
				chess::Tile removedTile = board.getTiles()[pos.second.x][pos.second.y];
				chess::move(board.getTiles(), pos.first, pos.second);
				chess::switchTurns(board);
				min_list.push_back(min(minimax(board, depth - 1, true),board.getTiles()));
				chess::move(board.getTiles(), pos.second, pos.first);
				board.getTiles()[pos.second.x][pos.second.y].pawn = removedTile.pawn;
				board.getTiles()[pos.second.x][pos.second.y].color = removedTile.color;
			}
			return min_list;
		}
	}
}

std::vector<std::pair<Position,Position>> getAllMovesFromCurrentTurn(chess::ChessBoard board)
{
	std::vector<std::pair<Position,Position>> moves;
	for (int i = 0; i < board.getTiles().size(); ++i)
	{
		for (int j = 0; j < board.getTiles()[i].size(); j++)
		{
			if (board.getTiles()[i][j].color == board.getTurn())
			{
				auto positions = chess::getMoves(board.getTiles(), board.getTurn(), Position(i, j));
				chess::removeCheck(positions, board.getTiles(), Position(i, j));
				std::vector<std::pair<Position, Position>> tuples;
				for (Position pos:positions)
				{
					tuples.push_back(std::pair<Position, Position>(Position(i, j), pos));
				}
				moves.insert(moves.end(), tuples.begin(), tuples.end());
			}
		}
	}
	return moves;
}

double getValue(int pawn)
{
	switch (pawn)
	{
	case 0:
		return 0;
	case 1:
		return 1;
	case 2:
		return 5;
	case 3:
		return 3;
	case 4:
		return 3.5;
	case 5:
		return 9;
	case 6:
		return INT_MAX;
	default:
		return 0;
	}
}

std::pair<Position,Position> max(std::vector<std::pair<Position,Position>> moves, std::array<std::array<chess::Tile,8>,8> tiles)
{
	int max = INT_MIN;
	std::pair<Position, Position> returnValue;
	for (std::pair<Position,Position> pos : moves)
	{
		int value = getValue(tiles[pos.second.x][pos.second.y].pawn);
		if (value>max)
		{
			max = value;
			returnValue = pos;
		}
	}
	return returnValue;
}

std::pair<Position, Position> min(std::vector<std::pair<Position, Position>> moves, std::array<std::array<chess::Tile, 8>, 8> tiles)
{
	int min = INT_MAX;
	std::pair<Position, Position> returnValue;
	for (std::pair<Position, Position> pos : moves)
	{
		int value = getValue(tiles[pos.second.x][pos.second.y].pawn);
		if (value < min)
		{
			min = -value;
			returnValue = pos;
		}
	}
	return returnValue;
}
