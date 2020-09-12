#include "chess.h"

std::vector<Position> chess::getPawnMoves(std::array<std::array<chess::Tile, 8>, 8> tiles, Position pos)
{
	std::vector<Position> positions;
	chess::Tile t = tiles[pos.x][pos.y];
	//the pawn is white
	if (t.color)
	{
		if (pos.y >0)
		{
			Position front(pos.x, pos.y - 1);
			Position doublefront(pos.x, pos.y - 2);
			Position left(pos.x - 1, pos.y - 1);
			Position right(pos.x + 1, pos.y - 1);
			//check for double front move
			if (pos.y == 6) // pawn has not moved yet
			{
				if (tiles[doublefront.x][doublefront.y].isEmpty() && tiles[front.x][front.y].isEmpty())
				{
					positions.push_back(doublefront);
				}
			}
			if (tiles[front.x][front.y].isEmpty())
				positions.push_back(front);
			//check for left and right moves
			if (left.x > -1)
			{
				if (!tiles[left.x][left.y].isEmpty() && tiles[left.x][left.y].color != t.color)
					positions.push_back(left);
			}
			if (right.x < 8)
			{
				if (!tiles[right.x][right.y].isEmpty() && tiles[right.x][right.y].color != t.color)
					positions.push_back(right);
			}
		}
	}
	else
	{
		if (pos.y <7)
		{
			Position front(pos.x, pos.y + 1);
			Position doublefront(pos.x, pos.y + 2);
			Position left(pos.x - 1, pos.y + 1);
			Position right(pos.x + 1, pos.y + 1);
			//check doublefront move
			if (pos.y == 1)
			{
				if (tiles[doublefront.x][doublefront.y].isEmpty() && tiles[front.x][front.y].isEmpty())
				{
					positions.push_back(doublefront);
				}
			}
			if (tiles[front.x][front.y].isEmpty())
			{
				positions.push_back(front);
			}
			// check for left and right
			if (left.x > -1)
			{
				if (!tiles[left.x][left.y].isEmpty() && tiles[left.x][left.y].color != t.color)
					positions.push_back(left);
			}
			if (right.x < 8)
			{
				if (!tiles[right.x][right.y].isEmpty() && tiles[right.x][right.y].color != t.color)
					positions.push_back(right);
			}
		}
	}
	return positions;
}

std::vector<Position> chess::getRookMoves(std::array<std::array<chess::Tile, 8>, 8> tiles, Position pos)
{
	std::vector<Position> positions;
	int front = tiles[pos.x][pos.y].color ? -1 : 1;
	int back = -front;
	int right = 1, left = -1;
	//get all positions in front
	int counter = 1;
	while(pos.y + (front*counter) >-1 && pos.y + (front*counter) < 8)
	{
		if (!tiles[pos.x][pos.y+(front*counter)].isEmpty())
		{
			if(tiles[pos.x][pos.y + (front * counter)].color != tiles[pos.x][pos.y].color)
			{
				positions.push_back(Position(pos.x, pos.y + (front * counter)));
			}
			break;
		}
		else
		{
			positions.push_back(Position(pos.x, pos.y + (front*counter)));
		}
		++counter;
	}
	// get backwards positions
	counter = 1;
	while (pos.y + (back * counter) > -1 && pos.y + (back * counter) < 8)
	{
		if (!tiles[pos.x][pos.y + (back * counter)].isEmpty())
		{
			if (tiles[pos.x][pos.y + (back * counter)].color != tiles[pos.x][pos.y].color)
			{
				positions.push_back(Position(pos.x, pos.y + (back * counter)));
			}
			break;
		}
		else
		{
			positions.push_back(Position(pos.x, pos.y + (back * counter)));
		}
		++counter;
	}
	counter = 1;
	// get positions to the right
	while (pos.x + (right*counter) <8)
	{
		if (!tiles[pos.x + (right*counter)][pos.y].isEmpty())
		{
			if (tiles[pos.x+(right*counter)][pos.y].color != tiles[pos.x][pos.y].color)
			{
				positions.push_back(Position(pos.x + (right * counter), pos.y));
			}
			break;
		}
		else
		{
			positions.push_back(Position(pos.x + (right * counter), pos.y));
		}
		counter++;
	}
	counter = 1;
	// get positions to the left
	while (pos.x + (left * counter) > -1)
	{
		if (!tiles[pos.x + (left * counter)][pos.y].isEmpty())
		{
			if (tiles[pos.x + (left * counter)][pos.y].color != tiles[pos.x][pos.y].color)
			{
				positions.push_back(Position(pos.x + (left * counter), pos.y));
			}
			break;
		}
		else
		{
			positions.push_back(Position(pos.x + (left * counter), pos.y));
		}
		counter++;
	}
	return positions;
}

