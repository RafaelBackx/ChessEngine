#ifndef NETWORK_H
#define NETWORK_H
#include "SFML/Network.hpp"
#include "..//imaging/imaging.h"

namespace network
{
	struct NetworkPackage
	{
		chess::ChessBoard chessboard;
		int turn;
		std::vector<chess::Move> history;
		int historyIndex;
	};

	class ChessGameNetwork
	{
	private:
		sf::TcpSocket& socket;
		chess::ChessBoard chessboard;
		std::vector<chess::Move> history;
		int historyIndex=0;
		bool turn = true;
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
		ChessGameNetwork(sf::TcpSocket& socket) : socket(socket)
		{
			window.create(sf::VideoMode(windowWidth + 2 * widthOffset, windowHeight), "Chess game over network - " + socket.getRemoteAddress().toString());
			gui.setTarget(window);
		};
		void run();
		void draw();
		void handleInput();
		void sendOverNetwork();
	private:
		void removeFocus();
		Position getFocusedTile();
		void promotePawn(chess::Tile* pawn);
	};
}


#endif // !NETWORK_H

