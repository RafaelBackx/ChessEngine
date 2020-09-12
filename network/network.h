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

	sf::Packet& operator <<(sf::Packet& packet, const NetworkPackage& nPackage);
	sf::Packet& operator <<(sf::Packet& packet, const chess::Tile& tile);
	sf::Packet& operator <<(sf::Packet& packet, const chess::Move& move);
	sf::Packet& operator <<(sf::Packet& packet, const Position& position);
	sf::Packet& operator <<(sf::Packet& packet, const chess::ChessBoard& board);

	sf::Packet& operator >>(sf::Packet& packet, NetworkPackage& nPackage);
	sf::Packet& operator >>(sf::Packet& packet, chess::Tile& tile);
	sf::Packet& operator >>(sf::Packet& packet, chess::Move& tile);
	sf::Packet& operator >>(sf::Packet& packet, Position& tile);
	sf::Packet& operator >>(sf::Packet& packet, chess::ChessBoard& board);

	class ChessGameNetwork
	{
	private:
		sf::TcpSocket& socket;
		chess::ChessBoard chessboard;
		HistoryManager historyManager;
		bool color; // this will determine who is white and who is black
		sf::RenderWindow window;
		tgui::Gui gui;
		std::array<std::array<imagingTile, 8>, 8> board;
		int tileWidth = 100;
		StyleManager style;
		int windowWidth = 800, windowHeight = 800;
		int widthOffset = 100;
		TextureManager tmanager;
		tgui::Label::Ptr turnLabel;
	public:
		ChessGameNetwork(sf::TcpSocket& socket, bool color, StyleManager style);
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