std::vector<Position> chess::getknightMoves(std::array<std::array<chess::Tile, 8>, 8> tiles, Position pos)
{
	std::vector<Position> positions;
	chess::Tile currentTile = tiles[pos.x][pos.y];
	Position llt(pos.x - 2, pos.y - 1);  // the most left top position
	Position ltt(pos.x - 1, pos.y - 2);  // the most top left position
	Position llb(pos.x - 2, pos.y + 1);  // the most left bottom position
	Position lbb(pos.x - 1, pos.y + 2);  // the most bottom left position
	Position rtt(pos.x + 1, pos.y - 2);  // the most top right position
	Position rrt(pos.x + 2, pos.y - 1);  // the most right top position
	Position rrb(pos.x + 2, pos.y + 1);  // the most right bottom position
	Position rbb(pos.x + 1, pos.y + 2);  // the most bottom right positon
	positions.push_back(llt);
	positions.push_back(ltt);
	positions.push_back(llb);
	positions.push_back(lbb);
	positions.push_back(rtt);
	positions.push_back(rrt);
	positions.push_back(rrb);
	positions.push_back(rbb);
	checkMoves(positions, tiles, currentTile);
	return positions;
}

std::vector<Position> chess::getBishopMoves(std::array<std::array<chess::Tile, 8>, 8> tiles, Position pos)
{
	std::vector<Position> positions;
	chess::Tile currentTile = tiles[pos.x][pos.y];
	int front = tiles[pos.x][pos.y].color ? -1 : 1;
	int back = -front;
	int right = 1, left = -1;
	// get all the moves to the right and top
	int counter = 1;
	while(pos.x + (counter * right)<8 && pos.x + (counter * right) >-1 && pos.y + (counter*front) <8 && pos.y + (counter * front) > -1)
	{
		if(!tiles[pos.x + (counter * right)][pos.y + (counter * front)].isEmpty())
		{
			if (currentTile.color != tiles[pos.x + (counter * right)][pos.y + (counter * front)].color)
				positions.push_back(Position(pos.x + (counter * right),pos.y + (counter * front)));
			break;
		}
		else
		{
			positions.push_back(Position(pos.x + (counter * right), pos.y + (counter * front)));
		}
		++counter;
	}
	counter = 1;
	while (pos.x + (counter * left) < 8 && pos.x + (counter * left) > -1 && pos.y + (counter * front) < 8 && pos.y + (counter * front) > -1)
	{
		if (!tiles[pos.x + (counter * left)][pos.y + (counter * front)].isEmpty())
		{
			if (currentTile.color != tiles[pos.x + (counter * left)][pos.y + (counter * front)].color)
				positions.push_back(Position(pos.x + (counter * left),pos.y + (counter * front)));
			break;
		}
		else
		{
			positions.push_back(Position(pos.x + (counter * left), pos.y + (counter * front)));
		}
		++counter;
	}
	counter = 1;
	while (pos.x + (counter * right) < 8 && pos.x + (counter * right) > -1 && pos.y + (counter * back) < 8 && pos.y + (counter * back) > -1)
	{
		if (!tiles[pos.x + (counter * right)][pos.y + (counter * back)].isEmpty())
		{
			if (currentTile.color != tiles[pos.x + (counter * right)][pos.y + (counter * back)].color)
				positions.push_back(Position(pos.x + (counter * right), pos.y + (counter * back)));
			break;
		}
		else
		{
			positions.push_back(Position(pos.x + (counter * right), pos.y + (counter * back)));
		}
		++counter;
	}
	counter = 1;
	while (pos.x + (counter * left) < 8 && pos.x + (counter * left) > -1 && pos.y + (counter * back) < 8 && pos.y + (counter * back) > -1)
	{
		if (!tiles[pos.x + (counter * left)][pos.y + (counter * back)].isEmpty())
		{
			if (currentTile.color != tiles[pos.x + (counter * left)][pos.y + (counter * back)].color)
				positions.push_back(Position(pos.x + (counter * left), pos.y + (counter * back)));
			break;
		}
		else
		{
			positions.push_back(Position(pos.x + (counter * left), pos.y + (counter * back)));
		}
		++counter;
	}
	return positions;
}

