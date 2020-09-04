#ifndef IMAGING_H
#define IMAGING_H

#include <iostream>
#include "..//game/chess.h"
#include "TextureManager.h"
#include <TGUI/TGUI.hpp>
class imagingTile
{
public:
	chess::Tile* tile;
	sf::RectangleShape rect;
	Position pos;
	sf::Color defaultColor;
	sf::Color focusColor = sf::Color::Yellow;
	sf::Color possibleMoveColor = sf::Color::Green;
	int state = 0; // 0 = default, 1=focus, 2=possibleMove
	void draw(sf::RenderWindow& window, TextureManager& tmanager);
	//std::vector<Position> getMoves(std::array<std::array<chess::Tile, 8>, 8>& board, int turn);
};


class ChessGame
{
private:
	chess::ChessBoard chessboard;
	std::vector<chess::Move> history;
	int historyIndex=0;
	TextureManager textureManager;
	sf::RenderWindow window;
	std::array<std::array<imagingTile, 8>, 8> board;
	int tileWidth = 100;
	sf::Color black_color = sf::Color::Color(118, 150, 86);
	sf::Color white_color = sf::Color::Color(238, 238, 210);
	int windowWidth = 800, windowHeight = 800;
	int widthOffset = 100;
	tgui::Button::Ptr undo, redo;
	tgui::Gui gui{ window };
public:
	ChessGame();
	//ChessGame(sf::RenderWindow& window);
	void run();
	void draw();
	void handleInput();
	void promotePawn(chess::Tile* pawn);
private:
	void setupPawns();
	void removeFocus();
	Position getFocusedTile();
};

#endif // !IMAGING_H

