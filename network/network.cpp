#include "network.h"

void network::ChessGameNetwork::run()
{
	while(this->window.isOpen())
	{
		this->handleInput();
		this->draw();
	}
}

void network::ChessGameNetwork::handleInput()
{
	sf::Event event;
	while(this->window.pollEvent(event))
	{
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

					if (this->historyIndex < this->history.size())
					{
						chess::Move move = { pos, Position(x,y), this->chessboard.getTiles()[x][y] };
						this->history[this->historyIndex++] = move;
					}
					else if (this->historyIndex == this->history.size())
					{
						this->history.push_back({ pos, Position(x,y), this->chessboard.getTiles()[x][y] });
						this->historyIndex++;
					}
					else
					{
						std::cout << "something wrong with history index" << std::endl;
					}

					chess::move(this->chessboard.getTiles(), pos, Position(x, y));
					if (this->chessboard.getTiles()[x][y].pawn == 1 && (y == 0 || y == 7))
					{
						std::cout << "Promoting pawn" << std::endl;
						promotePawn(&this->chessboard.getTiles()[x][y]);
					}
					removeFocus();
					chess::switchTurns(this->chessboard);
					std::cout << "turn: " << this->chessboard.getTurn() << std::endl;
					// check for checkmate
					//std::cout << "Checkmate ? ";
					//this->chessboard.setCheckMate(chess::checkCheckMate(this->chessboard.getTiles(), this->chessboard.getTurn()));
					//auto bestMove = getBestMove(this->chessboard,3  ,true);
					//std::cout << "Best move: " << " from " << bestMove.first << " to " << bestMove.second << std::endl;
					//chess::move(this->chessboard.getTiles(), bestMove.first, bestMove.second);
					//chess::switchTurns(this->chessboard);
				}
				else if (board[x][y].tile->pawn != 0)
				{
					removeFocus();
					board[x][y].state = 1;
					auto moves = getMoves(this->chessboard.getTiles(), this->chessboard.getTurn(), Position(x, y));
					chess::removeCheck(moves, this->chessboard.getTiles(), Position(x, y));
					for (Position pos : moves)
					{
						this->board[pos.x][pos.y].state = 2;
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
	//draw undo and redo buttons
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
		sf::Color c = counter % 2 == 0 ? this->black_color : this->white_color;
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

void network::ChessGameNetwork::sendOverNetwork()
{

}