std::vector<Position> chess::getQueenMoves(std::array<std::array<chess::Tile, 8>, 8> tiles, Position pos)
{
	std::vector<Position> positions;
	std::vector<Position> rookMoves = getRookMoves(tiles, pos);
	std::vector<Position> bishopMoves = getBishopMoves(tiles, pos);
	positions.reserve(rookMoves.size() + bishopMoves.size());
	positions.insert(positions.end(), rookMoves.begin(), rookMoves.end());
	positions.insert(positions.end(), bishopMoves.begin(), bishopMoves.end());
	return positions;
}

std::vector<Position> chess::getKingMoves(std::array<std::array<chess::Tile, 8>, 8> tiles, Position pos)
{
	Position left(pos.x - 1, pos.y); // the left most position
	Position leftTop(pos.x - 1, pos.y - 1); // the top left most position
	Position top(pos.x, pos.y - 1); // the top most positon
	Position rightTop(pos.x + 1, pos.y - 1); // the top right most position
	Position right(pos.x + 1, pos.y); // the most right position
	Position rightBottom(pos.x + 1, pos.y + 1); // the most bottom right position
	Position bottom(pos.x, pos.y + 1); // the bottom most position
	Position leftBottom(pos.x - 1, pos.y + 1); // the bottom most left position
	std::vector<Position> positions;
	positions.push_back(left);
	positions.push_back(leftTop);
	positions.push_back(top);
	positions.push_back(rightTop);
	positions.push_back(right);
	positions.push_back(rightBottom);
	positions.push_back(bottom);
	positions.push_back(leftBottom);
	chess::Tile currentTile = tiles[pos.x][pos.y];
	checkMoves(positions, tiles, currentTile);
	checkCastling(positions,tiles, pos);
	return positions;
}

void chess::checkCastling(std::vector<Position>& positions, std::array<std::array<chess::Tile, 8>, 8> tiles, Position pos)
{
	chess::Tile currentTile = tiles[pos.x][pos.y];
	if (currentTile.color)
	{
		//check if king has not moved yet
		if (pos.x == 4 && pos.y == 7)
		{
		// check if rook has not moved yet
			if (!tiles[0][7].hasMoved && tiles[0][7].pawn == 2)
			{
				// check if the path between the rook and king is clear
				if (tiles[1][7].isEmpty() && tiles[2][7].isEmpty() && tiles[3][7].isEmpty())
				{
					positions.push_back(Position(2, 7));
				}
			}
			if (!tiles[7][7].hasMoved && tiles[7][7].pawn == 2)
			{
				// check if the path between the rook and king is clear
				if (tiles[6][7].isEmpty() && tiles[5][7].isEmpty())
				{
					positions.push_back(Position(6,7));
				}
			}
		}
	}
	else
	{
		//check if king has not moved yet
		if (pos.x == 4 && pos.y == 0)
		{
			// check if rook has not moved yet
			if (!tiles[0][0].hasMoved && tiles[0][0].pawn == 2)
			{
				// check if the path between the rook and king is clear
				if (tiles[1][0].isEmpty() && tiles[2][0].isEmpty() && tiles[3][0].isEmpty())
				{
					positions.push_back(Position(2, 0));
				}
			}
			if (!tiles[7][0].hasMoved && tiles[7][0].pawn == 2)
			{
				// check if the path between the rook and king is clear
				if (tiles[6][0].isEmpty() && tiles[5][0].isEmpty())
				{
					positions.push_back(Position(6, 0));
				}
			}
		}
	}
}

