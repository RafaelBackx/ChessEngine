#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include <array>
#include <vector>
#include "..//util/position.h"
#include "SFML/Graphics.hpp"
namespace chess
{
	struct Tile
	{
		Tile(int color, int pawn) : pawn(pawn), color(color) {};
		Tile(int color) : color(color), pawn(0) {}
		Tile() : color(0), pawn(0) {}
		int pawn; // 0=no_pawn, 1=pawn, 2=rook, 3=knight, 4=bishop, 5=queen, 6=king
		int color; // 0=black, 1=white
		bool hasMoved = false;
		bool isEmpty() { return pawn == 0; }
		bool isPawn() { return pawn == 1; }
		bool isRook() { return pawn == 2; }
		bool isKnight() { return pawn == 3; }
		bool isBishop() { return pawn == 4; }
		bool isQueen() { return pawn == 5; }
		bool isKing() { return pawn == 6; }
	};

	struct Move 
	{
		Position from, to;
		Tile capturedPiece;
		friend std::ostream& operator<<(std::ostream& output, const Move& move)
		{
			output << "from: " << move.from << ", to: " << move.to << "captured piece: " << move.capturedPiece.pawn << " color: " << move.capturedPiece.color;
			return output;
		}
	};

	std::vector<Position> getPawnMoves(std::array<std::array<Tile, 8>, 8> tiles, Position pos);
	std::vector<Position> getRookMoves(std::array<std::array<Tile, 8>, 8> tiles, Position pos);
	std::vector<Position> getknightMoves(std::array<std::array<Tile, 8>, 8> tiles, Position pos);
	std::vector<Position> getBishopMoves(std::array<std::array<Tile, 8>, 8> tiles, Position pos);
	std::vector<Position> getQueenMoves(std::array<std::array<Tile, 8>, 8> tiles, Position pos);
	std::vector<Position> getKingMoves(std::array<std::array<Tile, 8>, 8> tiles, Position pos);
	void checkCastling(std::vector<Position>& positions, std::array<std::array<Tile, 8>, 8> tiles, Position pos);
	void checkMoves(std::vector<Position>& positions, std::array<std::array<chess::Tile,8>,8> tiles, chess::Tile currentTile);
	chess::Tile move(std::array<std::array<chess::Tile, 8>, 8>& tiles, Position from, Position to); // returns the captured piece
	void removeCheck(std::vector<Position>& positions, std::array<std::array<Tile, 8>, 8> tiles, Position pos);
	void removeCheck(std::vector<std::pair<Position, Position>>& moves, std::array<std::array<Tile, 8>, 8> tiles);
	std::vector<Position> getMoves(std::array<std::array<Tile, 8>, 8> & board, int turn, Position pos);
	bool checkCheckMate(std::array<std::array<Tile, 8>, 8> board, int turn);

	class ChessBoard 
	{
	public:
		std::vector<chess::Tile> capturedPieces;
		bool checkMate = false;
		int turn = 1;
		std::array<std::array<chess::Tile, 8>, 8> tiles;
		ChessBoard(){}
		std::array<std::array<chess::Tile, 8>, 8> & getTiles() { return tiles; }
		const std::array<std::array<chess::Tile, 8>, 8> & getTiles() const { return tiles; }
		const std::vector<chess::Tile> getCapturedPieces() const { return this->capturedPieces; }
		std::vector<chess::Tile>& getCapturedPieces() { return this->capturedPieces; }
		int getTurn() const { return turn; }
		void setTurn(int turn) { this->turn = turn; }
		void setCheckMate(bool checkmate) { this->checkMate = checkmate; }
		bool isCheckMate() const { return this->checkMate; }
		void addCapturedPiece(const chess::Tile& captured);
		void removeCapturedPiece(const chess::Tile& released);
	};
	inline void switchTurns(chess::ChessBoard& board) { board.setTurn(board.getTurn() ? 0 : 1); }
}
#endif
