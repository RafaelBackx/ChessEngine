#ifndef NETWORK_H
#define NETWORK_H
#include "SFML/Network.hpp"
#include "..//imaging/imaging.h"

namespace network
{
	struct NetworkPackage
	{
		chess::ChessBoard chessboard;
		//std::vector<chess::Move> history;
		//int historyIndex;
	};

	sf::Packet& operator << (sf::Packet packet, const NetworkPackage nPackage);
	sf::Packet& operator << (sf::Packet packet, const chess::Tile tile);
	sf::Packet& operator << (sf::Packet packet, const chess::Move tile);
	sf::Packet& operator << (sf::Packet packet, const Position tile);
	sf::Packet& operator << (sf::Packet packet, const chess::ChessBoard board);

	class ChessGameNetwork
	{
	private:
		sf::TcpSocket& socket;
		chess::ChessBoard chessboard;
		std::vector<chess::Move> history;
		int historyIndex=0;
		bool turn = true;
		bool color; // this will determine who is white and who is black
		sf::RenderWindow window;
		tgui::Gui gui;
		std::array<std::array<imagingTile, 8>, 8> board;
		int tileWidth = 100;
		sf::Color black_color = sf::Color::Color(118, 150, 86);
		sf::Color white_color = sf::Color::Color(238, 238, 210);
		int windowWidth = 800, windowHeight = 800;
		int widthOffset = 100;
		TextureManager tmanager;
	public:
		ChessGameNetwork(sf::TcpSocket& socket, bool color) : socket(socket), color(color)
		{
			window.create(sf::VideoMode(windowWidth + 2 * widthOffset, windowHeight), "Chess game over network - " + socket.getRemoteAddress().toString());
			gui.setTarget(window);
			sf::Color c = black_color;
			for (int i = 0; i < this->board.size(); i++)
			{
				for (int j = 0; j < this->board[i].size(); j++)
				{
					c = (i + j) % 2 == 0 ? this->black_color : this->white_color;
					board[i][j].rect = sf::RectangleShape(sf::Vector2f(tileWidth, tileWidth));
					board[i][j].sprite = sf::RectangleShape(sf::Vector2f(tileWidth, tileWidth));
					board[i][j].rect.setFillColor(c);
					board[i][j].defaultColor = c;
					board[i][j].rect.setPosition((i * tileWidth) + widthOffset, j * tileWidth);
					board[i][j].sprite.setPosition((i * tileWidth) + widthOffset, j * tileWidth);
					board[i][j].pos = Position(i, j);
					board[i][j].tile = &this->chessboard.getTiles()[i][j];
				}
			}
			setupPawns();
		};
		void run();
		void draw();
		void handleInput();
		void sendOverNetwork();
	private:
		void setupPawns();
		void removeFocus();
		Position getFocusedTile();
		void promotePawn(chess::Tile* pawn);
	};
}


#endif // !NETWORK_H