void chess::checkMoves(std::vector<Position>& positions, std::array<std::array<chess::Tile,8>,8> tiles, chess::Tile currentTile)
{
	//remove all positions outside of the board or if there is a player of the same team on that spot
	int i = 0;
	while (i < positions.size())
	{
		auto pos = positions[i];
		if (pos.x < 0 || pos.x >7 || pos.y < 0 || pos.y >7 || (!tiles[pos.x][pos.y].isEmpty() && tiles[pos.x][pos.y].color == currentTile.color))
		{
			positions.erase(positions.begin() + i);
		}
		else
		{
			++i;
		}
	}
}

// Returns the removed Tile
chess::Tile chess::move(std::array<std::array<chess::Tile, 8>, 8>& tiles, Position from, Position to)
{
	// check for castling
	if (from.x == 4 && from.y == 7) // white king
	{
		if (to.x == 2 && to.y == 7) // left castle
		{
			tiles[3][7].color = tiles[0][7].color;
			tiles[3][7].pawn = tiles[0][7].pawn;
			tiles[3][7].hasMoved = true;
			tiles[0][7].color = 0;
			tiles[0][7].pawn = 0;
		}
		else if (to.x == 6 && to.y == 7) // right castle
		{
			tiles[5][7].color = tiles[7][7].color;
			tiles[5][7].pawn = tiles[7][7].pawn;
			tiles[5][7].hasMoved = true;
			tiles[7][7].color = 0;
			tiles[7][7].pawn = 0;
		}
	}
	else if (from.x == 4 && from.y == 0) // black king
	{
		if (to.x == 2 && to.y == 0)  // left castle
		{
			tiles[3][0].color = tiles[0][0].color;
			tiles[3][0].pawn = tiles[0][0].pawn;
			tiles[3][0].hasMoved = true;
			tiles[0][0].color = 0;
			tiles[0][0].pawn = 0;
		}
		else if (to.x == 6 && to.y == 0) // right castle
		{
			tiles[5][0].color = tiles[7][0].color;
			tiles[5][0].pawn = tiles[7][0].pawn;
			tiles[5][0].hasMoved = true;
			tiles[7][0].color = 0;
			tiles[7][0].pawn = 0;
		}
	}
	chess::Tile capturedPiece = tiles[to.x][to.y];
	tiles[to.x][to.y].color = tiles[from.x][from.y].color;
	tiles[to.x][to.y].pawn = tiles[from.x][from.y].pawn;
	tiles[from.x][from.y].color = 0;
	tiles[from.x][from.y].pawn = 0;
	tiles[to.x][to.y].hasMoved = true;
	return capturedPiece;
}

void chess::removeCheck(std::vector<std::pair<Position,Position>>& moves,std::array<std::array<Tile,8>,8> tiles)
{
	int turn = tiles[moves[0].first.x][moves[0].first.y].color;
	int oponent = turn ? 0 : 1;
	for (int i=0;i<moves.size();i++)
	{
		const auto& move = moves[i];
		Tile removedTile = chess::move(tiles, move.first, move.second);
		std::vector<Position> oponentMoves;
		Position kingPosition;
		for (int i = 0; i < tiles.size(); ++i)
		{
			for (int j = 0; j < tiles[i].size(); ++j)
			{
				if (!tiles[i][j].isEmpty() && tiles[i][j].color != turn)
				{
					auto moves = getMoves(tiles, oponent, Position(i, j));
					oponentMoves.insert(oponentMoves.end(), moves.begin(), moves.end());
				}
				if (tiles[i][j].pawn == 6 && tiles[i][j].color == turn)
				{
					kingPosition = Position(i, j);
				}
			}
		}
		bool removed = false;
		for (Position pos : oponentMoves)
		{
			if (pos.x == kingPosition.x && pos.y == kingPosition.y)
			{
				chess::move(tiles, move.second, move.first);
				tiles[move.second.x][move.second.y] = removedTile;
				moves.erase(moves.begin() + i);
				i--;
				removed = true;
				break;
			}
		}
		if (!removed)
		{
			chess::move(tiles, move.second, move.first);
			tiles[move.second.x][move.second.y] = removedTile;
		}
	}
}

