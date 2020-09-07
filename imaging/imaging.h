#ifndef IMAGING_H
#define IMAGING_H

#include <iostream>
#include "..//game/chess.h"
#include "TextureManager.h"
#include <TGUI/TGUI.hpp>

struct StyleManager
{
	sf::Color blackTileColor = sf::Color(127,184,132);
	sf::Color whiteTileColor = sf::Color(238, 238, 210);
	std::string pawnStyle;
};


class imagingTile
{
public:
	chess::Tile* tile;
	sf::RectangleShape sprite;
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
	StyleManager style;
	int windowWidth = 800, windowHeight = 800;
	int widthOffset = 100;
	tgui::Button::Ptr undo, redo;
	tgui::Gui gui{ window };
public:
	ChessGame(StyleManager style);
	//ChessGame(sf::RenderWindow& window);
	void run();
	void draw();
	void handleInput();
	void promotePawn(chess::Tile* pawn);
	void setStyle(StyleManager style) { this->style = style; }
private:
	void setupPawns();
	void removeFocus();
	Position getFocusedTile();
};

#endif // !IMAGING_H

