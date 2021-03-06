#include "network.h"


network::ChessGameNetwork::ChessGameNetwork(sf::TcpSocket& socket, bool color,StyleManager style) : socket(socket), color(color), style(style)
{
	window.create(sf::VideoMode(windowWidth + 2 * widthOffset, windowHeight), "Chess game over network - " + socket.getRemoteAddress().toString());
	gui.setTarget(window);
	sf::Color c = style.blackTileColor;
	for (int i = 0; i < this->board.size(); i++)
	{
		for (int j = 0; j < this->board[i].size(); j++)
		{
			c = (i + j) % 2 == 0 ? this->style.blackTileColor : this->style.whiteTileColor;
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
	this->turnLabel = tgui::Label::create();
	turnLabel->setText("Turn: " + this->chessboard.getTurn() ?  "White" : "Black");
	turnLabel->setPosition({ 0,0 });
	turnLabel->setSize({ this->widthOffset,20 });
	gui.add(turnLabel);
	setupPawns();
}


void network::ChessGameNetwork::run()
{
	this->socket.setBlocking(true);// turn on blocking again so when you make a move your thread is blocked until your oponent responds with his move
	while(this->window.isOpen())
	{
		this->draw();
		this->handleInput();
	}
}

void network::ChessGameNetwork::handleInput()
{
	sf::Event event;
	while(this->window.pollEvent(event))
	{
		this->socket.setBlocking(false);
		//check for incoming packages
		sf::Packet response;
		//std::cout << "Waiting response from oponent" << std::endl;
		auto status = this->socket.receive(response);
		if (status == sf::Socket::Status::Done)
		{
			// cast response data to network::NetworkPackage
			NetworkPackage receivedData;
			response >> receivedData;
			std::cout << "Data received" << std::endl;
			this->chessboard = receivedData.chessboard;
		}

		this->gui.handleEvent(event);
		//handle all events
		switch (event.type)
		{
		case sf::Event::Closed:
		{
			//socket.disconnect(); not sure about this
			window.close();
		}
		case sf::Event::MouseButtonPressed:
		{
			int x = ((event.mouseButton.x - widthOffset) / (double)windowWidth) * 8;
			int y = (event.mouseButton.y / (double)windowHeight) * 8;
			std::cout << "Tile[" << x << "][" << y << "]" << " was pressed" << std::endl;
			if (x > -1 && x<8 && y>-1 && y < 8)
			{

				if (board[x][y].state == 2) // clicked on a possible move
				{
					Position pos = getFocusedTile();
					chess::Move move = { pos, Position(x,y), this->chessboard.getTiles()[x][y] };

					//this->historyManager.addMove(move);

					chess::move(this->chessboard.getTiles(), pos, Position(x, y));
					if (this->chessboard.getTiles()[x][y].pawn == 1 && (y == 0 || y == 7))
					{
						std::cout << "Promoting pawn" << std::endl;
						promotePawn(&this->chessboard.getTiles()[x][y]);
					}
					removeFocus();
					chess::switchTurns(this->chessboard);
					std::cout << "Turn: " << this->chessboard.getTurn() << std::endl;

					// sendOverTcp to oponent
					this->sendOverNetwork();
				}
				else if (board[x][y].tile->pawn != 0)
				{
					removeFocus();
					board[x][y].state = 1;
					if (board[x][y].tile->color == this->color)
					{
						auto moves = getMoves(this->chessboard.getTiles(), this->chessboard.getTurn(), Position(x, y));
						chess::removeCheck(moves, this->chessboard.getTiles(), Position(x, y));
						for (Position pos : moves)
						{
							this->board[pos.x][pos.y].state = 2;
						}
					}
				}
			}
		}
		default:
			break;
		}
	}
}

void network::ChessGameNetwork::draw()
{
	this->window.clear(sf::Color::Color(200, 200, 200));
	for (int i = 0; i < this->board.size(); i++)
	{
		for (int j = 0; j < this->board[i].size(); j++)
		{
			board[i][j].draw(this->window, this->tmanager);
		}
	}
	turnLabel->setText("Turn: " + this->chessboard.getTurn() ? "White" : "Black");
	this->gui.draw();
	this->window.display();
}

Position network::ChessGameNetwork::getFocusedTile()
{
	for (int x=0;x<8;x++)
	{
		for (int y=0;y<8;y++)
		{
			if (this->board[x][y].state == 1)// focussed tile
			{
				return Position(x, y);
			}
		}
	}
}

void network::ChessGameNetwork::removeFocus()
{
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			this->board[x][y].state = 0;
		}
	}
}