void chess::removeCheck(std::vector<Position>& positions, std::array<std::array<Tile, 8>, 8> tiles, Position pos)
{
	chess::Tile currentTile = tiles[pos.x][pos.y];
	Position kingPosition;
	for (int p=0;p<positions.size();p++)
	{
		chess::Tile removedTile = tiles[positions[p].x][positions[p].y];
		move(tiles, pos, positions[p]);
		std::vector<Position> oponentMoves;
		int turn = currentTile.color ? 0 : 1;
		for (int i = 0; i < tiles.size(); ++i)
		{
			for (int j = 0; j < tiles[i].size(); ++j)
			{
				if (!tiles[i][j].isEmpty() && tiles[i][j].color != currentTile.color)
				{
					auto moves = getMoves(tiles, turn, Position(i, j));
					oponentMoves.insert(oponentMoves.end(), moves.begin(), moves.end());
				}
				if (tiles[i][j].pawn == 6 && tiles[i][j].color == currentTile.color)
				{
					kingPosition = Position(i, j);
				}
			}
		}
		bool removed = false;
		for (int o = 0; o < oponentMoves.size(); o++)
		{
			if (oponentMoves[o].x == kingPosition.x && oponentMoves[o].y == kingPosition.y)
			{
				move(tiles, positions[p], pos);
				tiles[positions[p].x][positions[p].y].color = removedTile.color;
				tiles[positions[p].x][positions[p].y].pawn = removedTile.pawn;
				tiles[positions[p].x][positions[p].y].hasMoved = removedTile.hasMoved;
				positions.erase(positions.begin() + p);
				p--;
				removed = true;
				break;
			}
		}
		if (!removed)
		{
			move(tiles, positions[p], pos);
			tiles[positions[p].x][positions[p].y].color = removedTile.color;
			tiles[positions[p].x][positions[p].y].pawn = removedTile.pawn;
			tiles[positions[p].x][positions[p].y].hasMoved = removedTile.hasMoved;
		}
	}
}

std::vector<Position> chess::getMoves(std::array<std::array<chess::Tile, 8>, 8> & board, int turn, Position pos)
{
	chess::Tile currentTile = board[pos.x][pos.y];
	//std::vector<Position> positions;
	if (currentTile.color == turn)
	{
		switch (currentTile.pawn)
		{
		case 1: // pawn
		{
			return chess::getPawnMoves(board, pos);
		}
		case 2: // rook
		{
			return chess::getRookMoves(board, pos);
		}
		case 3: // knight
		{
			return chess::getknightMoves(board, pos);
		}
		case 4: // bishop
		{
			return chess::getBishopMoves(board, pos);
		}
		case 5: // queen
		{
			return chess::getQueenMoves(board, pos);
		}
		case 6: // king
		{
			return chess::getKingMoves(board, pos);
		}
		default:
			break;
		}
	}
}

bool chess::checkCheckMate(std::array<std::array<chess::Tile,8>,8> tiles,int turn)
{
	std::vector<Position> myPositions;
	for (int i=0;i<tiles.size();i++)
	{
		for (int j=0;j<tiles[i].size();j++)
		{
			if (tiles[i][j].color == turn)
			{
				auto moves = getMoves(tiles, turn, Position(i, j));
				myPositions.insert(myPositions.end(), moves.begin(), moves.end());
				removeCheck(myPositions, tiles, Position(i, j));
				if (myPositions.size() > 0) return false;
			}
		}
	}
	return true;
}

void chess::ChessBoard::addCapturedPiece(const chess::Tile& captured)
{
	this->capturedPieces.push_back(captured);
}

void chess::ChessBoard::removeCapturedPiece(const chess::Tile& released)
{
	// to be made
}