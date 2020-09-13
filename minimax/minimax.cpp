#include "minimax.h"
#include <algorithm>

std::pair<Position,Position> getBestMove(chess::ChessBoard board,int depth, bool maximizing, int alpha, int beta)
{
	if (!board.isCheckMate())
	{
		auto moves = getAllMovesFromCurrentTurn(board);
		int max = INT_MAX;
		std::pair<Position, Position> bestpos = moves[0];
		int teller = 0;
		for (const std::pair<Position,Position>& move : moves)
		{
			chess::Tile removedTile = board.getTiles()[move.second.x][move.second.y];
			chess::move(board.getTiles(), move.first, move.second);
			chess::switchTurns(board);
			double value = getBestOutcome(board, depth-1, !maximizing,alpha,beta);
			if (value < max)
			{
				max = value;
				bestpos = move;
			}
			std::cout << "move: " << move.first << " to " << move.second << " will result in a " << value << std::endl;
			chess::move(board.getTiles(), move.second, move.first);
			chess::switchTurns(board);
			board.getTiles()[move.second.x][move.second.y].pawn = removedTile.pawn;
			board.getTiles()[move.second.x][move.second.y].color = removedTile.color;
		}
		//std::cout << "bestmove: from " << bestpos.first << " to " << bestpos.second << " with a value of " << max << std::endl;
		return bestpos;
	}
}

double evaluatePosition(chess::ChessBoard board, int turn)
{
	board.setTurn(turn);
	auto moves = getAllMovesFromCurrentTurn(board);
	return static_cast<double>(moves.size()) / 10.0;
}

double getStaticEvaluation(chess::ChessBoard board)
{
	double white = 0, black = 0;
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
	//white += evaluatePosition(board, 1); // too expensive
	//black += evaluatePosition(board, 0);
	//std::cout << white - black << std::endl;
	return white - black;  // if white is playing better then black it will be positive otherwise it will be negative
}

double getBestOutcome(chess::ChessBoard& board,int depth, bool maximizing, int alpha, int beta)
{
	if (depth == 0 || board.isCheckMate())
	{
		double value = getStaticEvaluation(board);
		return value;
	}
	else
	{
		std::vector<double> valueList;
		auto moves = getAllMovesFromCurrentTurn(board);
		for (const std::pair<Position,Position>& move : moves)
		{
			chess::Tile removedTile = board.getTiles()[move.second.x][move.second.y];
			chess::move(board.getTiles(), move.first, move.second);
			chess::switchTurns(board);
			valueList.push_back(getBestOutcome(board, depth - 1, !maximizing, alpha, beta));
			chess::move(board.getTiles(), move.second, move.first);
			chess::switchTurns(board);
			board.getTiles()[move.second.x][move.second.y].pawn = removedTile.pawn;
			board.getTiles()[move.second.x][move.second.y].color = removedTile.color;
			if (maximizing)
			{
				double max_value = max(valueList);
				alpha = max_value > alpha ? max_value : alpha;
			}
			else
			{
				double min_value = min(valueList);
				beta = min_value < beta ? min_value : beta;
			}
			if (beta <= alpha)
			{
				break;
			}		
		}
		if (maximizing)
		{
			double max_value = max(valueList);
			return max_value;
		}
		else
		{
			double min_value = min(valueList);
			return min_value;
		}
	}
}

std::vector<std::pair<Position,Position>> getAllMovesFromCurrentTurn(chess::ChessBoard board)
{
	std::vector<std::pair<Position,Position>> allMoves;
	std::vector<std::pair<Position,Position>> moves;
	for (int i = 0; i < board.getTiles().size(); ++i)
	{
		for (int j = 0; j < board.getTiles()[i].size(); j++)
		{
			if (board.getTiles()[i][j].color == board.getTurn())
			{
				auto positions = chess::getMoves(board.getTiles(), board.getTurn(), Position(i, j));
				for (Position pos: positions)
				{
					allMoves.push_back(std::pair<Position, Position>({ i,j }, pos));
				}
				/*auto positions = chess::getMoves(board.getTiles(), board.getTurn(), Position(i, j));
				allMoves.insert(allMoves.end(), positions.begin(), positions.end());
				chess::removeCheck(positions, board.getTiles(), Position(i, j));
				std::vector<std::pair<Position, Position>> tuples;
				for (Position pos:positions)
				{
					tuples.push_back(std::pair<Position, Position>(Position(i, j), pos));
				}
				moves.insert(moves.end(), tuples.begin(), tuples.end());*/
			}
		}
	}
	chess::removeCheck(allMoves, board.getTiles());
	return allMoves;
}

double getValue(int pawn)
{
	switch (pawn)
	{
	case 0:
		return 0;
	case 1:
		return 1.0;
	case 2:
		return 5.0;
	case 3:
		return 3.0;
	case 4:
		return 3.25;
	case 5:
		return 9.0;
	case 6:
		return 0;
	default:
		return 0.0;
	}
}

double min(const std::vector<double>& list)
{
	double min = 2000000000.0;
	for (double value : list)
	{
		if (value < min)
		{
			min = value;
		}
	}
	return min;
}

double max(const std::vector<double>& list)
{
	double max = -2000000000.0;
	for (double value : list)
	{
		if (value > max)
		{
			max = value;
		}
	}
	return max;
}