void network::ChessGameNetwork::promotePawn(chess::Tile* pawn)
{
	//create new window like in chess.com
	sf::RenderWindow promoteWindow;
	promoteWindow.create(sf::VideoMode(400, 100), "", sf::Style::None);
	//create the 4 possible imagingTiles
	std::array<imagingTile, 4> promotions;
	std::array<chess::Tile, 4> tiles;
	int counter = 0;
	for (int i = 0; i < 4; i++)
	{
		sf::Color c = counter % 2 == 0 ? this->style.blackTileColor : this->style.whiteTileColor;
		imagingTile tile;
		chess::Tile t(pawn->color, 0);
		tiles[i] = t;
		tile.rect = sf::RectangleShape(sf::Vector2f(this->tileWidth, this->tileWidth));
		tile.rect.setFillColor(c);
		tile.defaultColor = c;
		tile.tile = &tiles[i];
		tile.pos = Position(i * tileWidth, 0);
		tile.rect.setPosition(sf::Vector2f(i * tileWidth, 0));
		promotions[i] = tile;
		++counter;
	}
	promotions[0].tile->pawn = 5;
	promotions[1].tile->pawn = 2;
	promotions[2].tile->pawn = 4;
	promotions[3].tile->pawn = 3;
	while (promoteWindow.isOpen())
	{
		sf::Event event;
		while (promoteWindow.pollEvent(event))
		{
			if (event.type == sf::Event::MouseButtonPressed)
			{
				int x = event.mouseButton.x / this->tileWidth;
				std::cout << x << std::endl;
				pawn->pawn = tiles[x].pawn;
				promoteWindow.close();
			}
		}
		promoteWindow.clear(sf::Color::White);
		for (imagingTile tile : promotions)
		{
			tile.draw(promoteWindow, this->tmanager);
		}
		promoteWindow.display();
	}
}

void network::ChessGameNetwork::setupPawns()
{
	// real setup
	for (int i = 0; i < 8; i++) this->board[i][0].tile->color = 0;
	this->board[0][0].tile->pawn = 2;
	this->board[1][0].tile->pawn = 3;
	this->board[2][0].tile->pawn = 4;
	this->board[4][0].tile->pawn = 6;
	this->board[3][0].tile->pawn = 5;
	this->board[5][0].tile->pawn = 4;
	this->board[6][0].tile->pawn = 3;
	this->board[7][0].tile->pawn = 2;

	for (int i = 0; i < 8; i++)
	{
		board[i][1].tile->pawn = 1;
		board[i][1].tile->color = 0;
	}

	for (int i = 0; i < 8; i++) this->board[i][7].tile->color = 1;
	this->board[0][7].tile->pawn = 2;
	this->board[1][7].tile->pawn = 3;
	this->board[2][7].tile->pawn = 4;
	this->board[4][7].tile->pawn = 6;
	this->board[3][7].tile->pawn = 5;
	this->board[5][7].tile->pawn = 4;
	this->board[6][7].tile->pawn = 3;
	this->board[7][7].tile->pawn = 2;
	for (int i = 0; i < 8; i++)
	{
		board[i][6].tile->pawn = 1;
		board[i][6].tile->color = 1;
	}
}

void network::ChessGameNetwork::sendOverNetwork()
{
	this->socket.setBlocking(true);
	this->draw();
	NetworkPackage nPackage;
	nPackage.chessboard = this->chessboard;
	//nPackage.history = this->history;
	//nPackage.historyIndex = this->historyIndex;
	sf::Packet packet;
	packet << nPackage;
	NetworkPackage check; // to be removed later
	packet >> check;
	//packet.append(&nPackage, sizeof(nPackage));
	auto status = this->socket.send(packet);
	if (status == sf::Socket::Partial)
	{
		this->socket.send(packet);
	}
	else if(status == sf::Socket::Done)
	{
		std::cout << "Packet sent succesfully" << std::endl;
	}
}

sf::Packet& network::operator << (sf::Packet& packet, const network::NetworkPackage& nPackage)
{	
	return packet << nPackage.chessboard;
}

sf::Packet& network::operator <<(sf::Packet& packet, const chess::Tile& tile)
{
	return packet << tile.pawn << tile.color << tile.hasMoved;
}

sf::Packet& network::operator << (sf::Packet& packet, const chess::ChessBoard& board)
{
	auto tiles = board.getTiles();
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			packet << tiles[x][y];
		}
	}
	packet << board.turn;
	packet << board.checkMate;
	return packet;
}

sf::Packet& network::operator <<(sf::Packet& packet, const chess::Move& move)
{
	return packet << move.from << move.to << move.capturedPiece;
}

sf::Packet& network::operator <<(sf::Packet& packet, const Position& position)
{
	return packet << position.x << position.y;
}

sf::Packet& network::operator >>(sf::Packet& packet, network::NetworkPackage& nPackage) 
{
	return packet >> nPackage.chessboard;
}

sf::Packet& network::operator >>(sf::Packet& packet, chess::Tile& tile) 
{
	return packet >> tile.pawn >> tile.color >> tile.hasMoved;
}

sf::Packet& network::operator >>(sf::Packet& packet, chess::Move& tile) 
{
	return packet >> tile.from >> tile.to >> tile.capturedPiece;
}

sf::Packet& network::operator >>(sf::Packet& packet, Position& tile) 
{
	return packet >> tile.x >> tile.y;
}

sf::Packet& network::operator >>(sf::Packet& packet, chess::ChessBoard& board) 
{
	auto tiles = board.getTiles();
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			packet >> tiles[x][y];
		}
	}
	board.tiles = tiles;
	packet >> board.turn;
	packet >> board.checkMate;
	return packet;
}